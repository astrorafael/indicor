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

#ifndef INDI_PIPE_EVENT_H
#define INDI_PIPE_EVENT_H

#include <errno.h>
#include <assert.h>

#include "log.h"
#include "logfact.h"
#include "eventloop.h"


/* This generic class implements the generic EventSource interface
 * by cretaing a Pipe and writing.
 * When the eventloop facility detects I/O on this pipe
 * calls a global function that invokes an instance of this class
 * again. The object reads the pipe and invokes the observer again
 */

template<typename Observable>
class PipeEventSource : public EventSource<Observable>  {

 public:

  typedef typename EventSource<Observable>::Action Action;

  /* Constructor and destructor */

  PipeEventSource();
  virtual ~PipeEventSource();

  /* observable invoke this to generate a 'changed event' */
  virtual void generate(Observable* obs, Action act);

  /* read an event and invoke changed() on its observable */
  virtual void changed(int fd);
  
 private:

  typedef struct {		// Inner structure
    Observable* observable;	/* observable that changed */
    Action      action;		/* observable callback */
  } ChangeEvent; 
  
  Log* log;
  int pipefd[2];		/* pipe file descriptors */
  int cbkId;			/* callback Id */  

};

/*---------------------------------------------------------------------------*/

template<typename Observable>
void 
callback(int fd, void *object)
{
  STATIC_CAST(EventSource<Observable>*, object)->changed(fd);
}

/*---------------------------------------------------------------------------*/

// creates the internal loopback pipe
template<typename Observable>
PipeEventSource<Observable>::PipeEventSource() :
  log(0)
{
  int res = pipe(pipefd);
  log = LogFactory::instance()->forClass("PipeEventSource");

  if(res == -1) {
    log->error(IFUN,"pipe() %s\n",strerror(errno));
  }
  assert (res == 0);

  cbkId = ::addCallback(pipefd[0], callback<Observable>, this);
}

/*---------------------------------------------------------------------------*/

template<typename Observable>
PipeEventSource<Observable>::~PipeEventSource() 
{ 
  ::rmCallback(cbkId);
  close(pipefd[1]);
  close(pipefd[0]);
  delete log;
}

/*---------------------------------------------------------------------------*/

template<typename Observable>
void 
PipeEventSource<Observable>::generate(Observable* obs, Action act)
{

  ChangeEvent event;
  event.observable = obs;
  event.action     = act;

  // atomic write

  int len = write(pipefd[1], &event, sizeof(ChangeEvent));
  assert(len == sizeof(ChangeEvent));

  log->verbose(IFUN,"wrote to pipe\n");
}


/*---------------------------------------------------------------------------*/

template<typename Observable>
void 
PipeEventSource<Observable>::changed(int fd)
{
  ChangeEvent event;

  log->verbose(IFUN,"about to read from pipe\n");

  // atomic read

  int len = read(fd, &event, sizeof(ChangeEvent));
  assert (len == sizeof(ChangeEvent));

  log->verbose(IFUN,"read from pipe %d bytes\n", len);
  (event.observable->*event.action)(); 	/* calls back */
  
}

/*---------------------------------------------------------------------------*/

#endif
