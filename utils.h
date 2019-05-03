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

#ifndef INDI_UTILS_H
#define INDI_UTILS_H


/******************/
/* basic typedefs */
/******************/

typedef unsigned char  u_char;
typedef short          int16;
typedef unsigned short u_int16;
typedef int            int32;
typedef unsigned int   u_int32;

/*
 * utility macros
 * as an alert to unsafe explicit hardcoded type casting
 * and as workaround for not using:
 *  - static_cast<Type>(expr) 
 *  - dynamic_cast<Type>(expr) 
 *  - const_cast<Type>(expr) 
 *  - reinterpret_cast<Type>(expr) 
 */

#define STATIC_CAST(type,expr)      ((type)(expr))
#define DYNAMIC_CAST(type,expr)     ((type)(expr))
#define CONST_CAST(type,expr)       ((type)(expr))
#define REINTERPRET_CAST(type,expr) ((type)(expr))

/*
 * BEGIN_C_DECLS should be used at the beginning of your declarations,
 *  so that C++ compilers don't mangle their names.  Use END_C_DECLS at
 * the end of C declarations. 
 */

#undef BEGIN_C_DECLS
#undef END_C_DECLS

#ifdef __cplusplus
# define BEGIN_C_DECLS extern "C" {
# define END_C_DECLS };
#else
# define BEGIN_C_DECLS /* empty */
# define END_C_DECLS   /* empty */
#endif


#endif
