#include "Header.h"

class Shader{

private:

	//Shader values
	string m_vertexShader;
	string m_fragmentShader;

	//Populate string shader from file
	string ReadFile(string _path);

	//Check vertex has compiled correctly
	bool CheckShaderValid(GLint _shader);

public:

	//GLData
	GLuint m_data;
	GLuint GetData();

	//Compile string shader into GLData
	void CompileData();

	//Populate vertex and fragment string shader from file
	void SetVertexShader(string _shader);
	void SetFragmentShader(string _shader);
};