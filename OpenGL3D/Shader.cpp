#include "Shader.h"
#include <iostream>
#include <vector>

GLuint Shader::currentlyBoundID = 0;

Shader::Shader()
{
}

Shader::~Shader()
{
}

void Shader::load(const char* vert, const char* frag)
{
	//Read in the vertex and fragment shaders
	//We must delete these after we are finished compiling the shaders
	char* vv = read(vert);
	char* vf = nullptr;
	if (frag)
		vf = read(frag);

	//Setup the shader
	setup(vv, vf);

	//Delete the file data arrays we allocted
	delete[] vv;
	if (vf)
		delete[] vf;
}

void Shader::bind()
{
	if (m_ProgramID != currentlyBoundID)
	{
		glUseProgram(m_ProgramID);
		currentlyBoundID = m_ProgramID;
	}
}

void Shader::unbind()
{
	if (currentlyBoundID != 0)
	{
		glUseProgram(0);
		currentlyBoundID = 0;
	}
}

char* Shader::read(const char *filename)
{
	char* shaderFile = 0;

	//Open the file
	FILE* fp = fopen(filename, "rb");
	if (!fp)
	{
		std::cerr << "File doesn't exist [" << filename << "]" << std::endl;
		system("pause");
		std::exit(1);
	}

	//Obtain the file size
	fseek(fp, 0, SEEK_END);
	long size = ftell(fp);
	rewind(fp);

	//Alloc memory - will be deleted while setting the shader up
	shaderFile = new char[size + 1];

	//Copy the file to the shaderFile
	fread(shaderFile, sizeof(char), size, fp);
	shaderFile[size] = '\0'; //Eliminate the garbage at EOF
	fclose(fp);

	return shaderFile;
}

void Shader::setup(const char* vs, const char* fs)
{
	GLint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	GLint fragmentShader;
	if (fs)
		fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(vertexShader, 1, &vs, 0);
	if (fs)
		glShaderSource(fragmentShader, 1, &fs, 0);

	glCompileShader(vertexShader);
	if (fs)
		glCompileShader(fragmentShader);

	char glslLog[1024];
	GLsizei glslLogSize;

	//Get the error log for the Vertex shader
	glGetInfoLogARB(vertexShader, 1024, &glslLogSize, glslLog);
	if (glslLogSize)
		std::cerr << "Vertex program log: " << glslLog << std::endl;

	if (fs)
	{
		//Get the error log for the Fragment shader
		glGetInfoLogARB(fragmentShader, 1024, &glslLogSize, glslLog);
		if (glslLogSize)
			std::cerr << "Fragment program log: " << glslLog << std::endl;
	}

	m_ProgramID = glCreateProgram();

	glAttachShader(m_ProgramID, vertexShader);
	if (fs)
		glAttachShader(m_ProgramID, fragmentShader);

	glLinkProgram(m_ProgramID);
	GLint result;
	glGetProgramiv(m_ProgramID, GL_LINK_STATUS, &result);
	if (result == GL_FALSE)
	{
		GLint length;
		glGetProgramiv(m_ProgramID, GL_INFO_LOG_LENGTH, &length);
		std::vector<char> error(length);
		glGetProgramInfoLog(m_ProgramID, length, &length, &error[0]);
		std::cout << "Failed to link shaders!" << std::endl;
		printf("%s\n", &error[0]);
		assert(false);
	}

	glValidateProgram(m_ProgramID);
	glGetProgramiv(m_ProgramID, GL_VALIDATE_STATUS, &result);
	if (result == GL_FALSE)
	{
		GLint length;
		glGetProgramiv(m_ProgramID, GL_INFO_LOG_LENGTH, &length);
		std::vector<char> error(length);
		glGetProgramInfoLog(m_ProgramID, length, &length, &error[0]);
		std::cout << "Failed to link shaders!" << std::endl;
		printf("%s\n", &error[0]);
		assert(false);
	}

	glDeleteShader(vertexShader);
	if (fs)
		glDeleteShader(fragmentShader);
}

GLint Shader::lookup(const std::string& uniformName)
{
	auto it = m_UniformCache.find(uniformName);
	if (it == m_UniformCache.end())
	{
		//insert into cache
		m_UniformCache[uniformName] = glGetUniformLocation(m_ProgramID, uniformName.c_str());

		std::cout << "Caching (" << m_UniformCache[uniformName] << "): " << uniformName << std::endl;
	}

	return m_UniformCache[uniformName];
}

void Shader::setUniform1(const std::string& uniformName, float value)
{
	glUniform1f(lookup(uniformName), value);
}

void Shader::setUniform1(const std::string& uniformName, int value)
{
	glUniform1i(lookup(uniformName), value);
}

void Shader::setUniform1(const std::string& uniformName, unsigned int value)
{
	glUniform1i(lookup(uniformName), (int)value);
}

void Shader::setUniform2(const std::string& uniformName, const glm::vec2& values)
{
	glUniform2f(lookup(uniformName), values.x, values.y);
}

void Shader::setUniform3(const std::string& uniformName, const glm::vec3& values)
{
	glUniform3f(lookup(uniformName), values.x, values.y, values.z);
}

void Shader::setUniformMatrix4(const std::string& uniformName, const glm::mat4& values)
{
	glUniformMatrix4fv(lookup(uniformName), 1, GL_FALSE, &values[0][0]);
}

Shader* Shader::createRegularDefault()
{
	Shader* s = new Shader();
	s->load("Shaders/Regular.vert", "Shaders/Regular.frag");
	return s;
}

Shader* Shader::createSkyboxDefault()
{
	Shader* s = new Shader();
	s->load("Shaders/Skybox.vert", "Shaders/Skybox.frag");
	return s;
}

Shader* Shader::createTerrainDefault()
{
	Shader* s = new Shader();
	s->load("Shaders/Terrain.vert", "Shaders/Terrain.frag");
	return s;
}