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

#ifndef INDI_HANDLER_H
#define INDI_HANDLER_H

#include <stdio.h>

#include "lilxml.h"
#include "propvector.h"
#include "device.h"


/*
 * once the XML has been parsed OK, the property vector handlers
 * handle all requests to the appropiate device by calling its
 * property update() method.
 * This is the base class with common members.
 */


class PropertyVectorHandler 
{
 public:

  PropertyVectorHandler(const char* logtag);
  virtual ~PropertyVectorHandler();

  /* the only method the INDI Handler calls */
  /* must be overriden and chained */
  virtual bool handle(XMLEle* root, const char* childTag);

protected:

  /* memory reallocation by default, must be overriden and chained  */
  virtual void growMem();

  /* sets the data in the arrays */
  virtual bool setData(XMLEle* child, XMLAtt* attr, int n) {return(false);}

  Log* log;
  Device* device;		/* related device */
  NoLightPropertyVector* propv;	/* related property vector */

  /* the array below may suffer reallocations by bigger requests */

  char** names;			/* array of pointers to decoded switch names */
  int    maxn;			/* max number of items malloc'ed */
  int    len;			/* array length */

 private:

  /* 
   * helper methods that performs thorough checks on 
   * the incoming XML message
   */

  /* check that <newXXXVector> has both 'device' and 'name' attributes */
  bool checkDriverName(XMLEle* root);

  /* check exsitence of oneXXX tag in the XML against the expected tag */
  bool checkTag(XMLEle* elem, const char* expectedTag);

  /* check existence of 'name' attribute and its value */
  bool checkOneName(XMLEle* elem, XMLAtt** attr);
  
};


/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/


/*
 * This class decodes <oneSwitch> properties and call the device/property 
 */

class SwitchVectorHandler : public PropertyVectorHandler 
{

 public:

  SwitchVectorHandler() : PropertyVectorHandler("SwitchVectorHandler"),
    states(0) {}
  ~SwitchVectorHandler() {
    if(states)  delete states;
  }

  /* 
   * customizable methods for this kind of property 
   */

  void growMem();		/* allocates more chunks for the arrays */  
  bool handle(XMLEle* root, const char* childTag);
  bool setData(XMLEle* child, XMLAtt* attr, int n);
  
private:

  /* the array below may suffer reallocations by bigger requests */

  ISState* states;		// array of decoded switch states
  
};

/*---------------------------------------------------------------------------*/

class NumberVectorHandler : public PropertyVectorHandler 
{

 public:

  NumberVectorHandler() : PropertyVectorHandler("NumberVectorHandler"),
    numbers(0) {}
  ~NumberVectorHandler() {
    if(numbers) delete numbers;
  }

  /* 
   * customizable methods for this kind of property 
   */

  void growMem();		/* allocates more chunks for the arrays */  
  bool handle(XMLEle* root, const char* childTag);
  bool setData(XMLEle* child, XMLAtt* attr, int n);
  
private:

  /* the array below may suffer reallocations by bigger requests */

  double* numbers;		// array of decoded numbers
  
};

/*---------------------------------------------------------------------------*/

class TextVectorHandler : public PropertyVectorHandler 
{

 public:

  TextVectorHandler() : PropertyVectorHandler("TextVectorHandler"), texts(0) {}
  ~TextVectorHandler() {
    if(texts) delete texts;
  }

  /* 
   * customizable methods for this kind of property 
   */

  void growMem();		/* allocates more chunks for the arrays */  
  bool handle(XMLEle* root, const char* childTag);
  bool setData(XMLEle* child, XMLAtt* attr, int n);
  
private:

  /* the array below may suffer reallocations by bigger requests */

  char** texts;		// array of pointer to texts
  
};


/*---------------------------------------------------------------------------*/

class BLOBVectorHandler : public PropertyVectorHandler 
{

 public:

  BLOBVectorHandler() : PropertyVectorHandler("BLOBVectorHandler"), blobs(0) {}
  ~BLOBVectorHandler() {
    if(blobs) delete blobs;
  }

  /* 
   * customizable methods for this kind of property 
   */

  void growMem();		/* allocates more chunks for the arrays */  
  bool handle(XMLEle* root, const char* childTag);
  bool setData(XMLEle* child, XMLAtt* attr, int n);
  
private:

  /* the array below may suffer reallocations by bigger requests */

  char** blobs;		// array of pointer to base64 blobs
  
};


/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

/*
 * INDI protocol handler that decode requests from clients
 * and dispatches calls to the appropiate devices
 */


class INDIHandler 
{
 public:
  
  static INDIHandler* instance();
  ~INDIHandler();

  // handle request arriving on file (pipe) descriptor fd
  void handleRequest(int fd);


 private:

  // handle message: true if ok, false otherwise
  bool handle(XMLEle* root);

  bool handleGetProperties(XMLEle* root);

  INDIHandler();

  static INDIHandler* _instance;
  static const int BUFSZ = 1024;

  Log* log;
  char buf[BUFSZ];		// read socket buffer 
  char msg[BUFSZ];		// XML parser error buffer
  LilXML* xmlParser;		// the litte XML parser
  bool protoNotSupported;       // lock against protocol mismatch

  BLOBVectorHandler  blobHandler;
  TextVectorHandler  textHandler;
  NumberVectorHandler numberHandler;
  SwitchVectorHandler switchHandler;

};

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

#endif
