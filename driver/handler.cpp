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

#include "indicom.h"
#include "handler.h"
#include "driver.h"
#include "property.h"
#include "logfact.h"

/*---------------------------------------------------------------------------*/

PropertyVectorHandler::PropertyVectorHandler(const char* tag) : 
  log(0), device(0), propv(0), names(0), maxn(0), len(0) 
{ 
  log = LogFactory::instance()->forClass(tag);
}

/*---------------------------------------------------------------------------*/

PropertyVectorHandler::~PropertyVectorHandler()
{
  if(names) delete names;
  delete log;
}

/*---------------------------------------------------------------------------*/

bool PropertyVectorHandler::checkDriverName(XMLEle* root)
{

  XMLAtt* attr;
  char* devName;
  char* propvName;

  // check for existance of 'device' attributes and its value

  log->verbose(IFUN,"checking for existance of 'device' attribute\n");
  attr = findXMLAtt (root, "device");
  if (!attr) {
    log->error(IFUN,"%s missing 'device' attribute\n", tagXMLEle(root));
    return (false);
  }
  devName = valuXMLAtt(attr);

  // unregistered devices are silently ignored
  device = DYNAMIC_CAST(Device*, Driver::instance()->find(devName));
  if(!device) {
    log->verbose(IFUN,"ignoring %s \n",devName);
    return(false);
  }

  /* check for existance of 'name' attribute and 
   * registered property vector within device 
   */

  log->verbose(IFUN,"checking for existance of 'name' attribute\n");
  attr = findXMLAtt (root, "name");
  if (!attr) {
    Driver::instance()->formatMsg("Falta el atributo device en %s",
		      tagXMLEle(root));
    Driver::instance()->indiMessage();
    return (false);
  }

  propvName = valuXMLAtt(attr); 
  
  log->verbose(IFUN,"checking registered property vector in device\n");
  propv = DYNAMIC_CAST(NoLightPropertyVector*, device->find(propvName));
  if(!propv) {
    device->formatMsg("%s: Error: No se ha encontrado propiedad -> %s",
		      device->getName(), propvName);
    device->indiMessage();
    return(false);
  }

  /*
   * check for write permissions 
   */

  log->verbose(IFUN,"checking write permissions\n");
  if(!propv->checkWrite()) {
    device->formatMsg("%s: Propiedad %s es de solo lectura",
		      device->getName(), propvName);
    device->indiMessage();
    return(false);
  }

  return (true);
}


/*---------------------------------------------------------------------------*/

bool PropertyVectorHandler::checkTag(XMLEle* elem, const char* expectedTag)
{
  char* tag = tagXMLEle(elem);

  log->verbose(IFUN,"checking tag %s against expected tag %s\n",tag,expectedTag);

  if (strcmp(tag, expectedTag) != 0) {
    propv->formatMsg("%s: Error: Esperando tag %s pero recibido %s",
		      propv->getName(), expectedTag, tag);
    propv->forceChange();
    propv->indiSetProperty();
    return(false);
  }

  return(true);
}

/*---------------------------------------------------------------------------*/

bool PropertyVectorHandler::checkOneName(XMLEle* elem, XMLAtt** attr)
{
  char* key;
  char* tag = tagXMLEle(elem);
  
  *attr = findXMLAtt(elem, "name");

  log->verbose(IFUN,"%s: checking 'name' attribute in %s\n", propv->getName(),tag);

  // expects a 'name' attribute

  if(!(*attr)) {
    propv->formatMsg("%s: Error: esperaba atributo name pare propiedad %s",
		      propv->getName(),tag);
    propv->forceChange();
    propv->indiSetProperty();
    
    return(false);
  }

  // expects name's value included in the property vector

  log->verbose(IFUN,"%s: checking value in 'name' attribute of %s\n", 
	    propv->getName(), tag);
  key = valuXMLAtt(*attr);
  if(propv->find(key) == 0) {

    propv->formatMsg("%s: Error: no existe la propiedad -> %s\n",
		      propv->getName(),key);
    propv->forceChange();
    propv->indiSetProperty();
    return(false);
  }

  return(true);
}

/*---------------------------------------------------------------------------*/

void PropertyVectorHandler::growMem()
{
  int newsz;

  maxn += 2;		

  log->verbose(IFUN,"expands to %d more names\n",maxn); 

  newsz = maxn * sizeof(char *);
  names = names ? STATIC_CAST(char**, realloc(names, newsz))
    : STATIC_CAST(char **, malloc (newsz));
}

