#include "main_model.h"
#include "basic_file_mesh.h"

MainModel::MainModel(std::string path, bool set_texture_flip)
{
    
	stbi_set_flip_vertically_on_load(set_texture_flip);
	loadModel(path);
}

MainModel::~MainModel()
{
    delete[] meshes;
    delete[] textures_loaded;
}

void MainModel::draw(Shader& shader)
{
    
    for (int i = 0; i < mesh_count; ++i)
        meshes[i]->draw(shader);
}



void MainModel::loadModel(std::string path)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) 
    {
        std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
        return;
    }
    f_path = path.substr(0, path.find_last_of('/'));
    
    mesh_count = 0;
    count_meshes(scene->mRootNode);
    meshes = new BFMesh*[mesh_count];
    textures_loaded = new Texture[scene->mNumMaterials];

    processNode(scene->mRootNode, scene);
}

void MainModel::processNode(aiNode* node, const aiScene* scene, unsigned int point)
{
    
    for (unsigned int i = 0; i < node->mNumMeshes; ++i)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes[i + point] =  processMesh(mesh, scene);
    }
    
    for (unsigned int i = 0; i < node->mNumChildren; ++i)
    {
        processNode(node->mChildren[i], scene, point + node->mNumMeshes);
    }
}


BFMesh* MainModel::processMesh(aiMesh* mesh, const aiScene* scene)
{
    Vertex* vertices = new Vertex[mesh->mNumVertices];
    unsigned int* indeces;
    Texture* textures;

    int* counts = new int[3];
    counts[0] = mesh->mNumVertices;
    for (int i = 0; i < mesh->mNumVertices; ++i)
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

    counts[1] = 0;
    for (int i = 0; i < mesh->mNumFaces; i++)
    {
        counts[1] += static_cast<int>(mesh->mFaces[i].mNumIndices);
    }

    indeces = new unsigned int[counts[1]];
    int tmp = 0;
    for (unsigned int i = 0; i < mesh->mNumFaces; ++i)
    {
        for (unsigned int j = 0; j < mesh->mFaces[i].mNumIndices; ++j, ++tmp)
            indeces[tmp]= mesh->mFaces[i].mIndices[j];
    }

    
    counts[2] = 0;
    load_matirial_texture(scene->mMaterials[mesh->mMaterialIndex], aiTextureType_DIFFUSE, "texture_diffuse", &counts[2]);
    load_matirial_texture(scene->mMaterials[mesh->mMaterialIndex], aiTextureType_SPECULAR, "texture_specular", &counts[2]);
    load_matirial_texture(scene->mMaterials[mesh->mMaterialIndex], aiTextureType_HEIGHT, "texture_normal", &counts[2]);
    load_matirial_texture(scene->mMaterials[mesh->mMaterialIndex], aiTextureType_AMBIENT, "texture_height", &counts[2]);
   
    return new BFMesh(vertices, textures_loaded, indeces, counts);
}

void MainModel::load_matirial_texture(aiMaterial* mat, aiTextureType type, std::string typeName, int* texture_amount)
{
    for (unsigned int i = 0; i < mat->GetTextureCount(type); ++i)
    {
        aiString str;
        mat->GetTexture(type, i, &str);

        bool skip = false;
        for (unsigned int j = 0; j < *texture_amount; ++j)
        {
            if (strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
            {
                skip = true;
                break;
            }
        }

        if (!skip)
        {
            textures_loaded[*texture_amount].id = TextureFromFile(str.C_Str(), this->f_path);
            textures_loaded[*texture_amount].type = typeName.c_str();
            textures_loaded[*texture_amount].path = str.C_Str();
            ++(*texture_amount);
            std::cout << "texture_amount: " << *texture_amount << "\n";
        }
    }
}


unsigned int MainModel::TextureFromFile(const char* path, const std::string& directory)
{
    std::string filename = std::string(path);
    filename = directory + '/' + filename;

    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, chanels;
    unsigned char* data = stbi_load(filename.c_str(), &width, &height, &chanels, 0);
    if (data)
    {
        GLenum format;
        if (chanels == 1)
            format = GL_RED;
        else if (chanels == 3)
            format = GL_RGB;
        else if (chanels == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}

void MainModel::count_meshes(const aiNode* node)
{
    if (node->mNumMeshes > 0) {
        mesh_count += node->mNumMeshes;
    }
    for (unsigned int i = 0; i < node->mNumChildren; ++i) {
        count_meshes(node->mChildren[i]);
    }
}
