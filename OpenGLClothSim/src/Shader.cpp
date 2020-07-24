#include "Shader.h"

	Shader::Shader(const char* vertexPath, const char* fragmentPath) {
		readShaderFiles(vertexPath, fragmentPath);
		compileShaders();
		createProgram();
	}

	Shader::~Shader()
	{
		glDeleteProgram(programID);
	}

	void Shader::readShaderFiles(const char* vertexPath, const char* fragmentPath) 
	{
		std::string vertexSource;
		std::string fragmentSource;
		std::ifstream vertexFile;
		std::ifstream fragmentFile;

		std::stringstream vStringstream, fStringstream;

		vertexFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fragmentFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

		try {
			vertexFile.open(vertexPath);
			fragmentFile.open(fragmentPath);

			vStringstream << vertexFile.rdbuf();   //rdbuf returns pointer to the underlying basic_streambuf object
			fStringstream << fragmentFile.rdbuf();

			vertexFile.close();
			fragmentFile.close();

			vertexSource = vStringstream.str();
			fragmentSource = fStringstream.str();
		}
		catch (std::ifstream::failure e)
		{
			std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
		}

		const char* vShaderCode = vertexSource.c_str();
		const char* fShaderCode = fragmentSource.c_str();

		vfSource = { vStringstream.str(), fStringstream.str() };
	}

	void Shader::compileShaders() {
		int success;
		char infoLog[512];
		const char* src;

		//~~~~Vertex~~~~
		vertexShader = glCreateShader(GL_VERTEX_SHADER);
		src = vfSource.VertexSource.c_str();
		glShaderSource(vertexShader, 1, &src, NULL);
		glCompileShader(vertexShader);

		//check for errors
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		}

		//~~~~Fragment~~~~
		fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		src = vfSource.FragmentSource.c_str();
		glShaderSource(fragmentShader, 1, &src, NULL);
		glCompileShader(fragmentShader);

		//check for errors
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
		}
	}

	void Shader::createProgram() 
	{
		int success;
		char infoLog[512];

		programID = glCreateProgram();
		glAttachShader(programID, vertexShader);
		glAttachShader(programID, fragmentShader);
		glLinkProgram(programID);

		glGetProgramiv(programID, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(programID, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		}

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
	}

	void Shader::use()
	{
		glUseProgram(programID);
	}

	void Shader::setBool(const std::string& name, bool value) const
	{
		glUniform1i(glGetUniformLocation(programID, name.c_str()), (int)value);
	}

	void Shader::setInt(const std::string& name, int value) const 
	{
		glUniform1i(glGetUniformLocation(programID, name.c_str()), value);
	}

	void Shader::setFloat(const std::string& name, float value) const 
	{
		glUniform1f(glGetUniformLocation(programID, name.c_str()), value);
	}

	void Shader::setMatrix4fv(const std::string& name, float* value) const
	{
		glUniformMatrix4fv(glGetUniformLocation(programID, name.c_str()), 1, GL_FALSE, value);
	}

	void Shader::setVec3(const std::string& name, glm::vec3 vector) const
	{
		glUniform3fv(glGetUniformLocation(programID, name.c_str()), 1, glm::value_ptr(vector));
	}