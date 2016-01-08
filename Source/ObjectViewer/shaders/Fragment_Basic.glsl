#version 430 core

/// Normal Mapping Shader Example
/// Arnaud Masserann (Jan 3, 2013). Tutorial 13, Normal Mapping [online].
/// [Accessed 2015]. Available from: <https://github.com/opengl-tutorials/ogl/tree/master/tutorial13_normal_mapping>.

//Vertex inputs from fragment shader
in vec3 vPosition;
in vec2 vUV;
in vec3 vNormal;

//Lighting inputs from fragment shader
in vec3 csEyeDirection;
in vec3 csLightDirection;

//Output color to screen
out vec3 fColor;

//Uniform textures samplers from program
uniform sampler2D txDiffuse;

//Uniform lighting vectors from program
uniform vec3 uLightPos;
uniform vec3 uLightColor;
uniform float uLightPower;

//Uniform material vectors from program
uniform vec3 uMaterialDiffuse;
uniform vec3 uMaterialAmbient;
uniform vec3 uMaterialSpecular;

void main()
{	
	//Calculate material colors from textures
	vec3 mDiffuseColor = uMaterialDiffuse * texture(txDiffuse, vUV).rgb * 2;
	vec3 mAmbientColor = uMaterialAmbient * mDiffuseColor;
	vec3 mSpecularColor = uMaterialSpecular;

	//Calculate distance fro light to vertex position
	float distance = length(uLightPos * - vPosition);

	//Calculate angle between normal and light direction
	vec3 n = tsNormal;
	vec3 l = normalize(tsLightDirection);
	float cosTheta = clamp(dot(n, l), 0, 1);

	//Calculate angle between eye direction and reflection
	vec3 e = normalize(tsEyeDirection);
	vec3 r = reflect(-l, n);
	float cosAlpha = clamp(dot(e, r), 0, 1);

	//Set vertex color
	fColor = 
		//Add ambient color
		mAmbientColor +
		//Calculate diffuse color
		mDiffuseColor * uLightColor * uLightPower * cosTheta / (distance*distance) +
		//Calculate specular color
		mSpecularColor * uLightColor * uLightPower * pow(cosAlpha,5) / (distance*distance);
}