/*---------------------------------------------------------------------------*/

bool PropertyVectorHandler::handle(XMLEle* root, const char* childTag)
{
  int n;

  XMLEle* child;
  XMLAtt* attr;
  char* tag;

  // check existence of 'device' and 'name' attributes in the
  // <newXXXVector> tag
  // also check for RW/WO permissions 
  // unregistered devices are silently ignored


  if(!checkDriverName(root))
    return(false);

  // iterates over the <oneXXX> elements

  len=0;
  for(n=0, child=nextXMLEle(root,1); child ; child=nextXMLEle(root,0), n++) {

    tag = tagXMLEle(child);
   
    // checks the incoming <oneXXX> tag against the expected child tag 

    if(!checkTag(child, childTag)) {
      log->error(IFUN,"%s: failed to find tag %s\n", propv->getName(), childTag);
      return(false);
    }
    
    // check the 'name' attribute in <oneXXX> elements
    // its value must also exists in the registered property vector

    if(!checkOneName(child, &attr)) {
      log->error(IFUN,"%s: failed to find a property member or name\n",
		propv->getName());
      return(false);
    }

    // grows memory allocation chunk if necessary

    if (n >= maxn)
      growMem();

    // check data boundaries and set values in the arrays

    if(!setData(child, attr, n)) {
      log->error(IFUN,"%s: error checking data value or boundaries\n", propv->getName());
      return(false);
    }
  } // for

  len = n;
  return(true);
}


/*---------------------------------------------------------------------------*/

void SwitchVectorHandler::growMem()
{
  int newsz;

  PropertyVectorHandler::growMem();

  log->verbose(IFUN,"growMem() expands to %d more switches\n",maxn); 

  newsz = maxn * sizeof(ISState);
  states = states ? STATIC_CAST(ISState* , realloc(states, newsz))
    : STATIC_CAST(ISState*, malloc (newsz));

}

/*---------------------------------------------------------------------------*/

void NumberVectorHandler::growMem()
{
  int newsz;

  PropertyVectorHandler::growMem();

  log->verbose(IFUN,"growMem() expands to %d more numbers\n",maxn); 

  newsz = maxn * sizeof(double);
  numbers = numbers ? STATIC_CAST(double* , realloc(numbers, newsz))
    : STATIC_CAST(double*, malloc (newsz));

}

/*---------------------------------------------------------------------------*/

void TextVectorHandler::growMem()
{
  int newsz;

  PropertyVectorHandler::growMem();

  log->verbose(IFUN,"growMem() expands to %d more texts\n",maxn); 

  newsz = maxn * sizeof(char**);
  texts = texts ? STATIC_CAST(char** , realloc(texts, newsz))
    : STATIC_CAST(char**, malloc (newsz));

}

/*---------------------------------------------------------------------------*/

void BLOBVectorHandler::growMem()
{
  int newsz;

  PropertyVectorHandler::growMem();

  log->verbose(IFUN,"growMem() expands to %d more blobs\n",maxn); 

  newsz = maxn * sizeof(char**);
  blobs = blobs ? STATIC_CAST(char** , realloc(blobs, newsz))
    : STATIC_CAST(char**, malloc (newsz));

}

/*---------------------------------------------------------------------------*/

bool SwitchVectorHandler::setData(XMLEle* elem, XMLAtt* attr, int n)
{
  char* key = valuXMLAtt(attr);
  char* datum = pcdataXMLEle(elem);
  bool ok;

  states[n] = sState4Str(datum, &ok);
  if(!ok) {
    propv->formatMsg("%s Error: el valor %s deberia ser On o Off\n",
		     propv->getName(),datum);
    propv->forceChange();
    propv->indiSetProperty();
    return(false);
  }

  log->verbose(IFUN,"setting switch[%d] (%s) = %d\n",n,key,states[n]); 

  names [n] = key;
  return(true);
}

/*---------------------------------------------------------------------------*/

