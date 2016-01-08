#include "Header.h"

class Scene
{
private:

	Shader *m_shader;
	Model *m_model;
	KeyMap m_curKey; //Current key actions pressed

	//Model management functions
	void LoadModel(string _filename);
	void ResetView(glm::vec3 _offset);
	void SetPivot(glm::vec3 _offset);

	//Shader uniform matrix
	Matrix m_modelMatrix;
	Matrix m_viewMatrix;
	Matrix m_projMatrix;

	//Shader uniform colours
	Vector m_lightPos;
	Vector m_lightColor;
	Float m_lightPower;

	//Shader uniform material elements
	Vector m_materialDiffuse;
	Vector m_materialSpecular;
	Vector m_materialAmbient;

public:

	//Apply shader to this scene
	void SetShader(Shader* _shader);

	//Game Loop
	void Setup();
	bool Update(float _deltaTime);
	void Draw();

	//Deconstructor
	~Scene()
	{
		delete m_shader;
		delete m_model;
	}
};