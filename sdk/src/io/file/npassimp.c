#include "npassimp.h"
#include "../gl/npgeometry.h"
#include "SOIL.h"					//zz debug should just be SOIL.h

// ----------------------------------------------------------------------------
/*
void recursive_render (const struct aiScene *sc, const struct aiNode* nd, void* dataRef)
{
	pData data = (pData) dataRef;
	pNPassimp assimp = data->io.assimp;
	unsigned int i;
	unsigned int n = 0, t;
	struct aiMatrix4x4 m = nd->mTransformation;

	// update transform
	aiTransposeMatrix4(&m);
	glPushMatrix();
//	glMultMatrixf((float*)&m);

	// draw all meshes assigned to this node
	for (; n < nd->mNumMeshes; ++n) {
//		const struct aiMesh* mesh = scene->mMeshes[nd->mMeshes[n]];
		const struct aiMesh* mesh = assimp->scene->mMeshes[nd->mMeshes[n]];

	//	apply_material(sc->mMaterials[mesh->mMaterialIndex]);

		if(mesh->mNormals == NULL) {
		//	glDisable(GL_LIGHTING);
		} else {
		//	glEnable(GL_LIGHTING);
		}

		for (t = 0; t < mesh->mNumFaces; ++t) {
			const struct aiFace* face = &mesh->mFaces[t];
		//	GLenum face_mode;

			switch(face->mNumIndices) {
		//		case 1: face_mode = GL_POINTS; break;
		//		case 2: face_mode = GL_LINES; break;
		//		case 3: face_mode = GL_TRIANGLES; break;
		//		default: face_mode = GL_POLYGON; break;
			}

	//		glBegin(face_mode);

			for(i = 0; i < face->mNumIndices; i++) {
				int index = face->mIndices[i];
				if(mesh->mColors[0] != NULL)
				{
	//				glColor4fv((GLfloat*)&mesh->mColors[0][index]);
				}
				if(mesh->mNormals != NULL) 
				{
	//				glNormal3fv(&mesh->mNormals[index].x);
				}
	//			glVertex3fv(&mesh->mVertices[index].x);
			}

//			glEnd();
		}

	}

	// draw all children
	for (n = 0; n < nd->mNumChildren; ++n) {
	//	recursive_render(sc, nd->mChildren[n]);
	}

//	glPopMatrix();
}
*/

void npAssimpInit(void* dataRef)
{
	pData data = (pData) dataRef;
	pNPassimp assimp = data->io.assimp;
	int index = 0;
	
	assimp = (pNPassimp)malloc(sizeof(npAssimp));
	if(assimp == NULL)
	{
		printf("\nmalloc failed (npAssimpInit)");
		assimp->running = false;
		return;
	}

	data->io.assimp = assimp;
	assimp->filepath[0] = '\0';
	assimp->numOfScenes = 0;

	for(index = 0; index < assimp->numOfScenes+1; index++)
	{
		assimp->scene[index] = NULL;
	}

	assimp->running = true;
	return;
}

void npAssimpGetTexturePath(void* dataRef)
{

}

void npIsValidPath(void* dataRef)
{
	
	
}

void npGetFileDirPath(char* filepath, char* dir, void* dataRef)
{
	int filepathSize = 0;
	int filepathIndex = 0;
	char* ptr = NULL;
	strcpy(dir, filepath);

	printf("\nnpGetFileDirPath");
	ptr = strstr(dir, ".");
	filepathSize = strlen(filepath);

	if( strstr(filepath, "\\") == NULL )
	{
		dir[0] = '\0';
		return;
	}

	printf("\nbefore while");
	while( (*ptr) != '\\' )
		ptr--;

	(*ptr) = '\0';
	printf("\nend npGetFileDirPath");
	return;
}

