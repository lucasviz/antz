/* -----------------------------------------------------------------------------
*
*  nppio.c
*
*  ANTz - realtime 3D data visualization tools for the real-world, based on NPE.
*
*  ANTz is hosted at http://openantz.com and NPE at http://neuralphysics.org
*
*  Written in 2010-2014 by Shane Saxon - saxon@openantz.com
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

#include "npio.h"

#include "io/npfile.h"
#include "io/npch.h"
#include "io/npmouse.h"
#include "io/npconsole.h"
#include "io/net/nposc.h"

#include "io/db/npdb.h"


/*! Initialize IO systems
*
*	@param dataRef is a global map reference instance
*
*	@todo add shared resource handling for multiple global map instances
*
*/
//-----------------------------------------------------------------------------
void npInitIO( void* dataRef )
{
	/// init the local IO devices

	/// launch file services and updates hard-coded global variables from file
	npInitFile( dataRef );

	/// Plugin Manager for native libraries and 3rd party modules.
	/// Once loaded, plugins can modify any method or data.
	npInitPlugin( dataRef );

	/// IO channels use both local drives and networking
	npInitCh( dataRef );	

	/// keyboard mapping and event handling
//	npInitKeyboard (dataRef);		//zz

	/// mouse event handling
	npInitMouse( dataRef );
//	npInitSerial (dataRef);			//zz support terminal based system boot-up

	/// audio video input and output
//  npInitAV (dataRef);				//zz
	npInitVideo( dataRef );

	/// start network connections which in turn can further update init state
	npInitOSC( dataRef );			//zz-osc

	/// @todo change npConnectDB over to npInitDB
//	npInitDB( dataRef );

	npConnectDB( dataRef );				//zzsql
}


//-----------------------------------------------------------------------------
void npCloseIO (void* dataRef)
{
	npCloseFile (dataRef);
	npCloseMouse (dataRef);
	npCloseCh (dataRef);	//zz-JJ
}


// update IO devices such as keyboard, mice, video, TNG3B and other inputs
//------------------------------------------------------------------------------
void npUpdateIO (void* dataRef)
{
	pData data = (pData) dataRef;

	data->io.cycleCount++;

	//we double buffer the mouse delta movement to maintain engine cycle sync
	npUpdateMouse (dataRef);
	
	npUpdateConsole (dataRef);

	npUpdateCh (dataRef);			//zz-JJ

	npUpdateDB( dataRef );			//zz db

}


//view tree of files catalogued
//------------------------------------------------------------------------------
void npViewFiles (void* dataRef)
{
	FILE* file = NULL;

	pData data = (pData) dataRef;

	//display names and paths of indexed files

	//let user find and choose the files
//	npFileBrowser (dataRef);
}

//void npdbLoadByName (int item, void* dataRef);

//zz db
void npdbLoadMenuItem (int menuItem, void* dataRef);
//------------------------------------------------------------------------------
void npdbLoadMenuItem (int menuItem, void* dataRef)
{
	int err = 0;
	char msg[kNPurlMax * 2];
	char name[kNPurlMax];			/// item name

//	char host[kNPurlMax];

	pData data = (pData) dataRef;		
	
	pNPdatabases dbList = ((struct databases*)data->io.dbs)->dbList;

	//load database by item index using same list that generated the menu
	strcpy( name, dbList->list[menuItem] );

	//	npdbActiveHost( host, data ); // dbID will replace dbName and host
	err = npdbLoadScene( name, data );

	if( err )
	{
		sprintf( msg, "err 6688 - menu item function fail code: %d", err);
		npPostMsg( msg, kNPmsgErr, data );
	}
}

