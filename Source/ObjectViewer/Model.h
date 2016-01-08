#include "Header.h"

class Model{

private:

	//Parts of the model
	vector<ModelPart *> m_parts;

public:

	//Setters for model parts
	void PushModelPart(ModelPart *_part);
	void SetShader(Shader *_shader);

	//Actions to perform on all model parts
	void Draw();

	//Deconstructor
	~Model()
	{
		//Remove all parts in the model
		for (int i=0; i<m_parts.size(); i++)
		{
			delete m_parts[i];
		}
	}
};