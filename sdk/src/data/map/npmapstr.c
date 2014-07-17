/* -----------------------------------------------------------------------------
*
*  npmapstr.c
*
*  ANTz - realtime 3D data visualization tools for the real-world, based on NPE.
*
*  http://openantz.com and NPE at http://neuralphysics.org
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

#include "npmapstr.h"

#include "../../npdata.h"

// #include "../io/npch.h"
// #include "../io/gl/nptags.h"	//move the necessary functions to npdata, debug zz

//-----------------------------------------------------------------------------
void npInitMapStr (void* dataRef)
{
	return;
}

//-----------------------------------------------------------------------------
void npCloseMapStr (void* dataRef)
{
	return;
}

//-----------------------------------------------------------------------------
void npUpdateMapStr (void* dataRef)
{
	return;
}

//passed in ptr to the CSV buffer and converts it to a string struct that is
//compatible with MySQL query struct... 
//conversion overwrites the comma deliminators with null terminators '\0'
//which turns the CSV file into a series of C strings... ta da!
//pads all deliminators with '\0' such as quotes, commas and line returns
//and creates separate ptr array to the start of each string with additional
//array for string length
//buffer with str map struct is returned to npmapstr which converts the strings 
//to the target data structure member types, primarily int, float and string
//then the buffer clear flag is set to allow re-use for next file chunk
//-----------------------------------------------------------------------------
void npCSVtoStr (char** str, const char* csv, int type, int size, void* dataRef)
{
	int i = 0;
//	pNProwFormat row = NULL;	//based on type

//	for (i=0; i < row->count; i++)
	{

	}
}

//hard coded optimization for our native data structs
//npStrToNode

//converts any table structure to a dynamically defined generic C container
//low-level on a per table type or per record bases
//separate JSON compatible shema can be used to define the data tree structure
//supporst plugins to define and work with any arbitrary data structure
//efficiency is almost as good as the structure specific hard coded routines
//some light processing overhead to for the loop and pointer math, etc...
//-----------------------------------------------------------------------------
void npMapStrToC (void** dest, const char* str, int type, int size, void* dataRef)
{
	int i = 0, j = 0;
/*
	int elementSize = npGetElementSize(type);
	int rowColumnCount = npGetColumnCount(type);

	//convert rows till we reach the defined endpoint size
	for (i < size)
	{
		//convert a row (strings) to C data type
		for (j=0; j < rowColumnCount; j++)
		{
			switch (typeElement[j])
			{
				case kNPint : dest[i] = npatoi(str[j]); break;
				case kNPfloat : dest[i] = npatof(str[j]); break;
				case kNPcstr : dest[i] = npstrncpy(str[j], strLen[j]); break;
			}
		}
		dest[i]
		i += elementSize
	}
*/
}

