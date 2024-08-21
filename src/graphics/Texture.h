#pragma once

#include <iostream>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
//#include "../loaders/png_loading.h"

//#define STB_IMAGE_IMPLEMENTATION
#include "../../external/stb_image.h"

class Texture {
public:
    unsigned int id;
    int width;
    int height;
    int nrChannels;
	Texture(const char* texturePath);
    ~Texture();



    void load_texture(const char* texturePath);
    void bind();

    Texture() = delete;
    Texture(const Texture&) = delete;
    Texture& operator=(const Texture&) = delete;
    Texture& operator=(Texture&& textire2d);
    Texture(Texture&& texture2d);
};
