#include "Header.h"

class Material{

private:

	//Data
	GLuint m_diffuseId;
	GLuint m_diffuseData;
	GLuint m_normalId;
	GLuint m_normalData;
	GLuint m_specularId;
	GLuint m_specularData;

public:

	//Lighting
	Vector m_diffuse;
	Vector m_ambient;
	Vector m_specular;

	//Setter for texture and lighting
	void SetTexture(GLuint _texture);
	void SetNormal(GLuint _normal);
	void SetSpecular(GLuint _texture);
	void SetShader(Shader* _shader);

	//Bind and unbinding the material
	void Bind();
	void Unbind();

};