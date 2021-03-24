/*********************************************************************
** Author: Ibrahim Mahmoud
** Date: 9/22/21
** Description: This is the header file for Texture.h
*********************************************************************/

#ifndef TEXTURE
#define TEXTURE

#include <iostream>
#include <glad\glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "stb_image.h"

class Texture
{
	private:


	public:
		unsigned int ID = -1;

		Texture();
		Texture(const char*, bool, bool);
		Texture(const char*, int, int, bool, bool);

		void Use(int);
};

#endif