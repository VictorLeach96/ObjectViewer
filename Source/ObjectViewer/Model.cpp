#include "Header.h"

//Setters for model parts
void Model::PushModelPart(ModelPart *_modelPart)
{
	m_parts.push_back(_modelPart);
}
void Model::SetShader(Shader *_shader)
{
	//Loop through each model part and draw
	for (int i=0; i<m_parts.size(); i++){
		m_parts[i]->SetShader(_shader);
	}
}

//Actions to perform on all model parts
void Model::Draw()
{
	//Loop through each model part and draw
	for (int i=0; i<m_parts.size(); i++){
		m_parts[i]->Draw();
	}
}