bool NumberVectorHandler::setData(XMLEle* elem, XMLAtt* attr, int n)
{
  char* datum;
  char* tag;
  char* key;

  tag = tagXMLEle(elem);
  key = valuXMLAtt(attr);
  datum = pcdataXMLEle(elem);

  log->verbose(IFUN,"setting number[%d] (%s)\n",n,key); 

  if (f_scansexa (datum, &numbers[n]) < 0) {
    propv->formatMsg("%s Error de formato en %s",propv->getName(),datum);
    propv->forceChange();
    propv->indiSetProperty();
    return(false);
  }

  log->verbose(IFUN,"PCDATA was %s and f_scansexa converted to number[%d] = %g\n",datum,n,numbers[n]);
  log->verbose(IFUN,"finding individual property %s for %s\n",key,propv->getName()); 

  NumberProperty* np = DYNAMIC_CAST(NumberProperty*, propv->find(key));
  
  if(numbers[n] < np->getMin()) {
    propv->formatMsg("%s Error: el valor minimo para %s es %g",
		     propv->getName(),key, np->getMin());
    propv->forceChange();
    propv->indiSetProperty();
    return(false);
  }

  if(numbers[n] > np->getMax()) {
    propv->formatMsg("%s Error: el valor maximo para %s es %g",
		     propv->getName(),key, np->getMax());
    propv->forceChange();
    propv->indiSetProperty();
    return(false);
  }

  names[n] = key;
  return(true);
}

/*---------------------------------------------------------------------------*/

bool TextVectorHandler::setData(XMLEle* elem, XMLAtt* attr, int n)
{
  char* key = valuXMLAtt(attr);
  log->verbose(IFUN,"setting %text[%d] (%s)\n",n,key); 
  names[n] = key;
  texts[n] = pcdataXMLEle(elem);
  return(true);
}

/*---------------------------------------------------------------------------*/

bool BLOBVectorHandler::setData(XMLEle* elem, XMLAtt* attr, int n)
{
  char* key = valuXMLAtt(attr);
  log->verbose(IFUN,"setting blob[%d] (%s)\n",n,key);
  names[n] = key; 
  blobs[n] = pcdataXMLEle(elem); // it is still base64 encoded
  return(true);
}

/*---------------------------------------------------------------------------*/

bool SwitchVectorHandler::handle(XMLEle* root, const char* childTag)
{

  bool ok;

  log->verbose(IFUN,"handling %s request\n", childTag);
  if(!PropertyVectorHandler::handle(root, childTag)) 
    return(false);

  // only one switch is allowed in setSwitchVector

  if(len != 1) {
    propv->formatMsg("%s Error: solo se permite cambiar 1 switch y no %d",
		     propv->getName(), len);
    propv->forceChange(); 
    propv->indiSetProperty();
    return(false);
  }

  // checks for Switch rules

  SwitchPropertyVector* pv = DYNAMIC_CAST(SwitchPropertyVector*,propv);
  SwitchProperty* sw = DYNAMIC_CAST(SwitchProperty*, propv->find(names[0]));
  ok = pv->checkRule(sw, states[0]);
  if(!ok) {
    
    propv->formatMsg("%s Error: regla %s forzada",
		     propv->getName(), rule2Str(pv->getRule()));
    propv->forceChange(); 
    propv->indiSetProperty();
    return(false);
  }

  device->update(DYNAMIC_CAST(SwitchPropertyVector*,propv),names[0],states[0]);
  return(true);
}

/*---------------------------------------------------------------------------*/

bool NumberVectorHandler::handle(XMLEle* root, const char* childTag)
{
  
  log->verbose(IFUN,"handling %s request\n", childTag);
  if(!PropertyVectorHandler::handle(root, childTag)) 
    return(false);

  device->update(DYNAMIC_CAST(NumberPropertyVector*,propv),names,numbers,len);
  return(true);
}

/*---------------------------------------------------------------------------*/

bool TextVectorHandler::handle(XMLEle* root, const char* childTag)
{
  
  log->verbose(IFUN,"handling %s request\n", childTag);
  if(!PropertyVectorHandler::handle(root, childTag)) 
    return(false);

  device->update(DYNAMIC_CAST(TextPropertyVector*,propv),names,texts,len);
  return(true);
}

/*---------------------------------------------------------------------------*/

bool BLOBVectorHandler::handle(XMLEle* root, const char* childTag)
{

  log->verbose(IFUN,"handling %s request\n", childTag);
  if(!PropertyVectorHandler::handle(root, childTag)) 
    return(false);

  device->update(DYNAMIC_CAST(BLOBPropertyVector*,propv),names,blobs,len);
  return(true);
}


/*---------------------------------------------------------------------------*/

