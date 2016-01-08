#include "Header.h"

//Setter for data and material
void ModelPart::SetData(GLuint _model, int _length)
{
	m_modelData = _model;
	m_dataLength = _length;
}
void ModelPart::SetMaterial(Material *_mat)
{
	m_material = _mat;
}
void ModelPart::SetShader(Shader *_shader)
{
	m_material->SetShader(_shader);

	//Get locations of material uniform data
	m_material->m_diffuse.location = glGetUniformLocation(_shader->GetData(), "uMaterialDiffuse");
	m_material->m_specular.location = glGetUniformLocation(_shader->GetData(), "uMaterialSpecular");
	m_material->m_ambient.location = glGetUniformLocation(_shader->GetData(), "uMaterialAmbient");
}

//Draw model part to the screen
void ModelPart::Draw()
{
	//Bind and unbind material
	m_material->Bind();

		//Update material uniforms
		glUniform3f(m_material->m_diffuse.location, m_material->m_diffuse.value.x, m_material->m_diffuse.value.y, m_material->m_diffuse.value.z);
		glUniform3f(m_material->m_specular.location, m_material->m_specular.value.x, m_material->m_specular.value.y, m_material->m_specular.value.z);
		glUniform3f(m_material->m_ambient.location, m_material->m_ambient.value.x, m_material->m_ambient.value.y, m_material->m_ambient.value.z);

		//Bind and unbind vertex array object
		glBindVertexArray(m_modelData);

			//Draw vertex arrays
			glDrawArrays(GL_TRIANGLES, 0, m_dataLength);

		glBindVertexArray(0);

	m_material->Unbind();
}