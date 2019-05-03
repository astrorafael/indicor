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
/*****************************************************************************/
/* HELPER FUNCTIONS                                                          */
/*****************************************************************************/

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "eventloop.h"
#include "proputils.h"

/* return static string corresponding to the given property or light state */

const char* pState2Str (IPState s)
{
  assert((s == IPS_IDLE) || (s == IPS_OK) || (s == IPS_BUSY) || (s == IPS_ALERT));

  switch (s) {
  case IPS_OK:    return ("Ok");
  case IPS_BUSY:  return ("Busy");
  case IPS_ALERT: return ("Alert");
  default:
    return("Idle");    
  }

}

/*---------------------------------------------------------------------------*/

IPState pState4Str(const char* state, bool* ok)
{
  *ok = true;
  if(!strncmp(state,"Idle",4))  return IPS_IDLE;
  if(!strncmp(state,"Ok",2))    return IPS_OK;
  if(!strncmp(state,"Busy",3))  return IPS_BUSY;
  if(!strncmp(state,"Alert",5)) return IPS_ALERT;

  assert(false);

  *ok = false;
  return(IPS_IDLE);
}

/*---------------------------------------------------------------------------*/

/* return static string corresponding to the given switch state */

const char* sState2Str (ISState s)
{
  assert((s == ISS_OFF) || (s == ISS_ON));

  if (s == ISS_ON)
    return ("On"); 
  else 
    return ("Off");
}

/*---------------------------------------------------------------------------*/

ISState sState4Str(const char* sw, bool* ok)
{
  *ok = true;
  if(!strncmp(sw,"On",2))  return ISS_ON;
  if(!strncmp(sw,"Off",3)) return ISS_OFF;

  assert(false);

  *ok = false;
  return ISS_OFF;
}

/*---------------------------------------------------------------------------*/

/* return static string corresponding to the given Rule */

const char* rule2Str (ISRule r)
{
  assert((r == ISR_1OFMANY) || (r == ISR_ATMOST1) || (r == ISR_NOFMANY));

  switch (r) {
  case ISR_1OFMANY: return ("OneOfMany");
  case ISR_ATMOST1: return ("AtMostOne");
  default:
    return("AnyOfMany");
  }
}

/*---------------------------------------------------------------------------*/

ISRule rule4Str(const char* rule, bool* ok)
{
  *ok = true;
  if(!strncmp(rule,"OneOfMany",9))  return ISR_1OFMANY;
  if(!strncmp(rule,"AtMostOne",9))  return ISR_ATMOST1;
  if(!strncmp(rule,"AnyOfMany",9))  return ISR_NOFMANY;

  assert(false);

  *ok = false;
  return (ISR_NOFMANY);
}

/*---------------------------------------------------------------------------*/

/* return static string corresponding to the given permission hint */

const char* perm2Str (IPerm p)
{
  assert((p == IP_RO) || (p == IP_WO) || (p == IP_RW));

  switch (p) {
  case IP_WO: return ("wo");
  case IP_RW: return ("rw");
  default:
    return("ro");
  }
}

/*---------------------------------------------------------------------------*/

IPerm perm4Str(const char* perm, bool* ok)
{
  *ok = true;
  if(!strncmp(perm,"ro",2))  return IP_RO;
  if(!strncmp(perm,"rw",2))  return IP_RW;
  if(!strncmp(perm,"ro",2))  return IP_WO;

  assert(false);

  *ok = false;
  return(IP_RO);
}

/*---------------------------------------------------------------------------*/

void strTrimRight(char* dest, const char* src)
{

  assert((dest != NULL) && (src != NULL));

  const char* c = src + strlen(src) - 1;
  
  while (!isalnum(*c) && c >= src) {
    c--;  /* skips trailing blanks CR, LF, TABs, */
  }

  if(c >= src) {
    if(dest != src) 		// avoids copy onto self
      strncpy(dest, src, c-src+1);   
    dest[c-src+1] = 0;
  } else {
    dest[0] = 0;		/* empty string */
  }

  assert(strlen(dest) <= strlen(src));
}

/*---------------------------------------------------------------------------*/
