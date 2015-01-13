#include "glslprogram.h"

void GLSLProgram::Create(const char *vfile, const char *gfile, const char *ffile)
{
	CanDoFragmentShader = IsExtensionSupported("GL_ARB_fragment_shader");
	CanDoGeometryShader = IsExtensionSupported("GL_EXT_geometry_shader4");
	CanDoVertexShader = IsExtensionSupported("GL_ARB_vertex_shader");

	InputTopology = GL_TRIANGLES;
	OutputTopology = GL_TRIANGLE_STRIP;

	Vshader = Gshader = Fshader = 0;
	Program = 0;
	AttributeLocs.clear();
	UniformLocs.clear();
	Verbose = false;

	this->Program = glCreateProgram();
	CheckGlErrors("glCreateProgram");

	if (vfile != NULL && vfile[0] != '\0')
	{
		if (!CanDoVertexShader)
		{
			fprintf(stderr,
			        "Warning: this system cannot handle vertex shaders\n");
		}
		this->Vshader = LoadVertexShader(vfile);
		int status = CompileShader(this->Vshader);
		if (status != 0)
		{
			if (this->Verbose)
				fprintf(stderr, "Shader '%s' compiled.\n", vfile);
			AttachShader(this->Vshader);
		}
		else
		{
			fprintf(stderr, "Shader '%s' did not compile.\n", vfile);
		}
	}

	if (gfile != NULL && gfile[0] != '\0')
	{
		if (!CanDoGeometryShader)
		{
			fprintf(stderr,
			        "Warning: this system cannot handle geometry shaders\n");
		}
		this->Gshader = LoadGeometryShader(vfile);
		int status = CompileShader(this->Gshader);
		if (status != 0)
		{
			if (this->Verbose)
				fprintf(stderr, "Shader '%s' compiled.\n", gfile);
			AttachShader(Gshader);
		}
		else
		{
			fprintf(stderr, "Shader '%s' did not compile.\n", gfile);
		}
	}

	if (ffile != NULL && ffile[0] != '\0')
	{
		if (!CanDoFragmentShader)
		{
			fprintf(stderr,
			        "Warning: this system cannot handle fragment shaders\n");
		}
		this->Fshader = LoadFragmentShader(ffile);
		int status = CompileShader(this->Fshader);
		if (status != 0)
		{
			if (this->Verbose)
				fprintf(stderr, "Shader '%s' compiled.\n", ffile);
			AttachShader(Fshader);
		}
		else
		{
			fprintf(stderr, "Shader '%s' did not compile.\n", ffile);
		}
	}

	LinkProgram();
}

GLSLProgram::GLSLProgram()
{
	Create(NULL, NULL, NULL);
}

GLSLProgram::GLSLProgram(const char *vfile, const char *gfile, const char *ffile)
{
	Create(vfile, gfile, ffile);
}

GLSLProgram::GLSLProgram(const char *vfile, const char *ffile)
{
	fflush(stdout);
	Create(vfile, NULL, ffile);
}

GLuint GLSLProgram::LoadVertexShader(const char *vfile)
{
	GLuint shader = glCreateShader(GL_VERTEX_SHADER);
	if (LoadShader(vfile, shader) != 0)
	{
		if (this->Verbose)
			fprintf(stderr, "Vertex shader '%s' loaded.\n", vfile);
		return shader;
	}
	else
	{
		fprintf(stderr, "Vertex shader '%s' failed to load.\n", vfile);
		glDeleteShader(shader);
		return 0;
	}
}

GLuint GLSLProgram::LoadGeometryShader(const char *gfile)
{
	GLuint shader = glCreateShader(GL_GEOMETRY_SHADER_EXT);
	if (LoadShader(gfile, shader) != 0)
	{
		if (this->Verbose)
			fprintf(stderr, "Geometry shader '%s' loaded.\n", gfile);
		return shader;
	}
	else
	{
		fprintf(stderr, "Geometry shader '%s' failed to load.\n", gfile);
		glDeleteShader(shader);
		return 0;
	}
}

