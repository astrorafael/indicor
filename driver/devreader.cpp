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


#include "devreader.h"
#include "indicom.h"
#include "logfact.h"

/***********************************/
/* Device XML file definitons tags */
/***********************************/

const char* DeviceReader::DEF_LIGHT  = "defLight";
const char* DeviceReader::DEF_SWITCH = "defSwitch";
const char* DeviceReader::DEF_TEXT   = "defText";
const char* DeviceReader::DEF_NUMBER = "defNumber";
const char* DeviceReader::DEF_BLOB   = "defBLOB";
const char* DeviceReader::DEF_LIGHT_VECTOR = "defLightVector";
const char* DeviceReader::DEF_SWITCH_VECTOR= "defSwitchVector";
const char* DeviceReader::DEF_TEXT_VECTOR  = "defTextVector";
const char* DeviceReader::DEF_NUMBER_VECTOR= "defNumberVector";
const char* DeviceReader::DEF_BLOB_VECTOR  = "defBLOBVector";
const char* DeviceReader::DEF_DEVICE       = "defDevice";


/*---------------------------------------------------------------------------*/

DeviceReader::DeviceReader(INDIItem* root)
  : log(0), lp(0), driver(root) 
{ 
  lp = newLilXML(); 
  log = LogFactory::instance()->forClass("DeviceReader");
}

/*---------------------------------------------------------------------------*/

DeviceReader::~DeviceReader()
{
  delLilXML(lp);
  delete log;
}

/*---------------------------------------------------------------------------*/

/* Reads name and optional label
 * for any element in the INDI properties file
 */

bool DeviceReader::readNameLabel(XMLEle* elem, char** name, char** label)
{
  XMLAtt *nameAtt, *labelAtt;

  /* GENERIC FOR ALL PROPERTY VECTORS */
  
  nameAtt = findXMLAtt(elem, "name");
  if(!nameAtt) {
    log->error(IFUN,"missing 'name' attribute for tag %s\n", tagXMLEle(elem));
    return(false);
  }
	
  *name = valuXMLAtt(nameAtt);
  
  labelAtt = findXMLAtt(elem, "label");
  if(labelAtt) {
    *label = valuXMLAtt(labelAtt);
  } else {
    *label = 0;
  }
  return(true);
}

/*---------------------------------------------------------------------------*/

bool DeviceReader::readText(XMLEle* elem, INDIItem* parent)
{
  char *name1, *label1, *value1;
  TextProperty* item = 0;
  bool result = true;


  if( ! readNameLabel(elem, &name1, &label1)) {
    result = false;
    goto epilog;		// purists been dammned !
  }

  value1 = pcdataXMLEle(elem);

  if(!value1) {
    log->error(IFUN,"<defText>: missing PCDATA for name %s child of %s\n", 
	      name1, parent->getName()) ;
    result = false;
    goto epilog;		// purists been dammned !
  }

  strTrimRight(value1, value1);	// IN-PLACE TRIM !!!
  
  item = new TextProperty(name1, value1, parent);
  

  if(label1)
    item->setLabel(label1);

 epilog:

  const char* resStr = (result) ? "ok" : "not ok" ;
  log->info(IFUN,"<%s> %s decoding %s.\n",tagXMLEle(elem), item->getName(),resStr);
  return(result);
}

/*---------------------------------------------------------------------------*/

bool DeviceReader::readSwitch(XMLEle* elem, INDIItem* parent)
{
  char *name1, *label1, *value1;
  bool result = true;
  SwitchProperty* item = 0;


  if( ! readNameLabel(elem, &name1, &label1)) {
    result = false;
    goto epilog;		// purists been dammned !
  }

  value1 = pcdataXMLEle(elem);
  if(!value1) {
    log->error(IFUN,"<defSwitch>: missing PCDATA for name %s child of %s\n", 
	      name1, parent->getName()) ;
    result = false;
    goto epilog;		// purists been dammned !
  }

  // should trim trailing blanks before passing to sState4Str()

  item = new SwitchProperty(name1, sState4Str(value1,&result), parent);
  

  if(label1)
    item->setLabel(label1);

 epilog:

  const char* resStr = (result) ? "ok" : "not ok" ;
  log->info(IFUN,"<%s> %s decoding %s.\n",tagXMLEle(elem), item->getName(),resStr);
  return(result);
 }

