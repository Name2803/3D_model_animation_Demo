#pragma once


#include <glad/glad.h> 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "../../external/stb_image.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <iostream>
#include <string>

class Mesh;

class MainModel
{
public:

	//vector<Texture> textures_loaded;	// stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
	Mesh** meshes;
	bool gammaCorrection;

	MainModel(std::string path);
	~MainModel();

	void draw();

private:
	std::string f_path;

	void loadModel(std::string path);
	void processNode(aiNode* node, const aiScene* scene, unsigned int point = 0);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	void loadMatirialTexture(Texture* textures, aiMaterial* mat, aiTextureType type, std::string typeName, int texture_amount);
	unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma);
};

