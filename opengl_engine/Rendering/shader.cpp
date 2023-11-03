#include "shader.h"

#include <Maths/Vector3.h>

//  TODO for later implementation of tessellation and geometry shaders in the real engine :
//  do 5 functions for each shader (that handle the null) and call them in the constructor (instead of doing 5161 if conditions)
//  and compile the shader program according to the shaders that are present
//  also it will be necessary to reimplement opengl and glad in the project with a 4.x version (here I used the 4.5)
//  also the shader check part for each shader can be put in a function

Shader::Shader(const char* vertexPath, const char* fragmentPath, const char* tesselationControlPath, const char* tesselationEvaluationPath)
{
	bool has_tess = !(tesselationControlPath == "" && tesselationEvaluationPath == "");

	//  Step 1 : retrieve the shaders source code from filePaths
	//  ========================================================

	std::string vertexCode;
	std::string tescCode;
	std::string teseCode;
	std::string fragmentCode;
	std::ifstream vShaderFile;
	std::ifstream tscShaderFile;
	std::ifstream tseShaderFile;
	std::ifstream fShaderFile;

	//  ensure ifstream objects can throw exceptions
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	tscShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	tseShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try
	{
		//  open files
		vShaderFile.open(vertexPath);
		if (has_tess)
		{
			tscShaderFile.open(tesselationControlPath);
			tseShaderFile.open(tesselationEvaluationPath);
		}
		fShaderFile.open(fragmentPath);
		std::stringstream vShaderStream, tscShaderStream, tseShaderStream, fShaderStream;

		//  read files's buffer contents into streams
		vShaderStream << vShaderFile.rdbuf();
		if (has_tess)
		{
			tscShaderStream << tscShaderFile.rdbuf();
			tseShaderStream << tseShaderFile.rdbuf();
		}
		fShaderStream << fShaderFile.rdbuf();

		//  close file handlers
		vShaderFile.close();
		if (has_tess)
		{
			tscShaderFile.close();
			tseShaderFile.close();
		}
		fShaderFile.close();

		//  convert stream into string
		vertexCode = vShaderStream.str();
		if (has_tess)
		{
			tescCode = tscShaderStream.str();
			teseCode = tseShaderStream.str();
		}
		fragmentCode = fShaderStream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}

	const char* vShaderCode = vertexCode.c_str();
	const char* tscShaderCode = tescCode.c_str();
	const char* tseShaderCode = teseCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();


	//  Step 2 : compile shaders and link them into the program
	//  =======================================================

	unsigned int vertex, tesc, tese, fragment;
	int success;
	char infoLog[512];

	//  vertex shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);

	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	} //  check if vertex shader has correctly compiled

	if (has_tess)
	{
		//  tessellation control shader
		tesc = glCreateShader(GL_TESS_CONTROL_SHADER);
		glShaderSource(tesc, 1, &tscShaderCode, NULL);
		glCompileShader(tesc);

		glGetShaderiv(tesc, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(tesc, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::TESSELLATION_CONTROL::COMPILATION_FAILED\n" << infoLog << std::endl;
		} //  check if tessellation control shader has correctly compiled

		//  tessellation evaluation shader
		tese = glCreateShader(GL_TESS_EVALUATION_SHADER);
		glShaderSource(tese, 1, &tseShaderCode, NULL);
		glCompileShader(tese);

		glGetShaderiv(tese, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(tese, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::TESSELLATION_EVALUATION::COMPILATION_FAILED\n" << infoLog << std::endl;
		} //  check if tessellation evaluation shader has correctly compiled
	}

	//  fragment shader
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);

	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragment, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	} //  check if fragment shader has correctly compiled


	//  shader program
	ID = glCreateProgram();
	glAttachShader(ID, vertex);
	if (has_tess)
	{
		glAttachShader(ID, tesc);
		glAttachShader(ID, tese);
	}
	glAttachShader(ID, fragment);
	glLinkProgram(ID);

	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(ID, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	} //  check if shader program correctly linked shaders

	//  delete shaders once they're link into the program
	glDeleteShader(vertex);
	if (has_tess)
	{
		glDeleteShader(tesc);
		glDeleteShader(tese);
	}
	glDeleteShader(fragment);
}


void Shader::use()
{
	glUseProgram(ID);
}

void Shader::deleteProgram()
{
	glDeleteProgram(ID);
}


void Shader::setBool(const std::string& name, bool value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::setInt(const std::string& name, int value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const std::string& name, float value) const
{
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setVec3(const std::string& name, Vector3 value) const
{
	setVec3(name, value.x, value.y, value.z);
}

void Shader::setVec3(const std::string& name, float xValue, float yValue, float zValue) const
{
	glUniform3f(glGetUniformLocation(ID, name.c_str()), xValue, yValue, zValue);
}

void Shader::setMatrix4(const std::string& name, const float* value) const
{
	glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_TRUE, value);
}