// the singleton pattern

INDIHandler* INDIHandler::_instance = 0;

INDIHandler* INDIHandler::instance()
{
  if(!_instance)
    _instance = new INDIHandler();
  return(_instance);
}


/*---------------------------------------------------------------------------*/

INDIHandler::INDIHandler() :
  log(0), protoNotSupported(false)
{
  xmlParser = newLilXML(); 
  log = LogFactory::instance()->forClass("INDIHandler");

}

/*---------------------------------------------------------------------------*/

INDIHandler::~INDIHandler()
{
  if(_instance) {
    _instance = 0;
    delLilXML(xmlParser);
    delete log;
  } 
}

/*---------------------------------------------------------------------------*/

void INDIHandler::handleRequest(int fd)
{
  char* bp;
  int nread;
  
  log->info(IFUN,"INDIHandler: new request arrived\n");
  
  /* one read */

  nread = read (fd, buf, sizeof(buf));
  if (nread < 0) {
    log->error(IFUN,"INDIHandler: Fatal error. Details follows. Bye ...\n");
    log->error(IFUN,"%s\n", strerror(errno));
    exit(1);
  }

  if (nread == 0) {
    log->error(IFUN,"INDIHandler: Fatal error. Pipe closed. Bye ...\n");
    exit(1);
  }
  
  if(protoNotSupported) {
    Driver::instance()->formatMsg("peticion INDI ignorada");
    Driver::instance()->indiMessage();
    return;
  }
  
  for (bp = buf; nread-- > 0; bp++) {
    XMLEle *root = readXMLEle (xmlParser, *bp, msg);
    if (root) {
      if (!handle (root)) {
	log->error(IFUN,"INDIHandler: XML validation failed\n");
      }
      delXMLEle (root);
    } else if (msg[0]) {
      log->error(IFUN,"INDIHandler: malformed XML message: %s\n", msg);
    }
  }  
  log->flush();
}


/*---------------------------------------------------------------------------*/

bool INDIHandler::handle(XMLEle* root)
{
  char* tag;
  bool result = true;

  tag = tagXMLEle(root);

  log->debug(IFUN,"INDIHandler: dispatching tag %s\n",tag);

  if(!strcmp(tag, "newNumberVector")) {
    result &= numberHandler.handle(root,"oneNumber");

  } else if(!strcmp(tag, "newSwitchVector")) {
    result &= switchHandler.handle(root,"oneSwitch");

  } else if(!strcmp(tag, "newTextVector")) {
    result &= textHandler.handle(root,"oneText");

  } else if(!strcmp(tag, "newBLOBVector")) {
    result &= blobHandler.handle(root,"oneBLOB");

  } else if(!strcmp(tag, "getProperties")) {
    result &= handleGetProperties(root);

  } else {
    Driver::instance()->formatMsg("Mensaje XML con marca <%s> desconocida",
				  tag);
    Driver::instance()->indiMessage();
    result = false;
  }

 const char* resStr = (result) ? "ok" : "not ok" ;
 log->info(IFUN,"<%s> dispatched %s.\n",tag, resStr);
 return (result);    
}


/*---------------------------------------------------------------------------*/

bool INDIHandler::handleGetProperties(XMLEle* root)
{

  Device* device;
  XMLAtt* attr;
  double ver;

  log->info(IFUN,"request to send properties to a client\n");

  // check for existance of 'device' attributes and its value

  attr = findXMLAtt (root, "version");
  if (!attr) {
    Driver::instance()->formatMsg("Error: falta atributo version en %s",
				  tagXMLEle(root));
    Driver::instance()->indiMessage();
    protoNotSupported = true;
    return(false);
  }

  ver = atof (valuXMLAtt(attr));
  if (ver > INDIV) {
    Driver::instance()->formatMsg("Error: version de cliente %g > servidor %g",
				  ver,INDIV);
    Driver::instance()->indiMessage();
    protoNotSupported = true;
    return(false);
  }

  attr = findXMLAtt (root, "device");
  if(attr) {
    device = DYNAMIC_CAST(Device*, Driver::instance()->find(valuXMLAtt(attr)));
    log->info(IFUN,"sending to client all properties from device %s\n",
	     device->getName());
    device->indiDefProperty();
  } else {
    log->info(IFUN,"sending to client all properties from all devices\n");
    Driver::instance()->indiDefProperty();
  }

  return(true);
}
