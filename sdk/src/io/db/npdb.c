/* -----------------------------------------------------------------------------
*
*  npdb.c
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

#include "npdb.h"


#include "npmysql.h"			/// @todo move these to npdbz	
#include "npostgresql.h"
#include "npsqlite.h"
#include "nphadoop.h"
#include "npmongo.h"

//------------------------------------------------------------------------------
void npInitDB (void* dataRef)
{
	pData data = (pData) dataRef;

	//zzsql moved this from npio.c
//	struct dbNewConnect *connect = malloc(sizeof(struct dbNewConnect)); // debug db //zzsql


//zz dbz	
	struct databases *dbs = malloc(sizeof(struct databases));
	data->io.dbs = dbs;

	npInitMySQL( data );		///< MySQL client is our primary RDBMS
	npInitPostgreSQL( data );	///< PostgreSQL has useful GIS tools
	npInitSQLite( data );		///< SQLite is our native RDMS server
	npInitMongoDB( data );		///< MongoDB is our NoSQL database

//zz dbz	
	dbs->numberOfDatabases = 0;
//zz dbz	
	npAddDb(dbs, "mysql", "localhost", "root", "admin", "", dataRef);

	//((npInitDbConnection(connect, MySQL, "127.0.0.1", "root", "admin", "antz");))
//	npOpenDb()
//zz db2	npOpenDb( &data->io.dbs->myDatabase[0] );
	//npOpenDbConnect(connect, dataRef);
	//npAttachMySQLtoDataRef(connect, dataRef); // debug db // This can be generalized later, debug db
//zz dbz	npAttachDbsToDataRef(dbs, dataRef);
}

//------------------------------------------------------------------------------
void npConnectDB (void* dataRef)
{
	pData data = (pData) dataRef;

	npOpenDb( &data->io.dbs->myDatabase[0] );

//zz dbz	npAttachDbsToDataRef(dbs, dataRef);
}

//------------------------------------------------------------------------------
void npCloseDB (void* dataRef)		//clean-up
{
	return;
}

/** Sync DB with main loop.
	@param dataRef is a global map reference instance.
*/
//------------------------------------------------------------------------------
void npUpdateDB (void* dataRef)							//add to ctrl loop, debug zz
{
	int result = 0;
	char msg[256];
	char host[256];
	pData data = (pData) dataRef;
	pNPdb db = NULL;
	char* dbName = NULL;

	db = &data->io.db[0];
	dbName = data->io.dbs->myDatabase[0].currentlyUsedDatabase;

	/// Main Loop Cycle Count - from startup!
	data->io.cycleCount++;

	if(0)// db->autoUpdate )
	{
		if( data->io.cycleCount % db->updatePeriod == 0 )
			db->update = true;
	}

	/// if scene is flagged for an update then do it.
	if( db->update )
	{
		result = npdbUpdateAntzStateFromDatabase( dataRef );
		
		/// @todo pull the msg up a level, is UI task, not a DB task
		if( !result )
		{
			sprintf( msg, "Done Loading DB Update: %s", dbName );
			npPostMsg( msg, kNPmsgDB, data );
		}	
		else if( result == 4242 )
		{
			npdbActiveHost( host, data );
			printf("host: %s   db: %s\n", host, dbName);
			npPostMsg( "cannot load update - No Active DB", kNPmsgDB, data );
			npPostMsg( "LOAD, SAVE or USE a DB to make Active!", kNPmsgDB, data );
		}

		db->update = false;
	}

	return;
}


//------------------------------------------------------------------------------
int npdbLoadUpdate( void* dataRef )							//add to ctrl loop, debug zz
{
	char msg[kNPurlMax *2];
	char* dbName;//[kNPurlMax];
	char host[kNPurlMax];

	pData data = (pData) dataRef;

	npdbActiveHost( host, data );
	dbName = npdbActiveDB( data );

	if( !dbName || !host )
	{
		printf("host: %s   db: %s\n", host, dbName);
		npPostMsg( "cannot load update - No Active DB", kNPmsgDB, data );
		npPostMsg( "LOAD, SAVE or USE a DB to make Active!", kNPmsgDB, data );
		return 4244;
	}

	sprintf( msg, "Update Scene from DB: %s  host: %s", dbName, host );
	npPostMsg( msg, kNPmsgDB, data );

	//set update flag for all databases related to the scene
	data->io.db[0].update = true;

	return 0;
}


