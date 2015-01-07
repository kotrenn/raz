varying float LightIntensity;
varying vec2 ST;
varying vec3 Loc;

uniform float Zoom;
uniform int MaxIters;
uniform vec3 ConvergeColor;
uniform vec3 DivergeColor1;
uniform vec3 DivergeColor2;
uniform vec3 HitPos;
uniform float Radius;

void main()
{
	float Limit = 4.0;
	float CS = 1.0;
	
	float real = ST.s * Zoom - 0.743643;
	float imag = ST.t * Zoom + 0.131825;
	float real0 = real;
	float imag0 = imag;
	float newr = 0.0;
	int numIters;

	for (numIters = 0; numIters < MaxIters; numIters++)
	{
		float newreal = real0 + real * real - imag * imag;
		float newimag = imag0 + 2. * real * imag;
		newr = newreal * newreal + newimag * newimag;
		if (newr >= Limit)
			break;
		real = newreal;
		imag = newimag;
	}

	vec3 color = vec3(0., 0., 0.);
	
	if (newr < Limit)
		//color = ConvergeColor;
	    discard;
	else
		color = mix(DivergeColor1, DivergeColor2,
		            fract(float(numIters) * 0.2));

	float dist = distance(Loc, HitPos);
	if (dist <= Radius && dist >= Radius - 0.2)
		color = ConvergeColor;

	color.rgb *= LightIntensity;
	gl_FragColor = vec4(color, 1.0);

	/*
	  vec3 col = gl_FragCoord.xyz;
	  gl_FragColor = clamp(vec4(col, 1.0), 0.0, 1.0);
	*/
}
