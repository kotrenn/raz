uniform vec3 LightPosition;

varying float LightIntensity;
varying vec2 ST;

void main()
{
	vec3 ECposition = vec3(gl_ModelViewMatrix * gl_Vertex);
	vec3 tnorm = normalize(gl_NormalMatrix * gl_Normal);
	vec3 lightVec = normalize(LightPosition - ECposition);

	LightIntensity = abs(dot(lightVec, tnorm));
	if (LightIntensity < .2)
		LightIntensity = .2;

	ST = 2.0 * (gl_MultiTexCoord0.st - 0.5);
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}
