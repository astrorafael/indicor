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


#include "property.h"
#include "log.h"


/*****************************************************************************/
/* Property CLASS IMPLEMENTATION                                             */
/*****************************************************************************/

void Property::indiDefProperty()
{

  log->verbose(IFUN,"sending %s %s\n",defTag(),getName());

  if(label != "") {
    fprintf(fp,"\t<%s name='%s' label='%s'>",defTag(), name.c_str(), label.c_str());
  } else {
    fprintf(fp,"\t<%s name='%s'>",defTag(), name.c_str());
  }
  fprintf(fp,"%s",pcData());
  fprintf(fp,"</%s>\n", defTag());
  changed = false;
}

/*---------------------------------------------------------------------------*/

void Property::xmlSave()
{
  if(label != "") {
    fprintf(fp,"\t\t<%s name='%s' label='%s'>",defTag(), name.c_str(), label.c_str());
  } else {
    fprintf(fp,"\t\t<%s name='%s'>",defTag(), name.c_str());
  }
  fprintf(fp,"\n\t\t\t%s",pcData());
  fprintf(fp,"\n\t\t</%s>\n", defTag());
}

/*---------------------------------------------------------------------------*/

void Property::indiOneProperty()
{
  log->verbose(IFUN,"sending %s %s\n",oneTag(),getName());

  fprintf(fp,"\t<%s name='%s'>",oneTag(), name.c_str());
  fprintf(fp,"%s",pcData());
  fprintf(fp,"</%s>\n",oneTag());
  changed = false;
}

/*****************************************************************************/
/* TextProperty CLASS IMPLEMENTATION                                         */
/*****************************************************************************/


/*****************************************************************************/
/* SwitchProperty CLASS IMPLEMENTATION                                       */
/*****************************************************************************/


/*****************************************************************************/
/* LightProperty CLASS IMPLEMENTATION                                       */
/*****************************************************************************/


/*****************************************************************************/
/* NumberProperty CLASS IMPLEMENTATION                                       */
/*****************************************************************************/

/* these two variables are found in the implementation of setNumberVectorProperty(), 
 * when sending <oneNumber> elements. 
 * Strange behaviour ... 
 */

double NumberProperty::lmin = 0;
double NumberProperty::lmax = 0;

void NumberProperty::indiDefProperty()
{
  log->verbose(IFUN,"sending defNumber %s\n",getName());

  if(label != "") {
    fprintf(fp,"\t<defNumber name='%s' label='%s' format='%s' min='%g' max='%g' step='%g'>",
	  name.c_str(), label.c_str(), format.c_str(), min, max, step);
  } else {
    fprintf(fp,"\t<defNumber name='%s' format='%s' min='%g' max='%g' step='%g'>",
	  name.c_str(), format.c_str(), min, max, step);
  }
  fprintf(fp,"%g",value);
  fputs("</defNumber>\n",fp);
  changed = false;
}

/*---------------------------------------------------------------------------*/

void NumberProperty::xmlSave()
{
  if(label != "") {
    fprintf(fp,"\t\t\t<defNumber name='%s' label='%s' format='%s' min='%g' max='%g' step='%g'>",
	  name.c_str(), label.c_str(), format.c_str(), min, max, step);
  } else {
    fprintf(fp,"\t\t\t<defNumber name='%s' format='%s' min='%g' max='%g' step='%g'>",
	  name.c_str(), format.c_str(), min, max, step);
  }
  fprintf(fp,"\n\t\t\t\t%g",value);
  fputs("\n\t\t\t</defNumber>\n",fp);
}

/*---------------------------------------------------------------------------*/

void NumberProperty::indiOneProperty()
{

  log->verbose(IFUN,"sending oneNumber %s\n",getName());

  fprintf(fp,"\t<oneNumber name='%s'",name.c_str());

  /* this min, max handling has been cloned from the original implementation */

  if(lmin != min) {
    fprintf(fp, " min='%g'", min); 
    lmin = min;
  }

  if (lmax != max) {
    fprintf(fp, " max='%g'", max); 
    lmax = max;
  }  

  fprintf(fp,">%g",value);
  fprintf(fp,"</oneNumber>\n");
  changed = false;
}

/*---------------------------------------------------------------------------*/

/*****************************************************************************/
/* BLOBProperty CLASS IMPLEMENTATION                                         */
/*****************************************************************************/

/* unike other defXXX elements,
   this does not contain an initial value for the BLOB */

void BLOBProperty::indiDefProperty()
{
  log->verbose(IFUN,"sending defBLOB %s\n",getName());

  if(label != "")
    fprintf(fp,"\t<defBLOB name='%s' label='%s'>",name.c_str(), label.c_str());
  else
    fprintf(fp,"\t<defBLOB name='%s'>",name.c_str());
  fputs("</defBLOB>\n",fp);
  changed = false;
}

/*---------------------------------------------------------------------------*/

void BLOBProperty::xmlSave()
{
 
  log->verbose(IFUN,"sending oneNumber %s\n",getName());

  if(label != "")
    fprintf(fp,"\t\t\t<defBLOB name='%s' label='%s'>",name.c_str(), label.c_str());
  else
    fprintf(fp,"\t\t\t<defBLOB name='%s'>",name.c_str());
  fputs("\n\t\t\t</defBLOB>\n",fp);
}

/*---------------------------------------------------------------------------*/

void BLOBProperty::indiOneProperty()
{
  fprintf(fp,"\t<oneBLOB name='%s' size='%d' format='%s'>",
	  name.c_str(), blob.size, extension.c_str());

  /* HERE WE MUST PERFORM base64 ENCODING */
  // size should be multiplied x 4/3 and rounded up if not multiple of 3
  
  fprintf(fp,"%s", (const char*)(blob.data));
  fprintf(fp,"</oneBLOB>\n");
  changed = false;
}

/*---------------------------------------------------------------------------*/