/*---------------------------------------------------------------------------*/

bool DeviceReader::readLight(XMLEle* elem, INDIItem* parent)
{
  char *name1, *label1, *value1;
  bool result = true;
  LightProperty* item = 0;


  if( ! readNameLabel(elem, &name1, &label1)) {
    result = false;
    goto epilog;		// purists been dammned !
  }

  value1 = pcdataXMLEle(elem);
  if(!value1) {
    log->error(IFUN,"<defLight>: missing PCDATA for name %s child of %s\n", 
	      name1, parent->getName()) ;
    result = false;
    goto epilog;		// purists been dammned !
  }

  item = new LightProperty(name1, pState4Str(value1,&result), parent);
  

  if(label1)
    item->setLabel(label1);

 epilog:

  const char* resStr = (result) ? "ok" : "not ok" ;
  log->info(IFUN,"<%s> %s decoding %s.\n",tagXMLEle(elem), item->getName(),resStr);
  return(result);
 }

/*---------------------------------------------------------------------------*/

bool DeviceReader::readNumber(XMLEle* elem, INDIItem* parent)
{
  char *name1, *label1, *value1,  *format1;
  XMLAtt* attr;
  double dvalue;
  bool result = true;

  NumberProperty* item = 0;


  if( ! readNameLabel(elem, &name1, &label1)) {
    result = false;
    goto epilog;		// purists been dammned !
  }

  value1 = pcdataXMLEle(elem);
  if(!value1) {
    log->error(IFUN,"<defNumber>: missing PCDATA for name %s child of %s\n", 
	      name1, parent->getName()) ;
    result = false;
    goto epilog;		// purists been dammned !
  }

  if(f_scansexa(value1,  &dvalue) < 0) {
    log->error(IFUN,"<defNumber>: %s PCDATA is not a number",name1) ;
    result = false;
    goto epilog;		// purists been dammned !
  }


  attr = findXMLAtt(elem, "format");
  if(!attr) {
    log->error(IFUN,"<defNumber>: %s missing 'format' attribute", name1) ;
    result = false;
    goto epilog;		// purists been dammned !
  }  
  format1 = valuXMLAtt(attr);


  item = new NumberProperty(name1, dvalue, format1, parent);
  

  if(label1)
    item->setLabel(label1);

  // handles optional min attribute

  attr = findXMLAtt(elem, "min");
  if(attr) {
    name1 = valuXMLAtt(attr);
    if(sscanf(name1, "%lf", &dvalue) >= 0) 
      item->setMin(dvalue);
    else
      log->warn(IFUN,"<defNumber>: %s 'min' attribute: error converting to double\n",
	       item->getName());
  }  

  // handles optional max attribute

  attr = findXMLAtt(elem, "max");
  if(attr) {
    name1 = valuXMLAtt(attr);
    if( sscanf(name1, "%lf", &dvalue) >= 0) 
      item->setMax(dvalue);
    else
      log->warn(IFUN,"<defNumber>: %s 'max' attribute: error converting to double\n",
	       item->getName());
  }  

  // handles optional step attribute

  attr = findXMLAtt(elem, "step");
  if(attr) {
    name1 = valuXMLAtt(attr);
    if(sscanf(name1, "%lf", &dvalue) >= 0) 
      item->setStep(dvalue);
    else
      log->warn(IFUN,"<defNumber>: %s 'step' attribute: error convertng to double\n",
	       item->getName());
  }  

 epilog:

  const char* resStr = (result) ? "ok" : "not ok" ;
  log->info(IFUN,"<%s> %s decoding %s.\n",tagXMLEle(elem), item->getName(),resStr);
  return(result);
 }

/*---------------------------------------------------------------------------*/

