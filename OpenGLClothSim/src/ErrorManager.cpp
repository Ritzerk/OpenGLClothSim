
#include "ErrorManager.h"

//Error Mangament functions...
void GLClearError() {
    while (glGetError() != GL_NO_ERROR);        //If its GL_NO_ERROR then all errors have been found. the glGetError only returns one flag at a time. 

}

bool GLLogCall(const char* function, const char* file, int line) {
    while (GLenum error = glGetError()) {
        std::cout << "[OpenGL Error] (" << error << "): " << function << " " << file << " " << line << std::endl;
        return false;
    }
    return true;
}
 