#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include <iostream>
#include <string>
#include <fstream>

#include "maths/Matrix4.h"
#include "maths/Vector2.h"
#include "maths/Vector4.h"
#include "maths/Matrix4Row.h"

// General purpose shader object. Compiles from file, generates
// compile/link-time error messages and hosts several utility 
// functions for easy management.
class Shader
{
public:
	// Program id
	GLuint id;

	// Constructor
	Shader() { }

	// Sets the current shader as active
	Shader& use();

	// Compiles the shader from given source code
	void compile(
		const GLchar *vertexSource,
		const GLchar *fragmentSource,
        const GLchar *tessControlSource = nullptr,
        const GLchar *tessEvalSource = nullptr,
		const GLchar *geometrySource = nullptr
	);

	bool isValid() { return isValid(id); }

	// Utility functions
	void setFloat(const GLchar *name, GLfloat value);
	void setInteger(const GLchar *name, GLint value);
	void setVector2f(const GLchar *name, GLfloat x, GLfloat y);
	void setVector2f(const GLchar *name, const Vector2 &value);
	void setVector3f(const GLchar *name, GLfloat x, GLfloat y, GLfloat z);
	void setVector3f(const GLchar *name, const Vector3 &value);
	void setVector4f(const GLchar *name, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
	void setVector4f(const GLchar *name, const Vector4 &value);
	void setMatrix4(const GLchar *name, const Matrix4 &matrix);
	void setMatrix4Row(const GLchar *name, const Matrix4Row &matrix);

private:
	// Checks if compilation or linking failed and if so, print the error logs
	void checkCompileErrors(GLuint object, std::string type);

	GLuint vs;
    GLuint fs;
    GLuint tcs;
    GLuint tes;
    GLuint gs;

    void compileVertexShader(const GLchar *vertexSource);
    void compileFragmentShader(const GLchar *fragmentSource);
    bool compileTessControlShader(const GLchar *tessControlSource);
    bool compileTessEvalShader(const GLchar *tessEvalSource);
    bool compileGeometryShader(const GLchar *geometrySource);
    void createShaderProgram(bool tessShadersExist, bool geometryShaderExists);

    void checkShaderErrors(GLuint shader, std::string shaderType);
    void printShaderInfoLog(GLuint shaderIndex);
    void printProgramInfoLog(GLuint programId);
    const char *GLTypeToString(GLenum type);
    void printAllParams(GLuint programId);
    bool isValid(GLuint programId);
};

#endif
