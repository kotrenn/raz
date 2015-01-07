varying vec3 Normal;
varying vec4 Pos;

varying vec2 ST;

void main()
{
	/* mand stuff */
	ST = 2.0 * (gl_MultiTexCoord0.st - 0.5);

	/* vert stuff */
	Pos = gl_ModelViewMatrix * gl_Vertex;
	Normal = normalize(gl_NormalMatrix * gl_Normal);
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}
