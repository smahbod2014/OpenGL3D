#include "Shader.h"
#include <iostream>


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
	glUseProgram(m_ProgramID);
}

void Shader::unbind()
{
	glUseProgram(0);
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
	glValidateProgram(m_ProgramID);
	glDeleteShader(vertexShader);
	if (fs)
		glDeleteShader(fragmentShader);
}

void Shader::setUniform1(const char* uniformName, float value)
{
	auto it = m_UniformCache.find(uniformName);
	if (it == m_UniformCache.end())
	{
		//insert into cache
		m_UniformCache[uniformName] = glGetUniformLocation(m_ProgramID, uniformName);
	}

	glUniform1f(m_UniformCache[uniformName], value);
}

void Shader::setUniform1(const char* uniformName, int value)
{
	auto it = m_UniformCache.find(uniformName);
	if (it == m_UniformCache.end())
	{
		//insert into cache
		m_UniformCache[uniformName] = glGetUniformLocation(m_ProgramID, uniformName);
	}

	glUniform1i(m_UniformCache[uniformName], value);
}

void Shader::setUniform3(const char* uniformName, const glm::vec3& values)
{
	auto it = m_UniformCache.find(uniformName);
	if (it == m_UniformCache.end())
	{
		//insert into cache
		m_UniformCache[uniformName] = glGetUniformLocation(m_ProgramID, uniformName);
	}

	glUniform3f(m_UniformCache[uniformName], values.x, values.y, values.z);
}

void Shader::setUniformMatrix4(const char* uniformName, const glm::mat4& values)
{
	auto it = m_UniformCache.find(uniformName);
	if (it == m_UniformCache.end())
	{
		//insert into cache
		m_UniformCache[uniformName] = glGetUniformLocation(m_ProgramID, uniformName);
	}

	glUniformMatrix4fv(m_UniformCache[uniformName], 1, GL_FALSE, &values[0][0]);
}