bool DeviceReader::readBLOB(XMLEle* elem, INDIItem* parent)
{
  char *name1, *label1;
  bool result = true;
  BLOBProperty* item = 0;


  if( ! readNameLabel(elem, &name1, &label1)) {
    result = false;
    goto epilog;		// purists been dammned !
  }

  item = new BLOBProperty(name1, parent);
  

  if(label1)
    item->setLabel(label1);

 epilog:

  const char* resStr = (result) ? "ok" : "not ok" ;
  log->info(IFUN,"<%s> %s decoding %s.\n",tagXMLEle(elem), item->getName(),resStr);
  return(result);
 }

/*---------------------------------------------------------------------------*/

bool DeviceReader::readAttrsPV(XMLEle* elem, PropertyVector* propv)
{
  XMLAtt* attr;
  bool result = true;

  // handles group optional attribute 

  attr = findXMLAtt(elem, "group");
  if(attr) {
      propv->setGroup(valuXMLAtt(attr));
  }  

  // handles state optional attribute

  attr = findXMLAtt(elem, "state");
  if(attr) {
    propv->setState(pState4Str(valuXMLAtt(attr),&result));
  } 

  return(result);
}

/*---------------------------------------------------------------------------*/

bool DeviceReader::readAttrsNLPV(XMLEle* elem, NoLightPropertyVector* propv)
{
  XMLAtt* attr;
  char* name1;
  double dvalue;
  bool result = true;
 

  // handles generic attributes

  result &= readAttrsPV(elem, propv);

  // handles group optional attribute 

  attr = findXMLAtt(elem, "perm");
  if(attr) {
    propv->setPerm(perm4Str(valuXMLAtt(attr),&result));
  }  

  // handles state optional attribute

  attr = findXMLAtt(elem, "timeout");
  if(attr) {
    name1 = valuXMLAtt(attr);
    if(sscanf(name1, "%lf", &dvalue) >= 0) 
      propv->setTimeout(dvalue);
    else {
      log->warn(IFUN,"<%s>: 'timeout' attribute: error converting to double\n",
	       tagXMLEle(elem));
      result = false;
    }
  }  
  return(result);
}

/*---------------------------------------------------------------------------*/

bool DeviceReader::readLightVector(XMLEle* elem, INDIItem* parent)
{
 
  char *name1, *label1;
  XMLEle* child;
  bool result = true;
  char* tag;
  LightPropertyVector* item = 0;


  if( ! readNameLabel(elem, &name1, &label1)) {
    result = false;
    goto epilog;		// purists been dammned !
  }

  item = new LightPropertyVector(name1, parent);
  

  if(label1)
    item->setLabel(label1);

  if(!readAttrsPV(elem, item)) {
    result = false;
    goto epilog;		// purists been dammned !
  }

  for (child = nextXMLEle(elem,1); child != NULL; child = nextXMLEle(elem,0)) {
    tag = tagXMLEle(child);
    log->verbose(IFUN,"tag is %s\n", tag);
    if(!strcmp(tag, DEF_LIGHT))
      result &= readLight(child, item);
    else {
      log->error(IFUN,"%s tag expected\n", DEF_LIGHT);
      result = false;
    }
    if(!result)	
      break;
  }

 epilog:

  const char* resStr = (result) ? "ok" : "not ok" ;
  log->info(IFUN,"<%s> %s decoding %s.\n",tagXMLEle(elem), item->getName(),resStr);
  return(result);
    
}

/*---------------------------------------------------------------------------*/

bool DeviceReader::readTextVector(XMLEle* elem, INDIItem* parent)
{
 
  char *name1, *label1;
  XMLEle* child;
  bool result = true;
  char* tag;
  TextPropertyVector* item = 0;


  if( ! readNameLabel(elem, &name1, &label1)) {
    result = false;
    goto epilog;		// purists been dammned !
  }

  item = new TextPropertyVector(name1, parent);
  

  if(label1)
    item->setLabel(label1);

  if(!readAttrsNLPV(elem, item)) {
    result = false;
    goto epilog;		// purists been dammned !
  }

  for (child = nextXMLEle(elem,1); child != NULL; child = nextXMLEle(elem,0)) {
    tag = tagXMLEle(child);
    log->verbose(IFUN,"tag is %s\n", tag);
    if(!strcmp(tag, DEF_TEXT))
      result &= readText(child, item);
    else {
      log->error(IFUN,"%s tag expected\n", DEF_TEXT);
      result = false;
    }
    if(!result)	
      break;
  }

 epilog:

  const char* resStr = (result) ? "ok" : "not ok" ;
  log->info(IFUN,"<%s> %s decoding %s.\n",tagXMLEle(elem), item->getName(),resStr);
  return(result);
    
}

