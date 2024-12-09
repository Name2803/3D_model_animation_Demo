#include "main_model.h"

#include "new_mesh_type.h"

MainModel::MainModel(std::string path):
    f_path(path)
{
	stbi_set_flip_vertically_on_load(true);
	loadModel(path);
}



void MainModel::loadModel(std::string path)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(f_path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) 
    {
        std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
        return;
    }

    f_path = path.substr(0, path.find_last_of('/'));

    meshes = new Mesh*[scene->mNumMeshes];
    processNode(scene->mRootNode, scene);
}

void MainModel::processNode(aiNode* node, const aiScene* scene, unsigned int point)
{
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes[i + point] = &processMesh(mesh, scene);
    }
    
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene, point + node->mNumMeshes);
    }
}


Mesh MainModel::processMesh(aiMesh* mesh, const aiScene* scene)
{
    Vertex* vertices = new Vertex[mesh->mNumVertices];
    unsigned int* indices;
    Texture* textures;

    for (int i = 0; i < mesh->mNumVertices; i++)
    {
        // vertices
        vertices[i].Position.x = mesh->mVertices[i].x;
        vertices[i].Position.y = mesh->mVertices[i].y;
        vertices[i].Position.z = mesh->mVertices[i].z;
        
        // normals
        if (mesh->HasNormals())
        {
            vertices[i].Normal.x = mesh->mNormals[i].x;
            vertices[i].Normal.y = mesh->mNormals[i].y;
            vertices[i].Normal.z = mesh->mNormals[i].z;
        }
        // texture coordiantes
        // does the mesh contain texture coordinates?
        if (mesh->mTextureCoords[0])
        {
            // here we use only one texture, so I should to upgrade it in future
            vertices[i].TexCoords.x = mesh->mTextureCoords[0][i].x;
            vertices[i].TexCoords.y = mesh->mTextureCoords[0][i].y;
            // tagent
            vertices[i].Tangent.x = mesh->mTangents[i].x;
            vertices[i].Tangent.y = mesh->mTangents[i].y;
            vertices[i].Tangent.z = mesh->mTangents[i].z;
            // bitangent
            vertices[i].Bitangent.x = mesh->mBitangents[i].x;
            vertices[i].Bitangent.y = mesh->mBitangents[i].y;
            vertices[i].Bitangent.z = mesh->mBitangents[i].z;
        }
        else
            vertices[i].TexCoords = glm::vec2(0.0f, 0.0f);
    }

    int tmp = 0;
    for (int i = 0; i < mesh->mNumFaces; i++)
    {
        tmp += mesh->mFaces[i].mNumIndices;
    }

    indices = new unsigned int[tmp];
    tmp = 0;
    for (unsigned int i = 0; i < mesh->mNumFaces; ++i)
    {
        for (unsigned int j = 0; j < mesh->mFaces[i].mNumIndices; ++j, ++tmp)
            indices[tmp]= mesh->mFaces[i].mIndices[j];
    }

    tmp = 0;
    textures = new Texture[scene->mNumMaterials];
    /*for (int i = 0; i < scene->mMaterials[mesh->mMaterialIndex]->GetTextureCount(aiTextureType_DIFFUSE); ++i, ++tmp) {}
    for (int i = 0; i < scene->mMaterials[mesh->mMaterialIndex]->GetTextureCount(aiTextureType_DIFFUSE); ++i, ++tmp) {}
    for (int i = 0; i < scene->mMaterials[mesh->mMaterialIndex]->GetTextureCount(aiTextureType_DIFFUSE); ++i, ++tmp) {}
    for (int i = 0; i < scene->mMaterials[mesh->mMaterialIndex]->GetTextureCount(aiTextureType_DIFFUSE); ++i, ++tmp) {}*/

    int tmp1 = loadMatirialTexture(textures, scene->mMaterials[mesh->mMaterialIndex], aiTextureType_DIFFUSE, "texture_diffuse");
    int tmp2 = loadMatirialTexture(textures + tmp1, scene->mMaterials[mesh->mMaterialIndex], aiTextureType_SPECULAR, "texture_specular");
    int tmp3 = loadMatirialTexture(textures + tmp1 + tmp2, scene->mMaterials[mesh->mMaterialIndex], aiTextureType_HEIGHT, "texture_normal");
    loadMatirialTexture(textures + tmp1 + tmp2 + tmp3, scene->mMaterials[mesh->mMaterialIndex], aiTextureType_AMBIENT, "texture_height");
    return Mesh(vertices, indices, textures);

}

int loadMatirialTexture(Texture* textures, aiMaterial* mat, aiTextureType type, std::string typeName)
{
    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);
        std::cout << str.data << "\n";
        // check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
        bool skip = false;
        for (unsigned int j = 0; j < textures_loaded.size(); j++)
        {
            if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
            {
                textures.push_back(textures_loaded[j]);
                skip = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
                break;
            }
        }
        if (!skip)
        {   // if texture hasn't been loaded already, load it
            Texture texture;
            texture.id = TextureFromFile(str.C_Str(), this->directory);
            texture.type = typeName;
            texture.path = str.C_Str();
            textures.push_back(texture);
            textures_loaded.push_back(texture);  // store it as texture loaded for entire model, to ensure we won't unnecessary load duplicate textures.
        }
    }
    return textures;
}