GLuint GLSLProgram::LoadFragmentShader(const char *ffile)
{
	GLuint shader = glCreateShader(GL_FRAGMENT_SHADER);
	if (LoadShader(ffile, shader) != 0)
	{
		if (this->Verbose)
			fprintf(stderr, "Fragment shader '%s' loaded.\n", ffile);
		return shader;
	}
	else
	{
		fprintf(stderr, "Fragment shader '%s' failed to load.\n", ffile);
		glDeleteShader(shader);
		return 0;
	}
}

int GLSLProgram::LoadShader(const char *fname, GLuint shader)
{
	FILE *in = fopen(fname, "rb");
	if (in == NULL)
	{
		fprintf(stderr, "Cannot open shader file '%s'\n", fname);
		return 0;
	}

	fseek(in, 0, SEEK_END);
	int length = ftell(in);
	fseek(in, 0, SEEK_SET);

	GLubyte *buf = new GLubyte[length + 1];
	fread(buf, sizeof(GLubyte), length, in);
	buf[length] = '\0';
	fclose(in);

	glShaderSource(shader, 1, (const char **)&buf, NULL);
	delete [] buf;
	CheckGlErrors("LoadShader:ShaderSource");
	return 1;
}

int GLSLProgram::CompileShader(GLuint shader)
{
	glCompileShader(shader);
	GLint infoLogLen;
	GLint compileStatus;
	CheckGlErrors("CompileShader:");
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compileStatus);

	if (compileStatus == 0)
	{
		fprintf(stderr, "Failed to compile shader.\n");
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLen);

		if (infoLogLen > 0)
		{
			GLchar *infoLog = new GLchar[infoLogLen + 1];
			glGetShaderInfoLog(shader, infoLogLen, NULL, infoLog);
			infoLog[infoLogLen] = '\0';
			FILE *file = fopen("glsllog.txt", "w");
			fprintf(file, "\n%s\n", infoLog);
			fprintf(stderr, "\n%s\n", infoLog);
			fclose(file);
			delete [] infoLog;
		}
		glDeleteShader(shader);
		return 0;
	}

	CheckGlErrors("LoadShader:Compile 2");
	return 1;
}

void GLSLProgram::AttachShader(GLuint shader)
{
	glAttachShader(this->Program, shader);
}

int GLSLProgram::LinkProgram()
{
	if (Gshader != 0)
	{
		glProgramParameteriEXT(Program,
		                       GL_GEOMETRY_INPUT_TYPE_EXT,
		                       InputTopology);
		glProgramParameteriEXT(Program,
		                       GL_GEOMETRY_OUTPUT_TYPE_EXT,
		                       OutputTopology);
		glProgramParameteriEXT(Program,
		                       GL_GEOMETRY_VERTICES_OUT_EXT,
		                       1024);
	}

	glLinkProgram(Program);
	CheckGlErrors("LoadShader:Link 1");

	GLchar *infoLog;
	GLint infoLogLen;
	GLint linkStatus;
	glGetProgramiv(this->Program, GL_LINK_STATUS, &linkStatus);
	CheckGlErrors("LoadShader:Link 2");

	if (linkStatus == 0)
	{
		glGetProgramiv(this->Program, GL_INFO_LOG_LENGTH, &infoLogLen);
		fprintf(stderr, "Failed to link program--Info LogLength = %d\n",
		        infoLogLen);
		if (infoLogLen > 0)
		{
			infoLog = new GLchar[infoLogLen + 1];
			glGetProgramInfoLog(this->Program, infoLogLen,
			                    NULL, infoLog);
			infoLog[infoLogLen] = '\0';
			fprintf(stderr, "Info Log:\n%s\n", infoLog);
			delete [] infoLog;
		}
		glDeleteProgram(Program);
		glDeleteShader(Vshader);
		glDeleteShader(Gshader);
		glDeleteShader(Fshader);
		return 0;
	}

	return 1;
}

void GLSLProgram::SetVerbose(bool v)
{
	Verbose = v;
}

void GLSLProgram::Use()
{
	Use(this->Program);
}

void GLSLProgram::Use(int p)
{
	if (p != CurrentProgram)
	{
		glUseProgram(p);
		CurrentProgram = p;
	}
}

int GLSLProgram::GetAttributeLocation(const char *name)
{
	map<const char *, int>::iterator pos;

	pos = AttributeLocs.find(name);
	if (pos == AttributeLocs.end())
	{
		AttributeLocs[name] = glGetAttribLocation(this->Program, name);
	}

	return AttributeLocs[name];
}

