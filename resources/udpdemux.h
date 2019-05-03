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

/*
 * CRISTOBAL GARCIA'S COR PROTOCOL DEMULTIMPLEXER
 */

#ifndef INDI_UDP_DEMUX_H
#define INDI_UDP_DEMUX_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>

#include <map>			/* STL's map */

#include "demux.h"
#include "log.h"
#include "logfact.h"
#include "eventloop.h"

template<class Receiver>
class UDPDemux : public Demux<Receiver>
{
 public:

  UDPDemux();
  virtual ~UDPDemux();

  /* registers in the event loop */
  void init(int fd, const struct in_addr& ip);
  
  /* registers a Plugin for a given COR message */
  virtual void add(Receiver* recv, unsigned int event);

  /* unregister a Plugin for a given COR message */
  virtual void remove(unsigned int event);

  /* handles activity on UDP socket by dispatching to upper layers */
  /* used by eventloop */
  virtual void handle(int fd);
  
 private:

#if 0
  /* the static array where incoming messages from COR arrive */
  static Incoming_Message msg;
#endif

  /* the dispatch table */

  typedef typename std::map<unsigned int, Receiver*> CallbackMap;
  CallbackMap callbackMap; 
  
  Log* log;			/* log object */  
  int cid;			/* own callback id */
  int udp_fd;			/* UDP socket file descriptor */
  struct in_addr myIP;		/* my own IP */

};

/*---------------------------------------------------------------------------*/

#if 0
template<class Receiver>
UDPDemux<Receiver>::Incoming_Message msg;
#else
static Incoming_Message msg;
#endif

/*---------------------------------------------------------------------------*/

template<class Receiver>
void 
demuxcallback(int fd, void* demux)
{
  STATIC_CAST(Demux<Receiver>*,demux)->handle(fd);
}

/*---------------------------------------------------------------------------*/

template<class Receiver>
UDPDemux<Receiver>::UDPDemux() 
  : callbackMap(),  log(0), cid(-1)
{
   log = LogFactory::instance()->forClass("UDPDemux");
}

/*---------------------------------------------------------------------------*/

template<class Receiver>
UDPDemux<Receiver>::~UDPDemux()
{
  if (cid != -1)
    rmCallback(cid);
  delete log;
}

/*---------------------------------------------------------------------------*/

template<class Receiver>
void 
UDPDemux<Receiver>::init(int fd, const struct in_addr& ip)
{
  // register callback in the event loop  
   cid = ::addCallback(fd, demuxcallback<Receiver>, this);
   udp_fd = fd;
   myIP = ip;
}

/*---------------------------------------------------------------------------*/

template<class Receiver>
void 
UDPDemux<Receiver>::add(Receiver* dev, unsigned int event)
{
  callbackMap.insert(typename CallbackMap::value_type(event,dev));
}

/*---------------------------------------------------------------------------*/

template<class Receiver>
void 
UDPDemux<Receiver>::remove(unsigned int event)
{
  typename CallbackMap::iterator i;

  i = callbackMap.find(event);
  if(i != callbackMap.end()) {
    callbackMap.erase(i);
  }
}

/*---------------------------------------------------------------------------*/

template<class Receiver>
void 
UDPDemux<Receiver>::handle(int fd)
{
  int len;
  unsigned int event;
  typename CallbackMap::iterator i;
  Receiver* dev;
  struct sockaddr_in peerAddr;
  socklen_t peerLen;

  peerLen= sizeof(peerAddr);
  len = recvfrom(udp_fd, &msg, UDP_BUF_SIZE, 0, 
		 (struct sockaddr *) &peerAddr, &peerLen);

  if(len == -1) {
    log->error(IFUN,"recvfrom() %s\n",strerror(errno));
    return;
  }

  /* 
   * ignores my own requests or other's messages
   * when listening in broadcast mode 
   */

  /* WARNING: CRISTOBAL'S COR PROGRAM HAS A BUG IN THE PROTOCOL.
   *
   * His broadcast message from COR do not set the ORIGIN_COR flag
   * so we cannot discriminate messages based on msg.header.origin == ORIGIN_PC
   * We'll include the test as soon as he fixes it.
   */
  
  if(peerAddr.sin_addr.s_addr == myIP.s_addr) {
    log->verbose(IFUN,"ignoring event not coming from COR %d\n",
		event);
    return;
  }


  /* dispatches event to the appropiate handler */
  
  event = msg.header.peripheal;
  
  i = callbackMap.find(event);
  if(i != callbackMap.end()) {
    dev = (*i).second;
    log->verbose(IFUN,"Handler found for msg type %d with len %d\n",event, len);
    dev->handle(event, &msg, len);
  } else {
    log->warn(IFUN,"No handler found for msg type %d, len = %d\n",event, len);
  }

  log->flush();
}


#endif
