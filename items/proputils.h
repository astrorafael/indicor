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

#ifndef INDI_PROPUTILS_H
#define INDI_PROPUTILS_H


/*****************************************************************************/
/* HELPER FUNCTIONS                                                          */
/*****************************************************************************/

#define	INDIV	1.6		/* INDI Version */

typedef enum {
  ISS_OFF, ISS_ON
} ISState;			/* switch state */


typedef enum {
  IPS_IDLE, IPS_OK, IPS_BUSY, IPS_ALERT
} IPState;			/* property state and light values */

typedef enum {
  ISR_1OFMANY, ISR_ATMOST1, ISR_NOFMANY
} ISRule;			/* switch vector rule hint */


typedef enum {
  IP_RO, IP_WO, IP_RW
} IPerm;			/* permission hint, WRT client */

typedef enum {
  IT_VALUE, IT_STATE		
} ITopic;			/* INDI topics */

/* conversions from INDI enumerations to string for XML message */

const char* pState2Str (IPState s);
const char* perm2Str (IPerm p);
const char* sState2Str(ISState s);
const char* rule2Str (ISRule r);

/*  conversions from string in XML message to INDI enumerations */
/* okFlag=true => string recognized, okFlag=false => unknown string */

IPState pState4Str(const char* state, bool* okFlag);
ISState sState4Str(const char* sw, bool* okFlag);
ISRule  rule4Str(const char* rule, bool* okFlag);
IPerm   perm4Str(const char* perm, bool* okFlag);


/*****************************************************************************/

/* trims right all \n, \t, spaces, etc in src, */
void strTrimRight(char* dest, const char* src);


#endif