/*
//------------------------------------------------------------------------------
int npdbSaveAs( int serverID, const char* dbName, void* dataRef )
{
	pData data = (pData) dataRef;

	if (0)
	{
		npPostMsg( "Saved Scene to Database", kNPmsgDB, data );
		return 0;
	}

	npPostMsg( "Failed to Save Scene to Database", kNPmsgDB, data );
	return 5523;
}
*/
/*
//------------------------------------------------------------------------------
int npdbPushScene ( int dbID, void* dataRef )							//add to ctrl loop, debug zz
{
int err = 0;
	char* nodeFields = NULL;
	const char* dbName = NULL;

	struct csvStrObjects *nodes = NULL;
	struct newChunksObj *chunks = NULL;
	struct dbFunction *myDbFuncs = NULL;

	pData data = (pData) dataRef;

	myDbFuncs = data->io.dbs->myDatabase[0].db;
	dbName = ((struct databases*)data->io.dbs)->dbList->list[dbID];

	printf("\ndatabase name : %s\n", dbName);

	//if DB exists then truncate it, deletes all rows to do update
	//else create a new DB
	if (0)
	{
		err = npDropDatabase( dbID, myDbFuncs, dbName );
		return err;
	}
	else if( 1 )
	{			//if DB exists then truncate it to delete all rows for update
		err = npdbTruncate( dbID, myDbFuncs, "node_tbl" );
		if( err ) return err;
	}
	else	//create DB tables with defined fields
	{
		err = npCreateDatabase2( dbID, myDbFuncs, (char*)dbName );
		if( err ) return err; 

		err = npUseDatabase2( dbID, myDbFuncs, (char*)dbName );
		if( err ) return err;

		nodeFields = npNewGenMysqlFields( kNPmapFileBufferMax, kNPnode, data );
		if( nodeFields != NULL)
		{
			err = npCreateTable2( myDbFuncs, dbID, "node_tbl", nodeFields );	
			free( nodeFields );
			if( err ) return err; 
		}
	}

	//insert the current scene nodes into the empty DB
	nodes = npRevisedNodeValues( dataRef ); 
	chunks = npEvenNewerAllChunk( nodes, dataRef ); 
	
	err = 0;
	npInsertAllChunks( chunks, dbID, &data->io.dbs->myDatabase[0], "node_tbl");

	npNewFreeChunks( chunks, dataRef );

	return err;
}
*/

/// @todo fix bug where after saving DB update then cannot load updates
//------------------------------------------------------------------------------
int npdbSaveUpdate ( void* dataRef )							//add to ctrl loop, debug zz
{
	int err = 0;
	int dbID = 0;
	char msg[256];
	char* dbName = NULL;
	char host[kNPurlMax];

	pData data = (pData) dataRef;

	dbID = data->io.dbs->myDatabase[0].id;

	dbName = npdbActiveDB( data );
	//npdbActiveHost( hostName, data );
	strcpy( host, data->io.dbs->myDatabase[0].hostIP );

	if( !dbName || !host )
	{
		printf("host: %s   db: %s\n", host, dbName);
		npPostMsg( "cannot save update - No Active DB", kNPmsgDB, data );
		npPostMsg( "LOAD, SAVE or USE a DB to make Active!", kNPmsgDB, data );
		return 4242;
	}

	sprintf( msg, "Save DB Update: %s  host: %s", dbName, host );
	npPostMsg( msg, kNPmsgDB, data );

	err = npdbPushScene( dbID, 
						 data->io.dbs->myDatabase[0].currentlyUsedDatabase, 
						 data );
	
	/// @todo pull the user msg up a level, is UI task, not a DB task
	if( !err )
	{
		sprintf( msg, "Done Saving DB Update: %s  host: %s", dbName, host );
		npPostMsg( msg, kNPmsgDB, data );
	}	
	else
	{
		sprintf( msg, "err 5585 - Save Update failed code: %d DB: %s  host: %s",
				 err, dbName, host );
		npPostMsg( msg, kNPmsgDB, data );
	}

	return err;
}

//------------------------------------------------------------------------------
int npdbUse( const char* dbName, void* dataRef )							//add to ctrl loop, debug zz
{
	int err = 0;
	int dbID = 0;

	struct databases *dbs = NULL;
	struct dbFunction *myDbFuncs = NULL;

	pData data = (pData) dataRef;

	/// @todo support checking DB connection and re-connect on command


	dbs = data->io.dbs;
	myDbFuncs = data->io.dbs->myDatabase[0].db;
	dbID = dbs->myDatabase[0].id;

	err = npUseDatabase2( dbID, myDbFuncs, (char*)dbName );

	/// copy the DB name string to our currently used database reference
	strcpy( data->io.dbs->myDatabase[0].currentlyUsedDatabase, dbName );

	return err;
}

