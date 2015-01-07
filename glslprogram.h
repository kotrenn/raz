/* GLSL Program class for simplifying GLSL shaders.
   From "Graphics Shaders:  Theory and Practice" 3rd Edition
   By Mike Bailey and Steve Cunningham
*/

#ifndef __GLSL_PROGRAM_H
#define __GLSL_PROGRAM_H

#include "common.h"

void CheckGlErrors(const char *);

inline int GetLocal(int flag)
{
	int i;
	glGetIntegerv(flag, &i);
	return i;
}

class GLSLProgram
{
private:
	map<char *, int> AttributeLocs;
	char *Ffile;
	unsigned int Fshader;
	char *Gfile;
	unsigned int Gshader;
	GLenum InputTopology;
	GLenum OutputTopology;
	int Program;
	char *Vfile;
	unsigned int Vshader;
	map<char *, int> UniformLocs;
	bool Verbose;

	static int CurrentProgram;

	void AttachShader(GLuint);
	bool CanDoFragmentShader;
	bool CanDoGeometryShader;
	bool CanDoVertexShader;
	int CompileShader(GLuint);
	void Create(char *, char *, char *);
	int GetAttributeLocation(char *);
	int GetUniformLocation(char *);
	int LinkProgram();
	GLuint LoadFragmentShader(char *);
	GLuint LoadGeometryShader(char *);
	GLuint LoadVertexShader(char *);
	int LoadShader(const char *, GLuint);

public:
	GLSLProgram();
	GLSLProgram(char *, char *, char *);
	GLSLProgram(char *, char *);

	bool IsExtensionSupported(const char *);
	void SetAttribute(char *, int);
	void SetAttribute(char *, float);
	void SetInputTopology(GLenum);
	void SetOutputTopology(GLenum);
	void SetUniform(char *, int);
	void SetUniform(char *, float);
	void SetUniform(char *name, float val1, float val2, float val3);
	void SetVerbose(bool);
	void Use();
	void Use(int);

	static void UseFixedFunction();
};

#endif
