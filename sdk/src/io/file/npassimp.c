#include "npassimp.h"
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

void npAssimpTest(void* dataRef)
{

	aiImportFile("test_model.raw", 0);	

	
	return;
}