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

#include <stdio.h>
#include <stdarg.h>


#include "device.h"
#include "plugin.h"
#include "timestamp.h"



/*****************************************************************************/
/* Device CLASS IMPLEMENTATION                                               */
/*****************************************************************************/


/*---------------------------------------------------------------------------*/

Device::Device(const char* nam, const char* path, INDIItem* paren) 
  : INDICompositeItem(nam,paren,"Device"), imp(0), fileName(path) 
{ 
}

/* tell client to delete the whole device */
void Device::indiDelProperty()
{
  fprintf(fp,"<delProperty device='%s'", name.c_str());
  if (message) {
    fputs(" message='",fp);
    fputs(msgbuf, fp);
    fputc('\'',fp);
    message = 0;
  }
  fputs(">\n</delProperty>",fp);
  fflush(fp);
}


/*---------------------------------------------------------------------------*/

void Device::indiMessage()
{
 if (message) {
   fprintf(fp, "<message device='%s' timestamp='%s'", name.c_str(),timestamp());
   fputs(" message='",fp);
   fputs(msgbuf, fp);
   fputc('\'',fp);
   message = 0;
   fputs(">\n</message>\n",fp);
   fflush(fp);
  }
}

/*---------------------------------------------------------------------------*/

void Device::xmlSave()
{

  FILE* newfile;

  newfile = fopen(fileName.c_str(),"w+");
  if(newfile == NULL) {
    log->error(IFUN,"error saving XML configuration file %s\n",fileName.c_str());
  }

  fp = newfile;	

  log->info(IFUN,"Saving device state %s in file %s\n",
	   name.c_str(), fileName.c_str());

  // header
  fprintf(fp,"\n<!--  Device %s  -->\n\n",name.c_str());
  fprintf(fp,"<defDevice name='%s'>\n", name.c_str());

  // iterate to save all children 

  INDICompositeItem::xmlSave();

  // footer
  fputs("\n</defDevice>\n\n",fp);

  fp = stdout;			// changes it back
  fclose(newfile);
}

/*---------------------------------------------------------------------------*/

void Device::update(BLOBPropertyVector* pv, char* names[], char* blob[], int n)
{
  if(imp)
    imp->update(pv, names, blob, n);
  else {
    pv->formatMsg("%s Error: Dispositivo no implementado",
		 pv->getName());
    pv->forceChange();
    pv->indiSetProperty();	// returns what's available
  }
}

/*---------------------------------------------------------------------------*/

void Device::update(TextPropertyVector* pv, char* names[], char* text[], int n)
{
  if(imp)
    imp->update(pv, names, text, n);
  else {
    formatMsg("%s: Dispositivo no implementado",
		 pv->getName());
    pv->forceChange();
    pv->indiSetProperty();	// returns what's available
  }
}

/*---------------------------------------------------------------------------*/

void Device::update(SwitchPropertyVector* pv, char* name, ISState swit)
{
  if(imp)
    imp->update(pv, name, swit);
  else {
    pv->formatMsg("%s Error: Dispositivo no implementado",
		  pv->getName());
    pv->forceChange();
    pv->indiSetProperty();	// returns what's available
  }
}

/*---------------------------------------------------------------------------*/

void Device::update(NumberPropertyVector* pv, char* names[], double number[], int n)
{
  if(imp)
    imp->update(pv, names, number, n);
  else {
    pv->formatMsg("%s Error: Dispositivo no implementado",
		 pv->getName());
    pv->forceChange();
    pv->indiSetProperty();	// returns what's available
  }
}

/*---------------------------------------------------------------------------*/

void
Device::update(PropertyVector* pv, ITopic topic)
{
  if(imp)
    imp->update(pv, topic);
  else {
    log->warn(IFUN,"%s: notificacion ignorada para %s\n", getName(), pv->getName());
  }
}

/*---------------------------------------------------------------------------*/
