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

#include "sysfactory.h"
#include "logfact.h"


/*---------------------------------------------------------------------------*/

SystemResourcesFactory::SystemResourcesFactory() :
  evso(0), logFp(0)
{
}

/*---------------------------------------------------------------------------*/

SystemResourcesFactory::~SystemResourcesFactory()
{
  if(evso)  delete evso;
  if(logFp) fclose(logFp);

}

/*---------------------------------------------------------------------------*/

PVEventSource*
SystemResourcesFactory::createPVEventSource()
{
  if(!evso)
    evso = new PipePVEventSource();
  return(evso);
}

/*---------------------------------------------------------------------------*/

FILE*
SystemResourcesFactory::createFileStream()
{
  if(logFp)
    return(logFp);

  const char* logFileName = LogFactory::instance()->getLogFile();

  /* Select logfp to be either a real file or stderr */
  
  if(!strcmp(logFileName,"stderr")) {
    logFp = stderr;
  } else {
    logFp = fopen(logFileName, "at");
    if(logFp == NULL) {
      fprintf(stderr, "[ERROR] %s opening file %s\n",
	      IFUN,strerror(errno));
      logFp = stderr;
    }
  }
  return(logFp);
}

/*---------------------------------------------------------------------------*/

