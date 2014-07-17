/* -----------------------------------------------------------------------------
*
*  npmapstr.h
*
*  ANTz - realtime 3D data visualization tools for the real-world, based on NPE.
*
*  ANTz is hosted at http://openantz.com and NPE at http://neuralphysics.org
*
*  Written in 2010-2014 by Shane Saxon - makecontact@saxondigital.net
*
*  Please see main.c for a complete list of additional code contributors.
*
*  To the extent possible under law, the author(s) have dedicated all copyright 
*  and related and neighboring rights to this software to the public domain
*  worldwide. This software is distributed without any warranty.
*
*  Released under the CC0 license, which is GPL compatible.
*
*  You should have received a copy of the CC0 Public Domain Dedication along
*  with this software (license file named COPYING.txt). If not, see
*  http://creativecommons.org/publicdomain/zero/1.0/
*
* --------------------------------------------------------------------------- */
/* -----------------------------------------------------------------------------
*  --- high performance string conversion ---					//zz debug, in progress
*
*  converts native C data structs to and from human readable strings
*  parses a defined table chunk size based on either item count or buffer size
*
*  fast string to number conversion and splits up chunks for parallel processing
*
*  supports bi-directional flow control between internal CPU process and IO
*  native string struct is formatted to be byte-for-byte compatible with MySQL
*
*  string struct is handedled by target format process for file, DB or network
*  target formats for import and export include CSV, JSON, XML, KML, OBJ...
*
*  data container and tree structure described separately
*
*  maps all elements of the global data context structure
*  organization of the data tree and element groups is handled by npmap
*
*  JSON formatted data and commands are packaged with OSC for UDP streaming 
*  GIS using GDAL for KML and OBJ files.
*
* --------------------------------------------------------------------------- */

#ifndef NPMAPSTR_H_
#define NPMAPSTR_H_


void npInitMapStr (void* dataRef);
void npCloseMapStr (void* dataRef);


#endif

