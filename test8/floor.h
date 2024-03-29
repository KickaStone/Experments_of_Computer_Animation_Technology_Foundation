#include <iostream>
#include <ctime>
#include <cmath>
#include <glm/glm.hpp>
#include "shader.h"

class Floor {
private:

	GLuint VBO, VAO;
	Shader *shader;

public:
	Floor();

	~Floor();

	void render(glm::mat4 model, glm::mat4 view, glm::mat4 projection);

};