/// @todo move npdbGetMenu to npdb.h
//------------------------------------------------------------------------------
pNPmenu npdbGetMenu (void* dataRef)
{
	int i = 0;
	int itemCount = 50;		//arbitrary

//	char name[256] = "antz1209111652\0";
	char* name = NULL;
//	char path[kNPmaxPath] = "127.0.0.1\0"; //data->io.db.list[i]			//zzsql
	char path[kNPmaxPath]; 

	pNPdatabases dbList = NULL;
	pNPmenu menu = NULL;

	pData data = (pData) dataRef;


	//get current list of databases and store it in the global data struct
//	printf("\nnpdbGetList");
//	dbList = (pNPdatabases)npdbGetList(dataRef);
	dbList = (pNPdatabases)npdbGetList(&data->io.dbs->myDatabase[0], data );
	if( !dbList ) return NULL;

//	printf("\nAfter npdbGetList");
//	data->io.db.list = dbList;
//	data->io.dbs->dbList->list = dbList;

//	dbList = (pNPdatabases)npdbGetList(dataRef);											//zzsql
	((struct databases*)data->io.dbs)->dbList = dbList;
//	((struct dbNewConnect*)data->io.dbs)->dbList = dbList;				//zzsql
	
	menu = malloc(sizeof(NPmenu));
	if( !menu )
	{
		npPostMsg("err 2255 - malloc failed npdbGetMenu", kNPmsgErr, dataRef);
		return NULL;
	}
	
//	itemCount = ((struct dbNewConnect*)data->io.dbs)->dbList->size;		//zzsql
	itemCount = ((struct databases*)data->io.dbs)->dbList->size;
	menu->list = malloc(sizeof(char*) * itemCount);
	if( !menu->list )
	{
		npPostMsg("err 2256 - malloc failed npdbGetMenu", kNPmsgErr, dataRef);
		return NULL;
	}

	//set the menubar fields
	menu->name = npNewStrcpy("Database", data);
	menu->details = npNewStrcpy("Location               |   Nodes", data);
	if( !menu->name || !menu->details ) return NULL;

	//copy the db list to the menu format
	for (i=1; i <= itemCount; i++)
	{
		menu->list[i] = malloc( 80 + kNPurlMax );	//host can be a URL
		if (!menu)
		{
			npPostMsg("err 2257 - malloc failed npdbGetMenu", kNPmsgErr, dataRef);
			return NULL;
		}
//		name = ((struct dbNewConnect*)data->io.dbs)->dbList->list[i];		  //zzsql
		//get the host address, can be URL, IPv4 or IPv6 as a string
		strcpy( path, data->io.dbs->myDatabase[0].hostIP );

		name = ((struct databases*)data->io.dbs)->dbList->list[i];
		sprintf (menu->list[i], "|%4.0d.| %-33s| %-23s|%10d |", i, (const char*)name, path, 123456 ); //zzsql
//		printf("\n%s", menu->list[i]);
//		sprintf (menu->list[i], "|%4.0d.| %16s | %40s |", i, name, path);	//zzsql
		//dbList->list[i].name, dbList->list[i].details
	}

	//set the list size
	menu->count = itemCount;

	npPostMsg( "Loaded DB List!", kNPmsgDB, dataRef );

	return menu;
}

/// @todo move npViewDatabases, related to: DB, ctrl, map, gl scene, hud, file
//view catalog of DBs from pathmap
//------------------------------------------------------------------------------
void npViewDatabases (void* dataRef)
{
	pNPmenu menuList = NULL;

	pData data = (pData) dataRef;
	pNPconsole console = &data->io.gl.hud.console;

	menuList = malloc(sizeof(NPmenu));

	console->menu = npdbGetMenu(dataRef);
	if (console->menu == NULL)
		return;					//failed to get menu

	//call menu function, pass list ptr and callback function ptr
	npConsoleMenu (npdbLoadMenuItem, console, dataRef);
}

