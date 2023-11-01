#include "texture.h"

Texture::Texture(const char* texturePath, unsigned int glFormat, bool flipVertical)
{
	//  create texture
	glGenTextures(1, &ID);
	glBindTexture(GL_TEXTURE_2D, ID);

	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(flipVertical);
	unsigned char* data = stbi_load(texturePath, &width, &height, &nrChannels, 0);

	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, glFormat, width, height, 0, glFormat, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture at path " << texturePath << std::endl;

		stbi_set_flip_vertically_on_load(false);
		data = stbi_load("Resources/Default/notexture.png", &width, &height, &nrChannels, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	stbi_image_free(data);
}


void Texture::use()
{
	glBindTexture(GL_TEXTURE_2D, ID);
}

void Texture::setWrappingParameters(unsigned int sAxis, unsigned int tAxis)
{
	use();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, sAxis);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, tAxis);
}

void Texture::setFilteringParameters(unsigned int minifying, unsigned int magnifying)
{
	use();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minifying);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magnifying);
}
