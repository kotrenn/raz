varying vec3 norml;
varying vec4 pos;

vec3 ADSLightModel(in vec3 myNormal, in vec4 myPosition)
{
    gl_FrontMaterial.diffuse;
    vec3 norm = normalize(myNormal);
    vec3 lightv = normalize(myLightPosition.xyz - myPosition.xyz);
    vec3 viewv = -normalize(myPosition.xyz);
    vec3 refl = reflect(-lightv, norm);

    vec4 diffuse = max(0.0, dot(lightv, norm)) * myMaterialDiffuse * myLightDiffuse;

    vec4 ambient = myMaterialAmbient * myLightAmbient;
    vec4 specular = vec4(0.0, 0.0, 0.0, 1.0);
    if (dot(lightv, viewv) > 0.0)
    {
        specular = pow(max(0.0, dot(viewv, refl)),
                       myMaterialShininess) * myMaterialSpecular * myLightSpecular;
    }
    return clamp(vec3(ambient + diffuse + specular), 0.0, 1.0);
}

void main()
{
    vec3 color = ADSLightModel(norml, pos);
    gl_FragColor = vec4(color, 1.0);
}