void GLSLProgram::SetAttribute(const char *name, int val)
{
	int loc;
	if ((loc = GetAttributeLocation(name)) >= 0)
	{
		this->Use();
		glUniform1i(loc, val);
	}
}

void GLSLProgram::SetAttribute(const char *name, float val)
{
	int loc;
	if ((loc = GetAttributeLocation(name)) >= 0)
	{
		this->Use();
		glUniform1f(loc, val);
	}
}

int GLSLProgram::GetUniformLocation(const char *name)
{
	map<const char *, int>::iterator pos;
	pos = UniformLocs.find(name);
	if (pos == UniformLocs.end())
	{
		UniformLocs[name] = glGetUniformLocation(this->Program, name);
	}

	return UniformLocs[name];
}

void GLSLProgram::SetUniform(const char *name, int val)
{
	int loc;
	if ((loc = GetUniformLocation(name)) >= 0)
	{
		this->Use();
		glUniform1i(loc, val);
	}
}

void GLSLProgram::SetUniform(const char *name, float val)
{
	int loc;
	if ((loc = GetUniformLocation(name)) >= 0)
	{
		this->Use();
		glUniform1f(loc, val);
	}
}

void GLSLProgram::SetUniform(const char *name, float val1, float val2, float val3)
{
	int loc;
	if ((loc = GetUniformLocation(name)) >= 0)
	{
		this->Use();
		glUniform3f(loc, val1, val2, val3);
	}
}

void GLSLProgram::SetInputTopology(GLenum t)
{
	if (t != GL_POINTS &&
	    t != GL_LINES &&
	    t != GL_LINES_ADJACENCY_EXT &&
	    t != GL_TRIANGLES &&
	    t != GL_TRIANGLES_ADJACENCY_EXT)
	{
		fprintf(stderr,
		        "Warning: You have not specified a supported "
		        "Input Topology\n");
	}
	InputTopology = t;
}

void GLSLProgram::SetOutputTopology(GLenum t)
{
	if (t != GL_POINTS &&
	    t != GL_LINE_STRIP &&
	    t != GL_TRIANGLE_STRIP)
	{
		fprintf(stderr,
		        "Warning: You have not specified a supported "
		        "Output Topology\n");
	}
	OutputTopology = t;
}

bool GLSLProgram::IsExtensionSupported(const char *extension)
{
	if (extension == NULL || extension[0] == '\0')
		return false;

	GLubyte *where = (GLubyte *)strchr(extension, ' ');
	if (where != 0)
		return false;

	const GLubyte *extensions = glGetString(GL_EXTENSIONS);

	for (const GLubyte *start = extensions; ; )
	{
		where = (GLubyte *)strstr((const char *)start, extension);
		if (where == 0)
			return false;

		GLubyte *terminator = where + strlen(extension);

		if (where == start ||
		    *(where - 1) == '\n' ||
		    *(where - 1) == ' ')
			if (*terminator == ' ' ||
			    *terminator == '\n' ||
			    *terminator == '\0')
				return true;
		start = terminator;
	}
	return false;
}

int GLSLProgram::CurrentProgram = 0;



void CheckGlErrors(const char *caller)
{
	unsigned int gle = glGetError();

	if (gle != GL_NO_ERROR)
	{
		fprintf(stderr, "GL Error discovered from caller %s:", caller);
		switch(gle)
		{
		case GL_INVALID_ENUM:
			fprintf(stderr, "Invalid enum.\n");
			break;
		case GL_INVALID_VALUE:
			fprintf(stderr, "Invalid value.\n");
			break;
		case GL_INVALID_OPERATION:
			fprintf(stderr, "Invalid operation.\n");
			break;
		case GL_STACK_OVERFLOW:
			fprintf(stderr, "Stack overflow\n");
			break;
		case GL_STACK_UNDERFLOW:
			fprintf(stderr, "Stack underflow\n");
			break;
		case GL_OUT_OF_MEMORY:
			fprintf(stderr, "Out of memory.\n");
			break;
		case GL_INVALID_FRAMEBUFFER_OPERATION:
			fprintf(stderr, "Framebuffer object is not complete.\n");
			break;
		}
	}
}
