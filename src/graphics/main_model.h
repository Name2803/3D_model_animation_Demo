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

class BFMesh;
class Texture;
class Shader;

class MainModel
{
public:

	Texture* textures_loaded;	// stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
	BFMesh** meshes;
	bool gammaCorrection;

	MainModel(std::string path, bool set_texture_flip = false);
	~MainModel();

	void draw(Shader& shader);


private:
	std::string f_path;
	int mesh_count;
	void loadModel(std::string path);
	void processNode(aiNode* node, const aiScene* scene, unsigned int point = 0);
	BFMesh* processMesh(aiMesh* mesh, const aiScene* scene);
	void load_matirial_texture(aiMaterial* mat, aiTextureType type, std::string typeName, int* texture_amount);
	unsigned int TextureFromFile(const char* path, const std::string& directory);
	// Рекурсивная функция для подсчета мешей
	void count_meshes(const aiNode* node);
};

