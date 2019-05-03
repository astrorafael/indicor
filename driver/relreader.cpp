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


#include "relreader.h"
#include "logfact.h"

/***********************************/
/* Device XML file definitons tags */
/***********************************/

const char* RelationshipsReader::ROOT       = "relationships";
const char* RelationshipsReader::OBSERVER   = "observer";
const char* RelationshipsReader::OBSERVABLE = "observable";
const char* RelationshipsReader::PROPERTY   = "property";

/*---------------------------------------------------------------------------*/

RelationshipsReader::RelationshipsReader(INDIItem* root)
  : log(0), lp(0), driver(root) 
{ 
  lp = newLilXML(); 
  log = LogFactory::instance()->forClass("RelationshipsReader");
}


/*---------------------------------------------------------------------------*/

RelationshipsReader::~RelationshipsReader()
{
  delLilXML(lp);
  delete log;
}

/*---------------------------------------------------------------------------*/

bool 
RelationshipsReader::readProperty(XMLEle* elem, Device* observer, Device* observable)
{
  XMLAtt *topicAtt;
  char* tag;
  char* value = "unknown";
  char* topicStr = "unknown";
  ITopic topic;
  bool  result = true;
  PropertyVector* pv;

  tag   = tagXMLEle(elem);
  log->verbose(IFUN,"tag is %s\n", tag);
  if(strcmp(tag, PROPERTY)) {
    log->error(IFUN,"Relationships XML reader: <%s> tag expected\n", PROPERTY);
    result = false;
    goto epilog;	
  }

  topicAtt = findXMLAtt(elem, "topic");

  if(!topicAtt) {
    log->error(IFUN,"missing 'topic' attribute for tag %s\n", tagXMLEle(elem));
    result = false;
    goto epilog;		
  } 

  topicStr = valuXMLAtt(topicAtt);
  if(!strcmp(topicStr,"value"))
    topic = IT_VALUE;
  else if(!strcmp(topicStr,"state"))
    topic = IT_STATE;
  else {
    log->error(IFUN,"incorrect 'topic' attribute value for tag %s\n", tagXMLEle(elem));
    result = false;
    goto epilog;
  }
    
  value = pcdataXMLEle(elem);
  strTrimRight(value, value);	// IN-PLACE TRIM !!

  if(!value) {
    log->error(IFUN,"<%s>: missing PCDATA\n", tag);
    result = false;
    goto epilog;		
  }

  pv = DYNAMIC_CAST(PropertyVector*, observable->find(value));
  if(pv) {
    pv->addObserver(observer, topic);

    log->info(IFUN,"Observer %s listens to %s, property vector %s, topic %s\n",
	      observer->getName(), observable->getName(),
	      pv->getName(), topicStr);
  } else {
    log->error(IFUN,"Relationships Reader: Cant't find property %s for observable %s. Check the XML file\n", value, observable->getName());
    result = false;
  }

 epilog:

  const char* resStr = (result) ? "ok" : "not ok" ;
  log->info(IFUN,"<%s> %s decoding %s.\n", tag, value, resStr);
  return(result);

}

/*---------------------------------------------------------------------------*/

bool 
RelationshipsReader::readObservable(XMLEle* elem, Device* observer)
{ 
  XMLEle* child;
  XMLAtt *nameAtt;
  bool result = true;
  char* tag;
  char *name; 

  tag = tagXMLEle(elem);
  log->verbose(IFUN,"tag is %s\n", tag);

  if(strcmp(tag, OBSERVABLE)) {
    log->error(IFUN,"Relationships XML reader: %s tag expected\n", OBSERVABLE);
    name = "unknown";
    result = false;
    goto epilog;
  }

  nameAtt = findXMLAtt(elem, "name");

  if(!nameAtt) {
    log->error(IFUN,"missing 'name' attribute for tag %s\n", tagXMLEle(elem));
    name = "unknown";
    result = false;
    goto epilog;		
  } 

  name = valuXMLAtt(nameAtt);

  // read children of <observable>
  
  for (child = nextXMLEle(elem,1); child != NULL; child = nextXMLEle(elem,0)) {
    Device* anObservable = DYNAMIC_CAST(Device*,driver->find(name));
    if(anObservable)
      result &= readProperty(child, observer, anObservable);
    else {
      log->error(IFUN,"Relationships XML Reader: Can't find an observable with name %s. Please check the XML file\n",name);
      result = false;
      break;
    }
  }

 epilog:

  const char* resStr = (result) ? "ok" : "not ok" ;
  log->info(IFUN,"<%s> %s decoding %s.\n",tag, name,resStr);
  
  return (result);    

}

