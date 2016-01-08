#include "Header.h"

//Setter for the texture and lighting
void Material::SetTexture(GLuint _texture)
{
	m_diffuseData = _texture;
}
void Material::SetNormal(GLuint _texture)
{
	m_normalData = _texture;
}
void Material::SetSpecular(GLuint _texture)
{
	m_specularData = _texture;	
}
void Material::SetShader(Shader* _shader)
{
	m_diffuseId = glGetUniformLocation(_shader->GetData(), "txDiffuse");
	m_normalId = glGetUniformLocation(_shader->GetData(), "txNormal");
	m_specularId = glGetUniformLocation(_shader->GetData(), "txSpecular");
}

//Bind and unbinding textures
void Material::Bind()
{
	//Bind diffuse texture map to unit 0
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_diffuseData);
	glUniform1i(m_diffuseId, 0);
	
	//Bind normal texture map to unit 1
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_normalData);
	glUniform1i(m_normalId, 1);

	//Bind specular texture map to unit 2	
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, m_specularData);
	glUniform1i(m_specularId, 2);	
}
void Material::Unbind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindTexture(GL_TEXTURE_2D, 1);
	glBindTexture(GL_TEXTURE_2D, 2);
}