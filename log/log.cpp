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


#include <stdarg.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>

#include "utils.h"
#include "log.h"

const char* Log::NONE  = "NONE";
const char* Log::ERROR = "ERROR";
const char* Log::WARN  = "WARN";
const char* Log::INFO  = "INFO";
const char* Log::DEBUG = "DEBUG";
const char* Log::VERBO = "VERBOSE";