/*---------------------------------------------------------------------------*/

bool 
RelationshipsReader::readObserver(XMLEle* elem)
{
  XMLEle* child;
  XMLAtt *nameAtt;
  bool result = true;
  char* tag;
  char *name; 

  tag = tagXMLEle(elem);
  log->verbose(IFUN,"tag is %s\n", tag);
  if(strcmp(tag, OBSERVER)) {
    log->error(IFUN,"Relationships XML reader: %s tag expected\n", OBSERVER); 
    name = "unknown";
    result = false;
    goto epilog;
  }

  nameAtt = findXMLAtt(elem, "name");
  if(!nameAtt) {
    log->error(IFUN,"missing 'name' attribute for tag %s\n", tagXMLEle(elem));
    name = "unknown";
    result = false;
    goto epilog;		
  } 

  name = valuXMLAtt(nameAtt); 


  log->verbose(IFUN,"observer name is %s\n",name);
  log->flush();

  // read children of <observer>
  
  for (child = nextXMLEle(elem,1); child != NULL; child = nextXMLEle(elem,0)) {
    Device* anObserver = DYNAMIC_CAST(Device*,driver->find(name));
    if(anObserver)
      result &= readObservable(child, anObserver);
    else {
      log->error(IFUN,"Relationships XML Reader: Can't find an observer with name %s. Please check the XML file\n",name);
      result = false;
      break;
    }
  }

 epilog:

  const char* resStr = (result) ? "ok" : "not ok" ;
  log->info(IFUN,"<%s> %s decoding %s.\n",tag, name, resStr);

  return (result);    

}

/*---------------------------------------------------------------------------*/

bool 
RelationshipsReader::readRelationships(XMLEle* elem)
{ 
  XMLEle* child;
  char* tag;
  bool  res = true;

  tag = tagXMLEle(elem);
  log->verbose(IFUN,"tag is %s\n", tag);

  if(strcmp(tag, ROOT)) {
    log->error(IFUN,"Relationships XML reader: %s tag expected\n", ROOT);
    return(false);
  }

  for (child = nextXMLEle(elem,1); child != NULL; child = nextXMLEle(elem,0))
    res &= readObserver(child); 

  return(res);
}

/*---------------------------------------------------------------------------*/

bool
RelationshipsReader::read(const char* basedir, const char* path)
{

  XMLEle* root = 0;

  FILE* fp = 0; 
  char errmsg[1024];
  bool res = true;

  xmlFile = basedir;
  xmlFile.append("/");
  xmlFile.append(path);

  fp = fopen(xmlFile.c_str(), "r");  
  if(!fp) {
    log->error(IFUN,"Relationships XML reader: file %s open error\n",
	       xmlFile.c_str());
    return(false);
  }


  root = readXMLFile (fp, lp, errmsg);

  if (root) {
    log->info(IFUN,"Relationships XML reader: decoding the XML file %s\n",path);
    res = readRelationships(root);
    delXMLEle(root);		// deletes the DOM tree
  
  } else if (errmsg[0]) {
    log->error(IFUN,"Relationships XML reader: error reading file: %s\n",errmsg);
    res = false;
  }
  
  log->flush();
  fclose(fp);
    
  return(res);
}

/*---------------------------------------------------------------------------*/
