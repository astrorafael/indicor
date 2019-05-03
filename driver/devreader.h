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

/*
 * Configuration file XML reader
 */

#ifndef INDI_DEVICE_READER_H
#define INDI_DEVICE_READER_H

#include "item.h"
#include "property.h"
#include "propvector.h"
#include "device.h"
#include "driver.h"
#include "lilxml.h"

class DeviceReader {

 public:

  /***********************************/
  /* Device XML file definitons tags */
  /***********************************/

  static const char* DEF_LIGHT;
  static const char* DEF_SWITCH;
  static const char* DEF_TEXT;
  static const char* DEF_NUMBER;
  static const char* DEF_BLOB;
  static const char* DEF_LIGHT_VECTOR;
  static const char* DEF_SWITCH_VECTOR;
  static const char* DEF_TEXT_VECTOR;
  static const char* DEF_NUMBER_VECTOR;
  static const char* DEF_BLOB_VECTOR;
  static const char* DEF_DEVICE;

  DeviceReader(INDIItem* root);
 ~DeviceReader();

  /* reads a device XML configuration file. ok=<NOT NULL> ,fail=NULL */
  /* and returns a Device object */

  Device* read(const char* basedir, const char* path);
  

 private:

  Log* log;
  LilXML* lp; 			/* XML parser */
  INDIItem* driver;		/* The global driver to link devices to */
  std::string xmlFile;
  
  /* reads name and optional label for any INDIitem */
  /* returns true if ok false otherwise */
  bool readNameLabel(XMLEle* elem, char** name, char** label);

  /* read optional attributes for all property vectors */
  bool readAttrsPV(XMLEle* elem, PropertyVector* propv);

  /* read optional attributes for Non Light property vectors */
  bool readAttrsNLPV(XMLEle* elem, NoLightPropertyVector* propv);

  /* reads rule in SwitchPropertyVector */
  bool readRule(XMLEle* elem, ISRule* rule);

  /* factory methods */

  bool readBLOB(XMLEle* elem, INDIItem* parent);
  bool readText(XMLEle* elem, INDIItem* parent);
  bool readLight(XMLEle* elem, INDIItem* parent);
  bool readNumber(XMLEle* elem, INDIItem* parent);
  bool readSwitch(XMLEle* elem, INDIItem* parent);
    
  bool readBLOBVector(XMLEle* elem, INDIItem* parent);
  bool readTextVector(XMLEle* elem, INDIItem* parent);
  bool readLightVector(XMLEle* elem, INDIItem* parent);
  bool readNumberVector(XMLEle* elem, INDIItem* parent);
  bool readSwitchVector(XMLEle* elem, INDIItem* parent);
  
  Device* readDevice(XMLEle* elem, const char* path);

};


#endif
