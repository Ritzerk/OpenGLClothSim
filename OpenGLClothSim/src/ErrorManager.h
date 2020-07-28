#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

//let us break on the exact line, and then return useful information to the console using LGLogCall
#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) GLClearError(); x; ASSERT(GLLogCall(#x, __FILE__, __LINE__))      


//Error Mangament functions...
void GLClearError();

bool GLLogCall(const char* function, const char* file, int line);

std::ostream& operator<< (std::ostream & out, const glm::vec3 & vec);