void npGetFileNameFromPath(char* filepath, char* filename, void* dataRef)
{
	char* ptr = NULL;

	printf("\nnpGetFileNameFromPath");
	ptr = strstr(filepath, ".");
	printf("\nfilepath : %s", filepath);
	//system("pause");

	if( strstr(filepath, "/") == NULL)
	{
		printf("\nasdf");
		strcpy(filename, filepath);
		return;
	}

	if( filepath[0] == '.' && filepath[1] == '/' )
	{
		printf("filename : %s", &filepath[2]);
		strcpy(filename, &filepath[2]);
	//	printf("\nzxcv");
	//	system("pause");
		return;
	}

	while( (*ptr) != '/' ) 
	{
		ptr--;
		printf("\niter");
	}

	ptr++;
	strcpy(filename, ptr);
	printf("\nnpGetFileNameFromPath filename : %s", filename);
	
}

/// pass output from npGetFileNameFromPath
char* npSearchPathsForFile(char* filename, void* dataRef)
{
	char* filepath = NULL;
	char* searchpaths[4];
	int index = 0;
	pData data = (pData) dataRef;

//	searchpath
	searchpaths[0] = data->io.file.appPath;
	searchpaths[1] = data->io.file.modelPath;
	searchpaths[2] = data->io.file.csvPath;
	searchpaths[3] = data->io.file.mapPath;

	if(filename[0] != '\0' && filename != NULL)
	{
		for(index = 0; index < 4; index++)
		{
			if( nposFileExistsAtDir(searchpaths[index], filename, dataRef ) == true )
			{
				filepath = malloc(sizeof(char) *  (strlen(searchpaths[index]) + strlen(filename) + 1) );
				if(filepath != NULL)
				{
					filepath[0] = '\0';
					sprintf(filepath, "%s%s", searchpaths[index], filename);
					printf("\nfilepath : %s", filepath);
					return filepath;
				}
				else
				{
					return NULL;
				}
			}
		}
	}

	return NULL;
}
/*
bool npAssimpDoesTextureFileExist(char* filepath, void* dataRef)
{		
	pData data = (pData) dataRef;

	if(filepath[0] != '\0')
	{
		npGetFileDirPath(filepath, &dir[0], dataRef);
		npGetFileNameFromPath(filepath, &filename[0], dataRef); 
		if(dir[0] == '\0')
		{
			printf("\nno path, just filename");		
			/// now look in the models folder
			fileExists = nposFileExistsAtDir(data->io.file.modelPath, &filename[0], dataRef);
			if(fileExists == true)
			{
				printf("\nfile exist @ %s", data->io.file.modelPath);
				sprintf(revisedPath, "%s%s", data->io.file.modelPath, filename);
				data->map.currentNode->textureID = SOIL_load_OGL_texture
				(
				revisedFilePath, /// file path
				SOIL_LOAD_AUTO,
				SOIL_CREATE_NEW_ID,
				SOIL_FLAG_INVERT_Y |
				SOIL_FLAG_MIPMAPS //|			//disabling breaks RGBA textures
				// | SOIL_FLAG_NTSC_SAFE_RGB	//we want the entire RGB spectrum
				// | SOIL_FLAG_COMPRESS_TO_DXT	//no lossy compression, faster too
				);	
	
//				if(textureID != 0)
//					return;
	

			}
			else
			{
				printf("\nfile does not exist @ %s", data->io.file.modelPath);
			}

			/// now look in the current working directory	
			fileExists = nposFileExistsAtDir(data->io.file.appPath, &filename[0], dataRef);
			if(fileExists == true)
			{
				printf("\nfile exists in current working directory @ %s", data->io.file.appPath);
				sprintf(revisedPath, "%s%s", data->io.file.appPath, filename);
				data->map.currentNode->textureID = SOIL_load_OGL_texture
				(
				revisedPath, /// file path
				SOIL_LOAD_AUTO,
				SOIL_CREATE_NEW_ID,
				SOIL_FLAG_INVERT_Y |
				SOIL_FLAG_MIPMAPS //|			//disabling breaks RGBA textures
				// | SOIL_FLAG_NTSC_SAFE_RGB	//we want the entire RGB spectrum
				// | SOIL_FLAG_COMPRESS_TO_DXT	//no lossy compression, faster too
				);	
			}
			else
			{
				printf("\nfile does not exist @ %s", data->io.file.appPath);
			}
	}
		else
		{
			printf("\ndir : %s", dir);
			dirExists = nposDirExists(dir, dataRef);
			if(dirExists == true)
			{
				printf("\ndirectory exists");
				fileExists = nposFileExistsAtDir(&dir[0], filename, dataRef);
				if(fileExists == true)
				{
					data->map.currentNode->textureID = SOIL_load_OGL_texture
					(
					assimp->path.data, /// file path
					SOIL_LOAD_AUTO,
					SOIL_CREATE_NEW_ID,
					SOIL_FLAG_INVERT_Y |
					SOIL_FLAG_MIPMAPS //|			//disabling breaks RGBA textures
					// | SOIL_FLAG_NTSC_SAFE_RGB	//we want the entire RGB spectrum
					// | SOIL_FLAG_COMPRESS_TO_DXT	//no lossy compression, faster too
					);	
					
				}
				else 
				{
						
				}
			}
			else if(dirExists == false)
			{
				printf("\ndirectory does not exist");
				/// now look in the models folder
				fileExists = nposFileExistsAtDir(data->io.file.modelPath, &filename[0], dataRef);
				if(fileExists == true)
				{
					printf("\nfile exist @ %s", data->io.file.modelPath);
				}
				else
				{
					printf("\nfile does not exist @ %s", data->io.file.modelPath);
				}

				/// now look in the current working directory	
				fileExists = nposFileExistsAtDir(data->io.file.appPath, &filename[0], dataRef);
				if(fileExists == true)
				{
					printf("\nfile exists @ %s", data->io.file.appPath);
				}
				else
				{
					printf("\nfile does not exist @ %s", data->io.file.appPath);
				}

			}
		}


//		fileExists = nposFileExists(assimp->path.data, dataRef);
//		if(fileExists == true)
//		{
//			printf("\nfile exists");
//		}
//		else
//		{
//			printf("\nfile does not exist");
//		}
		system("pause");
	}

	return false;
}
*/




