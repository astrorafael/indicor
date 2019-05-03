/* $Id: $ */
/*

  Copyright (C) 2005 Rafael Gonzalez (astrorafael@yahoo.es)

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.
  
  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.
  
  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

*/

#ifndef INDI_SYNC_TIMER_H
#define INDI_SYNC_TIMER_H

#include <set>			/* STL's set container */

#include "log.h"
#include "timer.h"

/*
 * a generic implementation for a timer maintain its period as seen
 * from any receiver. If the basic period is 1000 milliseconds
 * each receiver will be called each 1000 milliseconds, and only one
 * receiver at a time.
 *
 * This means that internally this timer has a period of 1000/Nreceivers
 *
 * The only requirement for receivers is to implement the tick() method
 *
 * This implementation uses the eventloop facility of the original INDI
 * library by Elwood C. Downey.
 *
 * Private instantiation of this timer will be made in the COR library
 * using the Plugin class as Receiver.
 */


template<typename Receiver>
class AdaptiveTimer : public Timer<Receiver> {

 public:

  static const unsigned int DEFAULT_PERIOD = 2000; /* milliseconds */

   AdaptiveTimer();
  ~AdaptiveTimer();

  /* register a receiver */
  void add(Receiver* imp);
  
  /* unregister a receiver */
  void remove(Receiver* imp);

  /* starts the global timer */
  void start();

  /* stops the global timer */
  void stop();

  /* stops invoking callbacks */
  void disable();

  /* allows invoking callbacks */
  void enable();

  /* called by event loop every period when started and enabled */
  void tick();

  /* gets current period in milliseconds */
  unsigned int getPeriod() const;

  /* sets the new period in milliseconds */
  virtual void setPeriod(unsigned int p);

 private:

  Log* log;

  int            timerId;
  unsigned int effPeriod;	/* effective timer period */
  unsigned int    period;	/* period as seen by the client apps */
  bool           enabled;	/* global enable/disable calling receivers */

  typedef typename std::set<Receiver*> DeviceSet;
  typedef typename std::set<Receiver*>::iterator DeviceSetIterator;

  DeviceSet         deviceSet;
  DeviceSetIterator   current;	/* current object to be invoked */
  
  void next();			/* advances iterator */
  void recalc();		/* recalculates effective period based */
};

/*---------------------------------------------------------------------------*/

template<typename Receiver>
void
AdaptiveTimer_tick(void* timer)
{
  STATIC_CAST(AdaptiveTimer<Receiver>*, timer)->tick();
}

/*---------------------------------------------------------------------------*/

template<typename Receiver>
AdaptiveTimer<Receiver>::AdaptiveTimer() :
  log(0), timerId(-1), effPeriod(DEFAULT_PERIOD), period(DEFAULT_PERIOD),
  enabled(true), deviceSet(), current(deviceSet.end())
{ 
  log = LogFactory::instance()->forClass("AdaptiveTimer"); 
}

/*---------------------------------------------------------------------------*/

template<typename Receiver>
AdaptiveTimer<Receiver>::~AdaptiveTimer()
{
  delete log;
}

/*---------------------------------------------------------------------------*/

template<typename Receiver>
void
AdaptiveTimer<Receiver>::enable()
{
  enabled = true;
}

/*---------------------------------------------------------------------------*/

template<typename Receiver>
void
AdaptiveTimer<Receiver>::disable()
{
  enabled = false;
}

/*---------------------------------------------------------------------------*/

template<typename Receiver>
unsigned int
AdaptiveTimer<Receiver>::getPeriod() const
{
  return(period);
}

/*---------------------------------------------------------------------------*/

template<typename Receiver>
void
AdaptiveTimer<Receiver>::recalc()
{
  unsigned int n = deviceSet.size();
  if(n == 0) 
    n = 1;
  effPeriod = period / n;			/* dropping decimals */
}

/*---------------------------------------------------------------------------*/

template<typename Receiver>
void
AdaptiveTimer<Receiver>::setPeriod(unsigned int p)
{ 
  period = p;
  recalc();
}

/*---------------------------------------------------------------------------*/

template<typename Receiver>
void
AdaptiveTimer<Receiver>::start()
{
  if(timerId != -1)
    return;

  timerId = ::addTimer(effPeriod, AdaptiveTimer_tick<Receiver>, this);
  log->debug(IFUN,"this = %x\n",this);
}

/*---------------------------------------------------------------------------*/

template<typename Receiver>
void
AdaptiveTimer<Receiver>::stop()
{
  if(timerId == -1)
    return;
  ::rmTimer(timerId);
  timerId = -1;
}

/*---------------------------------------------------------------------------*/

template<typename Receiver>
void
AdaptiveTimer<Receiver>::tick()
{
  
  log->debug(IFUN,"this = %x\n",this);

  /* call the receivers if enabled */
  if(enabled && deviceSet.size() != 0) {
    Receiver* recv = *(current);
    next();
    recv->tick();
  }

  timerId = ::addTimer(effPeriod, AdaptiveTimer_tick<Receiver>, this); 
}

/*---------------------------------------------------------------------------*/

template<typename Receiver>
void
AdaptiveTimer<Receiver>::next()
{
  /* handle degenerate case when all have gone */
  if(deviceSet.empty())	
    return;

  /* advances current iterator, cycling if necessary */
  current++;			
  if(current == deviceSet.end()) 
    current = deviceSet.begin();
}

/*---------------------------------------------------------------------------*/

template<typename Receiver>
void
AdaptiveTimer<Receiver>::add(Receiver* imp)
{
  deviceSet.insert(typename DeviceSet::value_type(imp));
  if(deviceSet.size() == 1) {
    current = deviceSet.begin();
  }

  recalc();
}

/*---------------------------------------------------------------------------*/

template<typename Receiver>
void
AdaptiveTimer<Receiver>::remove(Receiver* imp)
{
  deviceSet.erase(imp);

  /* advances iterator if it happened to be me */
  if(*(current) == imp)		
    next();

  recalc();			
}

/*---------------------------------------------------------------------------*/



#endif
