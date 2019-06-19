
#ifndef GLEW_STATIC
#define GLEW_STATIC
#endif // !GLEW_STATIC


#define GLEW_STATIC

#include <GL\glew.h>
#include <iostream>
#include <string>
#include "stb_image.h"

using namespace std;

class CTexture {
public:
	GLuint textureID;

	CTexture();

	~CTexture();

	void loadTexture(const char* path, bool constrain = false);
};