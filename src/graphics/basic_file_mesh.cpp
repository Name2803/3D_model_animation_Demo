#include "basic_file_mesh.h"


BFMesh::BFMesh(Vertex* vertices_, Texture* textures_, unsigned int* indeces_,int* counts_)
{
	vertices = vertices_;
	textures = textures_;
	indeces = indeces_;
	counts = counts_;
}


BFMesh::~BFMesh()
{

}