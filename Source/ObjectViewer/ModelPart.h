#include "Header.h"

class ModelPart
{
private:

	//GLData
	GLuint m_modelData;
	int m_dataLength;

	//Texture
	Material *m_material;

public:

	//Setter for GLData
	void SetData(GLuint  _model, int  _length);
	void SetMaterial(Material * _mat);
	void SetShader(Shader *_shader);

	//Draw GLData to the screen
	void Draw();
};