//------------------------------------------------------------------------------
int npdbDrop( const char* dbName, void* dataRef )							//add to ctrl loop, debug zz
{
	int err = 0;
	int dbID = 0;
	char msg[256];

	pData data = (pData) dataRef;

	struct dbFunction *myDbFuncs = data->io.dbs->myDatabase[0].db;

	//if DB exists then drop it
	dbID = data->io.dbs->myDatabase[0].id;
	
	err = npDropDatabase( dbID, myDbFuncs, dbName, data );

	strcpy( data->io.dbs->myDatabase[0].currentlyUsedDatabase, "\0" );

	if( err )
	{
		sprintf( "Failed to Drop Database: %s", dbName, msg );
		npPostMsg( msg, kNPmsgDB, data );
	}
	else
		npPostMsg( "Dropped Database", kNPmsgErr, data );

	return err;
}

//------------------------------------------------------------------------------
void npdbActiveHost( char* hostName, void* dataRef)
{
	pData data = (pData) dataRef;

	strcpy( hostName, data->io.dbs->myDatabase[0].hostIP );
}

//------------------------------------------------------------------------------
char* npdbActiveDB( void* dataRef )
{
	char* dbName = NULL;
	pData data = (pData) dataRef;

	dbName = data->io.dbs->myDatabase[0].currentlyUsedDatabase;
	if( dbName )//strcmp( dbName, "") == 0
	{
		if( dbName[0] == '\0')
			return NULL;
	}

	return dbName;
//	dbName = npMysqlShowStatement("current DB");
}

//------------------------------------------------------------------------------
void npdbSet( char* dbName, char* tblName, char* setStatement, void* dataRef)
{
	int result = 0;
	char statement[65535];

	pData data = (pData) dataRef;

	struct dbFunction *db = data->io.dbs->myDatabase[0].db;
	int dbID = data->io.dbs->myDatabase[0].id;

	sprintf( statement, "UPDATE %s SET %s", 
							tblName, setStatement );

	result = (int)(*db->query)(dbID, statement);
}


//------------------------------------------------------------------------------
void npdbSelect( char* dbName, char* tblName, char* selectWhere, void* dataRef)
{
	int result = 0;
	char statement[65535];

	pData data = (pData) dataRef;

	struct dbFunction *db = data->io.dbs->myDatabase[0].db;
	int dbID = data->io.dbs->myDatabase[0].id;

	sprintf( statement, "UPDATE %s SET selected=1 WHERE %s", 
							tblName, selectWhere );

	result = (int)(*db->query)(dbID, statement);
}


//---------------------------------------------------------------------------
void npdbSaveScene( void* dataRef )
{
	int err = 0;
	int dbID = 0;
	char dbName[kNPurlMax];	//max db identifier is 64 chars
	char host[kNPurlMax];
	char msg[kNPurlMax + kNPurlMax];

	pData data = (pData) dataRef;

	dbID = data->io.dbs->myDatabase[0].id;

	nposTimeStampName( dbName );

	npdbActiveHost( host, data );
/*
	strcpy( question, "Save New Database As?");
	strcpy( defaultAnswer, dbName );
	err = npConsoleAskUser( dbName, "Save Database As?", data );
*/
	sprintf(msg,"Saving New Database: %s  host: %s", dbName, host );
	npPostMsg(msg, kNPmsgView, dataRef);
	err = npdbSaveAs( dbID, dbName, data );
	if( err )
	{
		sprintf( msg, "err 5588 - failed to Save DB, return code: %d", err);
		npPostMsg( msg, kNPmsgView, data );
	}
	else
	{
		sprintf(msg,"Done Saving Database: %s", dbName );
		npPostMsg(msg, kNPmsgView, dataRef);
	}
}

int npdbLoadScene( char* dbName, void* dataRef )
{
	int err = 0;

	char msg[kNPurlMax * 2];

	char host[kNPurlMax];

	pData data = (pData) dataRef;		

	/// @todo add support for multiple DB hosts
	npdbActiveHost( host, data );

	//	sprintf(msg,"Loading DB: %s", selectedItem);
	sprintf( msg,"Loading Database: %s  host: %s", dbName, host );
	npPostMsg( msg, kNPmsgView, data );

	/// @todo change npdbLoadNodeTbl to npdbLoadScene( dbName, host, data );
	/// @todo add support for all tables, especially tags
	err = npdbLoadNodeTbl( dbName, dataRef);

	if( err )
		sprintf( msg,"Failed to Load DB: %s  code: %d", dbName, err );
	else
		sprintf( msg,"Done Loading DB: %s", dbName );

	npPostMsg( msg, kNPmsgDB, dataRef );

	return err;
}

