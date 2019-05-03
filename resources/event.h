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

#ifndef INDI_EVENT_SOURCE_H
#define INDI_EVENT_SOURCE_H

/* This interface will be implemente by internal classes that decouple 
 * two threads of execution in an observable:
 * - the one that generates the event
 * - the one that notifies its observers
 *
 * there should be one global object in the whole process 
 * implementing this interface.
 */

template<typename Observable>
class EventSource  {

 public:

  typedef void (Observable::* Action)();

  virtual ~EventSource() {}

  /* observable invoke this to propagate the event */
  virtual void generate(Observable* recv, Action act) = 0;

  /* read an event from the I/O system and invoke the observable again
   * so that it may start with notification to observers 
   */

  virtual void changed(int fd) = 0;

};


#endif
