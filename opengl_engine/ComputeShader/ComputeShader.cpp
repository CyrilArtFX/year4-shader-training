#include "ComputeShader.h"

#include <Maths/Vector3.h>
#include <fstream>
#include <sstream>
#include <iostream>

//  TODO for later implementation of compute shaders in the real engine :
//  make a base class for shaders and compute shaders to prevent code replication (use, delete program and all the set uniforms)

ComputeShader::ComputeShader(const char* path)
{
	//  Step 1 : retrieve the shader source code from filePath
	//  ======================================================

	std::string computeCode;
	std::ifstream cShaderFile;

	//  ensure ifstream object can throw exceptions
	cShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try
	{
		//  open file
		cShaderFile.open(path);
		std::stringstream cShaderStream;

		//  read file's buffer contents into streams
		cShaderStream << cShaderFile.rdbuf();

		//  close file handler
		cShaderFile.close();

		//  convert stream into string
		computeCode = cShaderStream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}

	const char* cShaderCode = computeCode.c_str();


	//  Step 2 : compile shader and link it into the program
	//  ====================================================

	unsigned int compute;
	int success;
	char infoLog[512];

	//  compute shader
	compute = glCreateShader(GL_COMPUTE_SHADER);
	glShaderSource(compute, 1, &cShaderCode, NULL);
	glCompileShader(compute);

	glGetShaderiv(compute, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(compute, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::COMPUTE::COMPILATION_FAILED\n" << infoLog << std::endl;
	} //  check if compute shader has correctly compiled


	//  shader program
	ID = glCreateProgram();
	glAttachShader(ID, compute);
	glLinkProgram(ID); 
	
	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(ID, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	} //  check if shader program correctly linked shaders

	//  delete shader once it's link into the program
	glDeleteShader(compute);
}

void ComputeShader::use()
{
	glUseProgram(ID);
}

void ComputeShader::deleteProgram()
{
	glDeleteProgram(ID);
}

void ComputeShader::setBool(const std::string& name, bool value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void ComputeShader::setInt(const std::string& name, int value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void ComputeShader::setFloat(const std::string& name, float value) const
{
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void ComputeShader::setVec3(const std::string& name, Vector3 value) const
{
	setVec3(name, value.x, value.y, value.z);
}

void ComputeShader::setVec3(const std::string& name, float xValue, float yValue, float zValue) const
{
	glUniform3f(glGetUniformLocation(ID, name.c_str()), xValue, yValue, zValue);
}

void ComputeShader::setMatrix4(const std::string& name, const float* value) const
{
	glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_TRUE, value);
}
