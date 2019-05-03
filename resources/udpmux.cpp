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
#include <errno.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>

#include "udpmux.h"
#include "eventloop.h"
#include "logfact.h"

/*---------------------------------------------------------------------------*/

UDPMux::UDPMux() 
  : log(0), udp_fd(-1)
{
   log = LogFactory::instance()->forClass("UDPMux");
}

/*---------------------------------------------------------------------------*/

UDPMux::~UDPMux()
{
  close(udp_fd);
  delete log;
}

/*---------------------------------------------------------------------------*/


const struct in_addr&
UDPMux::getMyAddress() const
{
  inet_aton(myIP.c_str(), CONST_CAST(struct in_addr*,&myBinIP));
  return(myBinIP);
}

/*---------------------------------------------------------------------------*/


void 
UDPMux::init()
{
  int res;

  struct sockaddr_in myAddr;

  findMyHostIP();

  /* creates and binds the UDP socket */
  /* reuses address and makes it listen in broadcast mode */

  udp_fd = socket(PF_INET, SOCK_DGRAM, 0);
  if(udp_fd <0) {
    log->error(IFUN,"socket() %s\n",strerror(errno));
    return;
  }
  
  /* binds own address in any interface */
  myAddr.sin_family      = AF_INET;
  myAddr.sin_addr.s_addr = INADDR_ANY;
  myAddr.sin_port        = htons(UDP_PORT);

  if(bind(udp_fd, (struct sockaddr *) &myAddr, sizeof(myAddr)) < 0) {
    log->error(IFUN,"bind() %s\n",strerror(errno));
    return;
  }

  int enable = 1;

  res = setsockopt(udp_fd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(enable));
   if(res <0) {
    log->error(IFUN,"setsockopt(SO_REUSEADDR) %s\n",strerror(errno));
    return;
  }
  res = setsockopt(udp_fd, SOL_SOCKET, SO_BROADCAST, &enable, sizeof(enable));
   if(res <0) {
    log->error(IFUN,"setsockopt(SO_BROADCAST) %s\n",strerror(errno));
    return;
  }

}

/*---------------------------------------------------------------------------*/

void 
UDPMux::sendMessage(Outgoing_Message* msg, int len)
{
  int res;
  int n = len;

  msg->header.origin       = ORIGIN_PC;
  res = sendall(STATIC_CAST(char*, msg), &n);
  if(res == -1) {
    log->error(IFUN,"error %s\n",strerror(errno));
    log->debug(IFUN,"written %d of %d bytes\n",n,len);
  }
  assert(res != -1);
}


/*---------------------------------------------------------------------------*/

void
UDPMux::setPeer(const char* ip, unsigned short port)
{
  int enable;

  if(!strcmp(ip,"255.255.255.255")) {
    enable = 1;
  } else {
    enable = 0;
  }
  setRemoteSocket(ip, port);

  if(setsockopt(udp_fd, SOL_SOCKET, SO_BROADCAST, &enable, sizeof(enable))<0) {
    log->error(IFUN,"setsockopt(SO_BROADCAST) %s\n",strerror(errno));
    return;
  }
}

/*---------------------------------------------------------------------------*/

void
UDPMux::setRemoteSocket(const char* addr, unsigned short port)
{

  log->debug(IFUN,"address = %s, port = %u\n",addr,port);
  remoteAddr.sin_family = AF_INET;
  remoteAddr.sin_port   = htons(port);
  memset(&remoteAddr.sin_zero, '\0', 8);
  if( inet_aton(addr, &remoteAddr.sin_addr) == -1) {
    log->error(IFUN,"inet_aton() %s\n",strerror(errno));
    return;
  }
}

/*---------------------------------------------------------------------------*/

void UDPMux::findMyHostIP()
{
  char myName[64];
  struct hostent* h;
  char* ip;

  gethostname(myName, sizeof(myName)-1 );
  log->debug(IFUN,"my name is %s\n",myName);      
  h = gethostbyname(myName);
  if(h == NULL) {
    log->error(IFUN,"%s\n",hstrerror(h_errno));
    return;
  } 

  ip = h->h_addr_list[0];	// searches the first one
  myIP = inet_ntoa(*(STATIC_CAST(struct in_addr*,ip)));

  log->debug(IFUN,"my IP address is %s\n",myIP.c_str());      
}

/*---------------------------------------------------------------------------*/

int 
UDPMux::sendall(const char* buf, int* len)
{
  int total = 0;
  int bytesleft = *len;
  int n = -1;

  while(total < *len) {
    n = sendto(udp_fd, 
	       buf+total, bytesleft, 0,
	       STATIC_CAST(struct sockaddr*,&remoteAddr), sizeof(remoteAddr));
    if (n == -1)
      break;
    total     += n;
    bytesleft -= n;
  }

  *len = total;
  return (n == -1) ? -1 : 0;
}

/*---------------------------------------------------------------------------*/



