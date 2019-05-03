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

#ifndef INDI_UDP_MUX_H
#define INDI_UDP_MUX_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <string>

#include "mux.h"
#include "utils.h"
#include "log.h"

/*
 * The UDP Mux implements the MUX interface using an UDP socket
 */

class UDPMux : public Mux
{
 public:

   UDPMux();
  ~UDPMux();

  static Mux* instance();

  /* creates the udp_socket and registers in the event loop */
  /* UDP socket is set in broadcast mode */
  void init();

  /* fills the addres structure to use with sendto() */
  /* if adress is 255.255.255.255, then put the Mux in broadcast mode */
  virtual void setPeer(const char* addr, unsigned short port);

  /* returns my own IP address in dotted format */
  /* used by COR plug-in */
  virtual const char* getMyHostIP() const;
  
  /* sends an outgoing message to COR with 'len' bytes */
  /* used by the various plugins */
  virtual void sendMessage(Outgoing_Message* msg, int len);


  /* returns socket file descriptor to add to eventloop */
  /* used by the COR plug-in */
  /* used only once, at startup */
  int getSocket() const;

  const struct in_addr& getMyAddress() const;

 private:

  /* find my own IP address  */
  void findMyHostIP();

  /* handles partial send() in socket calls */
  int sendall(const char* buf, int* len);

  /* fills in the inet address structure */
  void setRemoteSocket(const char* addr, unsigned short port);
  

  Log* log;			/* log object */
  std::string myIP;		/* my own IP in dotted format */
  int udp_fd;			/* UDP socket file descriptor */
  struct sockaddr_in remoteAddr; /* UDP remote address/port */
  struct in_addr     myBinIP;	/* my IP in binary format (last minute hack) */
};


/*---------------------------------------------------------------------------*/

inline const char*
UDPMux::getMyHostIP() const
{
  return (myIP.c_str()); 
}

/*---------------------------------------------------------------------------*/

inline int
UDPMux::getSocket() const 
{ 
  return udp_fd; 
}


#endif