/*---------------------------------------------------------------------------*/

bool DeviceReader::readRule(XMLEle* elem, ISRule* rule)
{
  XMLAtt *attr;
  bool okFlag;


  attr = findXMLAtt(elem, "rule");
  if(!attr) {
    log->error(IFUN,"missing 'rule' attribute for tag %s\n", tagXMLEle(elem));
    return(false);
  }
	
  *rule = rule4Str(valuXMLAtt(attr), &okFlag);
  
  return(okFlag);
}


/*---------------------------------------------------------------------------*/

bool DeviceReader::readSwitchVector(XMLEle* elem, INDIItem* parent)
{
 
  char *name1, *label1;
  XMLEle* child;
  bool result = true;
  char* tag;
  SwitchPropertyVector* item = 0;
  ISRule rule;


  if( ! readNameLabel(elem, &name1, &label1)) {
    result = false;;
    goto epilog;		// purists been dammned !
  }

  item = new SwitchPropertyVector(name1, parent);
  

  if(label1)
    item->setLabel(label1);

  if(!readAttrsNLPV(elem, item)) {
    result = false;
    goto epilog;		// purists been dammned !
  }

  if(!readRule(elem, &rule)) {
    result = false;
    goto epilog;		// purists been dammned !
  }
  
  item->setRule(rule);
  
  for (child = nextXMLEle(elem,1); child != NULL; child = nextXMLEle(elem,0)) {
    tag = tagXMLEle(child);
    log->verbose(IFUN,"tag is %s\n", tag);
    if(!strcmp(tag, DEF_SWITCH))
      result &= readSwitch(child, item);
    else {
      log->error(IFUN,"%s tag expected\n", DEF_SWITCH);
      result = false;
    }
    if(!result)	
      break;
  }

  item->updateLastOn();		// find last 'On' switch and cache it

 epilog:

  const char* resStr = (result) ? "ok" : "not ok" ;
  log->info(IFUN,"<%s> %s decoding %s.\n",tagXMLEle(elem), item->getName(),resStr);
  return(result);
}

/*---------------------------------------------------------------------------*/

bool DeviceReader::readNumberVector(XMLEle* elem, INDIItem* parent)
{
 
  char *name1, *label1;
  XMLEle* child;
  bool result = true;
  char* tag;
  NumberPropertyVector* item = 0;


  if( ! readNameLabel(elem, &name1, &label1)) {
    result = false;
    goto epilog;		// purists been dammned !
  }

  item = new NumberPropertyVector(name1, parent);
  

  if(label1)
    item->setLabel(label1);
  
  if(!readAttrsNLPV(elem, item)) {
    result = false;
    goto epilog;		// purists been dammned !
  }

  for (child = nextXMLEle(elem,1); child != NULL; child = nextXMLEle(elem,0)) {
    tag = tagXMLEle(child);
    log->verbose(IFUN,"tag is %s\n", tag);
    if(!strcmp(tag, DEF_NUMBER))
      result &= readNumber(child, item);
    else {
      log->error(IFUN,"%s tag expected\n", DEF_NUMBER);
      result = false;
    }
    if(!result)	
      break;
  }

 epilog:

  const char* resStr = (result) ? "ok" : "not ok" ;
  log->info(IFUN,"<%s> %s decoding %s.\n",tagXMLEle(elem), item->getName(),resStr);
  return(result);    
}

/*---------------------------------------------------------------------------*/

