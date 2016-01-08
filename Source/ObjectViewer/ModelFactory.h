#include "Header.h"

class ModelFactory
{
private:

	//Singleton
	ModelFactory(){};
	ModelFactory(ModelFactory const&){};
	ModelFactory& operator=(ModelFactory const&){};
	static ModelFactory* privateInstance;

	//Temp objects
	string m_loadPath;
	string m_currentMaterial;

	//Obj loading
	void LoadObjFile(string _path);
	void LoadObjLine(string _contents);
	void LoadObjVertex(vector<string> _vertex);
	void LoadObjUv(vector<string> _uv);
	void LoadObjNormal(vector<string> _normal);
	void LoadObjFace(vector<string> _face);
	
	//Material 
	void LoadMaterialDiffuseMap(string _path);
	void LoadMaterialNormalMap(string _path);
	void LoadMaterialSpecularMap(string _path);
	void LoadMaterialDiffuse(vector<string> _diffuse);
	void LoadMaterialAmbient(vector<string> _ambient);
	void LoadMaterialSpecular(vector<string> _specular);

	//Buffer objects (Loaded from file)
	vector<glm::vec3> m_vertices;
	vector<glm::vec2> m_uvs;
	vector<glm::vec3> m_normals;

	//Face buffer objects (Key is the material to use)
	map<string, vector<glm::vec3>> m_faceVertices;
	map<string, vector<glm::vec3>> m_faceUvs;
	map<string, vector<glm::vec3>> m_faceNormals;

	//Material buffer objects (key is the material name)
	map<string, SDL_Surface *> m_materialDiffuseMaps;
	map<string, SDL_Surface *> m_materialNormalMaps;
	map<string, SDL_Surface *> m_materialSpecularMaps;
	map<string, glm::vec3> m_materialDiffuse;
	map<string, glm::vec3> m_materialAmbient;
	map<string, glm::vec3> m_materialSpecular;

	//Data objects (Sent to VBO)
	vector<glm::vec3> m_vertexData;
	vector<glm::vec2> m_uvData;
	vector<glm::vec3> m_normalData;
	vector<glm::vec3> m_tangentData;
	vector<glm::vec3> m_bitangentData;
	
	//Compile buffer objects into data objects
	Model* CompileModel();
	ModelPart* CompilePart(string _material);
	Material* CompileMaterial(string _material);

	//Compile model into GLData and send to shader
	void CompileData();
	void CompileDataAtPosition(int _position, int _size, const void *data, int _indexes);

	//Compile each data type to a part
	void CompileVerticies(string _material);
	void CompileUvs(string _material);
	void CompileNormals(string _material);
	void CompileTangents();
	GLuint CompileTexture(int unit, SDL_Surface *_surface);

public:
	
	//Singleton
	static ModelFactory *Instance();

	//Load obj file into buffers
	Model *LoadObj(string _path);

	//Deconstructor
	~ModelFactory()
	{
		//Remove all surface objects
		for(map<string, SDL_Surface *>::iterator iterator = m_materialDiffuseMaps.begin(); iterator != m_materialDiffuseMaps.end(); iterator++)
		{
			delete m_materialDiffuseMaps[iterator->first];
		}
	}
};