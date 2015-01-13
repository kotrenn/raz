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
	map<const char *, int> AttributeLocs;
	const char *Ffile;
	unsigned int Fshader;
	const char *Gfile;
	unsigned int Gshader;
	GLenum InputTopology;
	GLenum OutputTopology;
	int Program;
	const char *Vfile;
	unsigned int Vshader;
	map<const char *, int> UniformLocs;
	bool Verbose;

	static int CurrentProgram;

	void AttachShader(GLuint);
	bool CanDoFragmentShader;
	bool CanDoGeometryShader;
	bool CanDoVertexShader;
	int CompileShader(GLuint);
	void Create(const char *, const char *, const char *);
	int GetAttributeLocation(const char *);
	int GetUniformLocation(const char *);
	int LinkProgram();
	GLuint LoadFragmentShader(const char *);
	GLuint LoadGeometryShader(const char *);
	GLuint LoadVertexShader(const char *);
	int LoadShader(const char *, GLuint);

public:
	GLSLProgram();
	GLSLProgram(const char *, const char *, const char *);
	GLSLProgram(const char *, const char *);

	bool IsExtensionSupported(const char *);
	void SetAttribute(const char *, int);
	void SetAttribute(const char *, float);
	void SetInputTopology(GLenum);
	void SetOutputTopology(GLenum);
	void SetUniform(const char *, int);
	void SetUniform(const char *, float);
	void SetUniform(const char *name, float val1, float val2, float val3);
	void SetVerbose(bool);
	void Use();
	void Use(int);

	static void UseFixedFunction();
};

#endif