//hybrid ASCII text based console(s) with 3D objects, xtree vr
//choose a folder, file or DB to view, can select multiple items
//manipulating 3D objects changes console text and simultaneously vice versa
//supports multiple instances of the viewer and its components
//selection sets and active object tracking per console, multi-user conpatible
//xtree gold style UI with wildcards and key shortcuts... user prompts
//commands - Load, Save, Cut, Copy, Paste, Delete, Split, Merge, Move, Convert
//select - drag region, shift/ctrl/arrows, wildcard and boolean expressions
//------------------------------------------------------------------------------
void npViewer (void* dataRef)
{
	FILE* file = NULL;

	pData data = (pData) dataRef;

//	if (data->io.gl.fullscreen)	//workaround, gl file dialog conflict
//		npCtrlCommand(kNPcmdFullscreen, data);

	//dynamic components, default has Files on left and DBs on the right
//	npViewFiles(dataRef);			//view tree of files catalogued
	npViewDatabases(dataRef);		//view catalog of DBs from pathmap

	//npViewDataMap()		//can be used for runtime code profiling analysis
	//npViewIO()
	//npViewNetworks()
	//npViewSystem()		//shows HW and SW components, IO, RAM, CPU, PCIe
	//npViewCode()			//code editor with syntax highlighting and compile
							//automatic dynamic plugin loading within antz
	
//	npPostTool (kNPtoolBrowse, data);
	
	//npThread(npDataBrowser, dataRef);

	//currently display a simple list or to launch a file dialog
	//later add full 3D xtree viewer with live data sampling and thumb images...
	// npDisplayDatasets()	//known folders on the left and known DB on the right
	//						//option to specify or browse for new folder or DB
							//options to filter data type, 
								// meta info (date, size, etc.)
								// file types, DB, CSV, JPG...
	// npCtrlCommand(kNPcmdPauseUpdates, dataRef);		//freeze NPE, GL, etc...
	// if (file) Then
	//  file = npFileDialog (data->io.file.csvPath, kNPfileDialogOpen, dataRef);
	//	if (file == directory || file.tableType == tablemap)
	//   do the following in a loop for all files... CSV, JPG...
	//		npFileOpen (file,dataRef);
	//		format = npGetFileFormat (file, dataRef);
	//		dataType = npGetDataType (file, dataRef);
	//		npMultiThread (npParse(fileBuffer,dataRef));
	//		do npReadFileChunk()
	// else (DB)
	//	npListDB();	//list known databases and allow user to choose (multiple)
	//  db = npBrowseDB(dataRef);
	//  npOpenDB (db, dataRef);
	//	
	// sync 
	// npCtrlCommand(kNPcmdDoUpdates, dataRef);	//update tracks, NPE, GL...
	// return 0;


	return;
}

//zz tag debug move this to npconsole.c
//this is the entry point for the current user focus console
//zz add multi-console support here , like multi-window handling
//------------------------------------------------------------------------------
void npConsole (void* dataRef)
{
	pData data = (pData) dataRef;
	pNPconsole console = &data->io.gl.hud.console;

	//activate command console
	npConsoleCmd( console, dataRef );
}

//------------------------------------------------------------------------------
void npPostNodeID( pNPnode node, void* dataRef )
{
	pData data = (pData) dataRef;

	char msg[256];
	char msgPart[256];

	msg[0] = '\0';
	msgPart[0] = '\0';

	if( node->type == kNodeLink )
		strcpy( msg, "edge  " );
	else if( node->branchLevel && !node->childCount )
		strcpy( msg, "leaf  " );
	else if( !node->childCount )
		strcpy( msg, "solo  " );				//tree with only root node
	else if ( !node->branchLevel )
		strcpy( msg, "root  " );
	else
		strcpy( msg, "nexus " );				//internal node that is neither root nor leaf
	

	if (node->type == kNodeCamera)
		sprintf(msgPart, "%s cam ", msg);
	if (node->type == kNodeGrid)
		sprintf(msgPart, "%s grid", msg);
	if (node->type == kNodePin)
		sprintf(msgPart, "%s pin ", msg);
	if (node->type == kNodeLink)
		sprintf(msgPart, "%s link", msg);

		//display id level and tag							//zz add subspace grid_id?
		if (node->recordID)									
			sprintf( msg, "%s level: %-3d id: %-6d rec: %-6d tag: %s", msgPart,
				node->branchLevel + 1, node->id, node->recordID, node->tag->title );
		else
			sprintf( msg, "%s level: %-3d id: %-6d", msgPart,
				node->branchLevel + 1, node->id );
			
		npPostMsg (msg, kNPmsgCtrl, dataRef);
}