bool DeviceReader::readBLOBVector(XMLEle* elem, INDIItem* parent)
{
 
  char *name1, *label1;
  XMLEle* child;
  bool result = true;
  char* tag;

  BLOBPropertyVector* item = 0;


  if( ! readNameLabel(elem, &name1, &label1)) {
    result = false;
    goto epilog;		// purists been dammned !
  }

  item = new BLOBPropertyVector(name1, parent);
  

  if(label1)
    item->setLabel(label1);
  
  if(!readAttrsNLPV(elem, item)) {
    result = false;
    goto epilog;		// purists been dammned !
  }

  for (child = nextXMLEle(elem,1); child != NULL; child = nextXMLEle(elem,0)) {
    tag = tagXMLEle(child);
    log->verbose(IFUN,"tag is %s\n", tag);
    if(!strcmp(tag, DEF_BLOB))
      result &= readBLOB(child, item);
    else {
      log->error(IFUN,"%s tag expected\n", DEF_BLOB);
      result = false;
    }
    if(!result)	
      break;
  }

 epilog:

  const char* resStr = (result) ? "ok" : "not ok" ;
  log->info(IFUN,"<%s> %s decoding %s.\n",tagXMLEle(elem), item->getName(),resStr);
  return(result);    
}

/*---------------------------------------------------------------------------*/

Device* 
DeviceReader::readDevice(XMLEle* elem, const char* path)
{
 
  char *name1, *label1;
  XMLEle* child;
  Device* item = 0;
  bool result = true;
  char* tag;


  if( ! readNameLabel(elem, &name1, &label1)) {
    result = false;
    goto epilog;		// purists been dammned !
  }

  item = new Device(name1, path, driver);
  

  if(label1)
    item->setLabel(label1);

  // read children of Device
  
  for (child = nextXMLEle(elem,1); child != NULL; child = nextXMLEle(elem,0)) {
    
    tag = tagXMLEle(child);
    log->verbose(IFUN,"tag is %s\n", tag);

    if(!strcmp(tag, DEF_LIGHT_VECTOR)) {
      log->verbose(IFUN,"reading a LightPropertyVector\n");
      result &= readLightVector(child, item);

    } else if(!strcmp(tag, DEF_TEXT_VECTOR)) {
      log->verbose(IFUN,"reading a TextPropertyVector\n");
      result &= readTextVector(child, item);

    } else if(!strcmp(tag, DEF_SWITCH_VECTOR)) {
      log->verbose(IFUN,"reading a SwitchPropertyVector\n");
      result &= readSwitchVector(child, item);

    } else if(!strcmp(tag, DEF_NUMBER_VECTOR)) {
      log->verbose(IFUN,"reading a NumberPropertyVector\n");
      result &= readNumberVector(child, item);

    } else if(!strcmp(tag, DEF_BLOB_VECTOR)) {
      log->verbose(IFUN,"reading a BLOBPropertyVector\n");
      result &= readBLOBVector(child, item);

    } else {
      log->error(IFUN,"tag %s unknown or unexpected at this level\n", tag);
      result = false;
    }
    if(!result)	
	break;
  }

 epilog:

  const char* resStr = (result) ? "ok" : "not ok" ;
  log->info(IFUN,"<%s> %s decoding %s.\n",tagXMLEle(elem), item->getName(),resStr);
  
  if(!result && item != 0) {
    delete item;
    item = 0;
  }

  return (item);    
}



/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/


Device* 
DeviceReader::read(const char* basedir, const char* path)
{

  XMLEle *root = 0;
  FILE* fp = 0;
  char errmsg[1024];
  Device* res = 0;		// assume bad by default

  xmlFile = basedir;
  xmlFile.append("/");
  xmlFile.append(path);

  fp = fopen(xmlFile.c_str(), "r");  
  if(!fp) {
    log->error(IFUN,"file %s open error\n", xmlFile.c_str());
    return(0);
  }

  driver = Driver::instance();	// should be already created

  root = readXMLFile (fp, lp, errmsg);

  if (root) {
    log->info(IFUN,"decoding the XML file %s\n",path);
    if(!strcmp(tagXMLEle(root), DEF_DEVICE)) {
      res = readDevice(root, path); 
    }
    else {
      log->error(IFUN,"%s tag expected\n", DEF_DEVICE);
    }
    delXMLEle(root);		// deletes the DOM tree
  
  } else if (errmsg[0]) {
    log->error(IFUN,"error reading the XML file: %s\n",errmsg);
  }
  
  log->flush();
  fclose(fp);
    
  return(res);
}
