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

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include "item.h"
#include "driver.h"
#include "logfact.h"

static void usage(void);

static int verbose;			/* chatty */


/*---------------------------------------------------------------------------*/
/*                      SECCION DE VARIABLES GLOBALES                        */
/*---------------------------------------------------------------------------*/

char*   me;			// executable name
Driver* driver = 0;		// the global Driver object
const char* pkgdatadir=PKGDATADIR; // the data installation directory

static const char* LOG_CONFIG_FILE="log.xml";

/*---------------------------------------------------------------------------*/

/* print usage message and exit (1) */
static void 
usage(void)
{
  fprintf (stderr, "Usage: %s [options]\n", me);
  fprintf (stderr, "Purpose: INDI Device driver framework.\n");
  fprintf (stderr, "Options:\n");
  fprintf (stderr, " -v    : more verbose to stderr\n");  
  exit (1);
}

/*---------------------------------------------------------------------------*/

int 
main (int ac, char *av[])
{

  /* save handy pointer to our base name */
  for (me = av[0]; av[0][0]; av[0]++)
    if (av[0][0] == '/')
      me = &av[0][1];
  
  /* crack args */
  while (--ac && (*++av)[0] == '-')
    while (*++(*av))
      switch (*(*av)) {
      case 'v':	/* verbose */
	verbose++;
	break;
      default:
	usage();
      }
  
  /* ac remaining args starting at av[0] */
  if (ac > 0)
    usage();



  /**************************/
  /* INITIALIZATION SECTION */
  /**************************/

  /* usage of log objects is pervasive in this library
     so they must be setup first */
  bool res = LogFactory::instance()->read(pkgdatadir, LOG_CONFIG_FILE);
  if(!res) {
    fprintf(stderr, "[ WARN] log XML config. file not found or malformed\n");
  }
  
  driver = Driver::instance();
  driver->init();


  /*******************/
  /* MAIN EVENT LOOP */
  /*******************/

  driver->eventLoop();		/* service client */

  
  /* should never return here */
  fprintf (stderr, "%s: inf loop ended\n", me);


  return (1);
}

