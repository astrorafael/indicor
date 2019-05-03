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

#ifndef INDI_SIMPLE_ALARM_H
#define INDI_SIMPLE_ALARM_H

#include <set>			/* STL's set container */

#include "log.h"
#include "logfact.h"
#include "alarm.h"
#include "eventloop.h"

/*
 * a generic implementation for a simple alarm 
 *
 *
 * The only requirement for receivers is to implement the timeout() method
 *
 * This implementation uses the eventloop facility of the original INDI
 * library by Elwood C. Downey.
 *
 *
 */


template<typename Receiver>
class AlarmImp : public Alarm<Receiver> {

 public:

   AlarmImp();
  ~AlarmImp();

  /* register a receiver */
  virtual void add(Receiver* recv);
  
  /* unregister a receiver */
  virtual void remove(Receiver* recv);

  /* starts the alarm using a given timeout value */
  virtual void start(unsigned int timeout);

  /* cancels the alarm */
  virtual void cancel();

  /* called by event loop when timer expired */
  virtual void timeout();

 private:

  Log* log;

  int alarmId;

  typedef typename std::set<Receiver*> DeviceSet;
  typedef typename std::set<Receiver*>::iterator DeviceSetIterator;

  DeviceSet deviceSet;
  
};

/*---------------------------------------------------------------------------*/

template<typename Receiver>
void
AlarmImp_timeout(void* timer)
{
  STATIC_CAST(AlarmImp<Receiver>*, timer)->timeout();
}

/*---------------------------------------------------------------------------*/

template<typename Receiver>
AlarmImp<Receiver>::AlarmImp() :
  log(0), alarmId(-1)
{ 
  log = LogFactory::instance()->forClass("AlarmImp"); 
}


/*---------------------------------------------------------------------------*/

template<typename Receiver>
AlarmImp<Receiver>::~AlarmImp()
{
  delete log;
}

/*---------------------------------------------------------------------------*/


template<typename Receiver>
void
AlarmImp<Receiver>::start(unsigned int period)
{
  if(alarmId != -1)
    return;

  log->debug(IFUN,"period = %d\n",period);
  alarmId = ::addTimer(period, AlarmImp_timeout<Receiver>, this);
}

/*---------------------------------------------------------------------------*/

template<typename Receiver>
void
AlarmImp<Receiver>::cancel()
{
  log->debug(IFUN,"\n");
  if(alarmId == -1)
    return;
  ::rmTimer(alarmId);
  alarmId = -1;
}

/*---------------------------------------------------------------------------*/

template<typename Receiver>
void
AlarmImp<Receiver>::timeout()
{
  log->debug(IFUN,"deviceSet.size() = %d\n",deviceSet.size());

  DeviceSetIterator i;

  for(i=deviceSet.begin(); i != deviceSet.end(); i++)
    (*i)->timeout();
  alarmId = -1;
}

/*---------------------------------------------------------------------------*/

template<typename Receiver>
void
AlarmImp<Receiver>::add(Receiver* recv)
{
  deviceSet.insert(typename DeviceSet::value_type(recv));
}

/*---------------------------------------------------------------------------*/

template<typename Receiver>
void
AlarmImp<Receiver>::remove(Receiver* recv)
{
  deviceSet.erase(recv);
}

/*---------------------------------------------------------------------------*/


#endif
