#ifndef NPASSIMP_H_
#define NPASSIMP_H_


#include "../../npdata.h"
#include "../../os/npos.h"
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

struct npAssimp {
 bool running;
 struct aiString path;
 char filepath[200];
 struct aiScene *scene[50];
 int numOfScenes;
 struct aiMesh *mesh[500];
 int numOfMeshes;
};
typedef struct npAssimp npAssimp;
typedef npAssimp* pNPassimp;


void npGetFileDirPath(char* filepath, char* dir, void* dataRef);
void npGetFileNameFromPath(char* filepath, char* filename, void* dataRef);
void npAssimpTest(void* dataRef);
void npAssimpInit(void* dataRef);

#endif