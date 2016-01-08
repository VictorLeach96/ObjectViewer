#version 430 core

/// Normal Mapping Shader Example
/// Arnaud Masserann (Jan 3, 2013). Tutorial 13, Normal Mapping [online].
/// [Accessed 2015]. Available from: <https://github.com/opengl-tutorials/ogl/tree/master/tutorial13_normal_mapping>.

//Inputs from program
layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec2 inUV;
layout(location = 2) in vec3 inNormal;
layout(location = 3) in vec3 inTangent;
layout(location = 4) in vec3 inBitangent;

//Vertex outputs to fragment shader
out vec3 vPosition;
out vec3 vNormal;
out vec2 vUV;

//Lighting outputs to fragment shader
out vec3 csEyeDirection;
out vec3 csLightDirection;
out vec3 tsEyeDirection;
out vec3 tsLightDirection;

//Uniform MVP matrixes from program
uniform mat4 uModelMat;
uniform mat4 uViewMat;
uniform mat4 uProjMat;

//Uniform lighting vectors from program
uniform vec3 uLightPos;
uniform vec3 uLightColor;
uniform float uLightPower;

void main()
{
	//Set vertex position
	gl_Position =  uProjMat* uViewMat * uModelMat * vec4(inPosition,1);
	
	//Send vertex outputs to fragment shader
	vPosition = (uModelMat * vec4(inPosition,1)).xyz;
	vUV = inUV;

	//Calculate vertex position in camera space
	vec3 csVertexPosition = (uViewMat * uModelMat * vec4(inPosition,1)).xyz;
	csEyeDirection = vec3(0,0,0) - csVertexPosition;

	//Calculate light position in camera space
	vec3 csLightPosition = (uViewMat * vec4(uLightPos,1)).xyz;
	csLightDirection = csLightPosition + csEyeDirection;

	//Calculate vertex position data in camera space
	vec3 csVertexTangent = mat3(uModelMat) * inTangent;
	vec3 csVertexBitangent = mat3(uModelMat) * inBitangent;
	vec3 csVertexNoral = mat3(uModelMat) * inNormal;
	
	//Transpose camera space vertex data
	mat3 TBN = transpose(mat3(
		csVertexTangent,
		csVertexBitangent,
		csVertexNoral	
	));
	
	//Calculate light and eye position in tangent space
	tsLightDirection = TBN * csLightDirection;
	tsEyeDirection =  TBN * csEyeDirection;
}