/* -----------------------------------------------------------------------------
*
*  nptexmap.h
*
*  ANTz - realtime 3D data visualization tools for the real-world, based on NPE.
*
*  ANTz is hosted at http://openantz.com and NPE at http://neuralphysics.org
*
*  Written in 2010-2016 by Shane Saxon - saxon@openantz.com
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
*  with this software (license file named LICENSE.txt). If not, see
*  http://creativecommons.org/publicdomain/zero/1.0/
*
* --------------------------------------------------------------------------- */

#ifndef NPTEXMAP_H_
#define NPTEXMAP_H_

#include "../../npdata.h"

//------------------------------------------------------------------------------
void npInitTexMap (void* dataRef);
void npUpdateTexMap (void* dataRef);
int npGeoTexId( int geometryId, void* dataRef );
pNPtexmap npAddTexMap(int* extTexId, char* filename, char* path, void* dataRef);
void npLoadExtTexMaps(void* dataRef);
void npCloseTexMap (void* dataRef);

pNPtexmap npTexlistSearchFile(char* filename, char* path, void* dataRef);
void npUpdateTexMap (void* dataRef);

void npLoadGeos(void* dataRef);
void npLoadTextures(void* dataRef);

int npLoadTexture( char* filePath, int fileType, void* dataRef);

int npSetTexture( int textureID, void* dataRef);

int npScreenGrab( char* filePath, int type, int x, int y, int w, int h, void* dataRef);

int npScreenGrabThumb( char* name, int type, int x, int y, int w, int h, void* dataRef );

pNPtexmap npTexTest(void* dataRef);

int npGetUnusedExtTexId(void* dataRef);

#endif

