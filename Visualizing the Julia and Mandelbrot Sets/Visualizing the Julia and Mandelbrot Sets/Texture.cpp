/*********************************************************************
** Author: Ibrahim Mahmoud
** Date: 9/22/21
** Description: This is the implementation file for Texture.h
*********************************************************************/

#include "Texture.h"

/*********************************************************************
** Function: Texture
** Description: Base constructor for a Texture class object
** Parameters: none
*********************************************************************/
Texture::Texture() {}

/*********************************************************************
** Function: Texture
** Description: Constructor for a Texture class object
** Parameters: const char*, bool
** Post-Conditions: GLFW allocates memory for the texture
*********************************************************************/
Texture::Texture(const char* path, bool flip, bool alphaChannel)
{
	glGenTextures(1, &this->ID);
	glBindTexture(GL_TEXTURE_2D, this->ID);

	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// load image, create texture and generate mipmaps
	int width, height, nrChannels;
	
	if (flip)
		stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.

	//unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);
	unsigned char* data = stbi_load(path, &width, &height, &nrChannels, STBI_rgb_alpha);

	if (data)
	{
		if (alphaChannel)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		else
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}

	stbi_image_free(data);
}

/*********************************************************************
** Function: Window
** Description: Constructor for a Window class object
** Parameters: const char*, int, int, bool
** Post-Conditions: GLFW allocates memory for the texture
*********************************************************************/
Texture::Texture(const char* path, int textureWrap, int textureFilter, bool flip, bool alphaChannel)
{
	glGenTextures(1, &this->ID);
	glBindTexture(GL_TEXTURE_2D, this->ID);

	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, textureWrap);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, textureWrap);

	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, textureFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, textureFilter);

	// load image, create texture and generate mipmaps
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.

	//unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);
	unsigned char* data = stbi_load(path, &width, &height, &nrChannels, STBI_rgb_alpha);

	if (data)
	{
		if (alphaChannel)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		else
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}

	stbi_image_free(data);
}

/*********************************************************************
** Function: Use
** Description: GLFW binds the texture to the provided texture unit
** Parameters: const char*, bool
** Post-Conditions: The provided texture unit is binded
*********************************************************************/
void Texture::Use(int textureUnit)
{
	glActiveTexture(textureUnit);
	glBindTexture(GL_TEXTURE_2D, this->ID);
}

int id = std::rand() % 10000;
int Texture::SaveScreenshot(std::string path)
{
	stbi_flip_vertically_on_write(true);

	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);

	int x = viewport[0];
	int y = viewport[1];
	int width = viewport[2];
	int height = viewport[3];

	char* data = (char*)malloc((size_t)(width * height * 3)); // 3 components (R, G, B)

	if (!data)
		return 0;

	glPixelStorei(GL_PACK_ALIGNMENT, 1);
	glReadPixels(x, y, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);

	char buffer[32];
	_itoa_s(id++, buffer, 10);
	int saved = stbi_write_png((path + '/' + buffer + ".png").c_str(), width, height, 3, data, 0);

	free(data);

	return saved;
}