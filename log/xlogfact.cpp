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

#include "xlogfact.h"

#include "log0.h"
#include "log1.h"
#include "log2.h"
#include "log3.h"
#include "log4.h"

const char*
XLogFactory::LOG   = "log";

const char*
XLogFactory::PATH   = "path";

const char*
XLogFactory::CLASS = "class";

const char*
XLogFactory::NAME  = "name";

const char*
XLogFactory::VALUE = "value";


/*---------------------------------------------------------------------------*/

XLogFactory::XLogFactory() :
  path("stderr")
{ 
  lp = newLilXML(); 
}


/*---------------------------------------------------------------------------*/

XLogFactory::~XLogFactory()
{
  delLilXML(lp);
}

/*---------------------------------------------------------------------------*/

bool
XLogFactory::read(const char* basedir, const char* newXmlFile)
{
  FILE* fp = 0; 
  char errmsg[1024];
  XMLEle* root;			/* root of DOM tree */

  xmlFile = basedir;
  xmlFile.append("/");
  xmlFile.append(newXmlFile);

  fp = fopen(xmlFile.c_str(), "r");  
  if(!fp) {
    fprintf(stderr,"[ERROR] %s : file %s open error\n", IFUN, xmlFile.c_str());
    return(false);
  }

  bool res = true;
  root = readXMLFile (fp, lp, errmsg);

  if (root) {
    res = readLog(root);  
    delXMLEle(root);
  } else if (errmsg[0]) {
    fprintf(stderr," %s : error reading file: %s\n", IFUN, errmsg);
    res = false;
  }
  
  fclose(fp);   
  return(res);
}

/*---------------------------------------------------------------------------*/

bool 
XLogFactory::readLog(XMLEle* elem)
{ 
  XMLEle* child; 
  XMLAtt *pathAtt;
  char* tag;
  bool  res = true;

  tag = tagXMLEle(elem);

  if(strcmp(tag, LOG)) {
    fprintf(stderr,"[ERROR] %s : expected tag '%s' and found tag '%s'\n",
	    IFUN, LOG, tagXMLEle(elem));
    return(false);
  }

  /* reads the path attribute */
  pathAtt = findXMLAtt(elem, PATH);
  if(!pathAtt) {
    fprintf(stderr,"[ERROR] %s : missing '%s' attribute for tag %s\n",
	    IFUN, PATH, tagXMLEle(elem));
    return(false);
  } 

  path = valuXMLAtt(pathAtt); 


  for (child = nextXMLEle(elem,1); child != NULL; child = nextXMLEle(elem,0))
    res &= readClass(child); 

  return(res);
}

/*---------------------------------------------------------------------------*/

bool 
XLogFactory::readClass(XMLEle* elem)
{ 
  
  XMLAtt *nameAtt;
  XMLAtt *valueAtt;

  char* tag;
  char *name; 
  char *value; 

  tag = tagXMLEle(elem);

  if(strcmp(tag, CLASS)) {
    fprintf(stderr,"[ERROR] %s : expected tag '%s' and found tag '%s'\n",
	    IFUN, CLASS, tagXMLEle(elem));
    return(false);
  }

  nameAtt = findXMLAtt(elem, NAME);
  if(!nameAtt) {
    fprintf(stderr,"[ERROR] %s : missing '%s' attribute for tag %s\n",
	    IFUN,NAME,tagXMLEle(elem));
    return(false);
  } 
  name = valuXMLAtt(nameAtt);

  valueAtt = findXMLAtt(elem, VALUE);
  if(!valueAtt) {
    fprintf(stderr,"[ERROR] %s : missing '%s' attribute for tag %s\n",
	    IFUN,VALUE,tagXMLEle(elem));
    return(false);
  } 
  value = valuXMLAtt(valueAtt);
  logLevelMap.insert(LogLevelMap::value_type(name,value));
  return(true);
}

/*---------------------------------------------------------------------------*/

Log*
XLogFactory::forClass(const char* name)
{
  std::string key(name);
  LogLevelMap::const_iterator i;
  std::string value;
  
  /* find given key. If not found returns a NONE log level */
  i = logLevelMap.find(key);
  if(i == logLevelMap.end()) {
     fprintf(stderr,"[ WARN] %s : name %s not found\n", IFUN, name);
    return(new Log2());
  }

  /* key found, create the corresponding log level */
  value = (*i).second;
  if(!strcmp(value.c_str(), Log::VERBO)) {
    return(new Log5());
  } else if(!strcmp(value.c_str(), Log::DEBUG)) {
    return(new Log4());
  } else if(!strcmp(value.c_str(), Log::INFO)) {
    return(new Log3());
  } else if(!strcmp(value.c_str(), Log::WARN)) {
    return(new Log2());
  } else if(!strcmp(value.c_str(), Log::ERROR)) {
    return(new Log1());
  } else if(!strcmp(value.c_str(), Log::NONE)) {
    return(new Log0());
  } else {
    return(new Log2());
  }
}

/*---------------------------------------------------------------------------*/