void npAssimpTest(void* dataRef)
{
	struct aiString str;
	float blend = 0;
	char* myStr = NULL;
	char* driveLetter = NULL;
	char* delimit = NULL;
	char* last = NULL;
	char filename[256];
	char revisedPath[256] = {'\0'};
	char* filepath = NULL;
	char dir[256] = {'\0'};
	unsigned int uvind = 0;
	int test = 0;
	struct aiScene *scene = NULL;
	struct aiMaterial *material = NULL;
	int op = 0;
	int mode[3] = {0};
	pData data = (pData) dataRef;
	int x = 0;
	int index = 0;
	static int id = 1;
	int mapping = 0;
	unsigned int type = 0;
	unsigned int idx = 0;
	unsigned int flags = 0;
	void* image = NULL;
	bool fileExists = false;
	bool dirExists = false;
	int total = 0;
	pNPassimp assimp = (pNPassimp)data->io.assimp;

    static const int types[] = {
          aiTextureType_NONE,
          aiTextureType_DIFFUSE,
          aiTextureType_SPECULAR,
          aiTextureType_AMBIENT,
          aiTextureType_EMISSIVE,
          aiTextureType_HEIGHT,
          aiTextureType_NORMALS,
          aiTextureType_SHININESS,
          aiTextureType_OPACITY,
          aiTextureType_DISPLACEMENT,
          aiTextureType_LIGHTMAP,
          aiTextureType_REFLECTION,
          aiTextureType_UNKNOWN
    };

	if( (assimp->running == false) || (assimp == NULL) ) 
		return;

	if(assimp->filepath[0] != '\0')
		assimp->scene[++(assimp->numOfScenes)] = aiImportFile(assimp->filepath, 0);
	else ///filepath is null 
		return;

	if( assimp->scene[assimp->numOfScenes] != NULL )
	{
//		printf("\nnumOfScenes : %d", assimp->numOfScenes);
		scene = assimp->scene[assimp->numOfScenes]; /// assimp->scene[0] is dummy value (NULL) 
		material = scene->mMaterials[0];
		
		assimp->path.data[0] = '\0';

		for(type = 0; type < sizeof(types)/sizeof(types[0]); ++type)
		{
		//	printf("\ntype : %d", type);
			for(idx = 0; AI_SUCCESS==aiGetMaterialString(scene->mMaterials[0],
				AI_MATKEY_TEXTURE(types[type],idx),&assimp->path); idx++)
			{
		//		printf("%s\n    \'%s\'",(total++?"":"\nTexture Refs:" ),assimp->path.data);
			}
		}


		/// check for empty path
		if(assimp->path.data[0] == '\0')
		{
			data->map.currentNode->modelId = assimp->numOfScenes;
			data->map.currentNode->geometry = kNPgeoAssimp;
			
			/// Set the colorIndex & color.rgb so the texture can be clearly seen
			/*
			data->map.currentNode->colorIndex = 19;
			data->map.currentNode->color.r = 255;
			data->map.currentNode->color.g = 255;
			data->map.currentNode->color.b = 255;
			*/
			data->map.currentNode->textureID = 0;
			return;
		}

		data->map.currentNode->textureID = SOIL_load_OGL_texture
		(
		assimp->path.data, /// file path
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_INVERT_Y |
		SOIL_FLAG_MIPMAPS //|			//disabling breaks RGBA textures
		// | SOIL_FLAG_NTSC_SAFE_RGB	//we want the entire RGB spectrum
		// | SOIL_FLAG_COMPRESS_TO_DXT	//no lossy compression, faster too
		);	

		if( data->map.currentNode->textureID == 0)
		{
			npGetFileNameFromPath(assimp->path.data, &filename[0], dataRef);
//			printf("Searching paths for file");
			filepath = npSearchPathsForFile(filename, dataRef);

			if(filepath != NULL)
			{
				data->map.currentNode->textureID = SOIL_load_OGL_texture
				(
				filepath, /// file path
				SOIL_LOAD_AUTO,
				SOIL_CREATE_NEW_ID,
				SOIL_FLAG_INVERT_Y |
				SOIL_FLAG_MIPMAPS //|			//disabling breaks RGBA textures
				// | SOIL_FLAG_NTSC_SAFE_RGB	//we want the entire RGB spectrum
				// | SOIL_FLAG_COMPRESS_TO_DXT	//no lossy compression, faster too
				);	

				if (data->map.currentNode->textureID != 0)
				{
					data->io.gl.textureCount = data->map.currentNode->textureID;

					data->map.currentNode->modelId = assimp->numOfScenes;
					data->map.currentNode->geometry = kNPgeoAssimp;
					data->map.currentNode->colorIndex = 19;
					data->map.currentNode->color.r = 255;
					data->map.currentNode->color.g = 255;
					data->map.currentNode->color.b = 255;
				}
				else
				{
					/// Texture load failed
				}
			}
			else
			{
				printf("\nSearch could not find file");
				return;
			}
		}
		else
		{
			data->io.gl.textureCount = data->map.currentNode->textureID;

			data->map.currentNode->modelId = assimp->numOfScenes;
			data->map.currentNode->geometry = kNPgeoAssimp;
			data->map.currentNode->colorIndex = 19;
			data->map.currentNode->color.r = 255;
			data->map.currentNode->color.g = 255;
			data->map.currentNode->color.b = 255;
		}

		/*
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); 
		printf("\nwidth : %d & Height : %d", data->io.texmap.width, data->io.texmap.height);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, data->io.texmap.width, data->io.texmap.height, 0, GL_RGB, GL_UNSIGNED_BYTE, data->io.texmap.image);
		*/

//		printf( "\nSOIL loading error: '%s'\n", SOIL_last_result() );
	}
	else
	{
		/// model failed to load, decrement numOfScenes
		assimp->numOfScenes--;
		printf("\nModel failed to load : %s", aiGetErrorString());
		return;
	}

	return;
}