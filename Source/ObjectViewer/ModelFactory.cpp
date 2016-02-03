#include "Header.h"

//Singleton
ModelFactory* ModelFactory::privateInstance = NULL; 
ModelFactory* ModelFactory::Instance()
{
   if (!privateInstance)
   {
      privateInstance = new ModelFactory;
   }
   return privateInstance;
}

//====================-------------------- Obj Loading --------------------====================

//Load contents of file into string line by line
Model* ModelFactory::LoadObj(string _path)
{
	//If no material is found just use the default
	m_currentMaterial = "default";

	//Prepare path for loading
	string localFile = _path.substr(_path.find_last_of("/") + 1);
	m_loadPath = _path.substr(0, _path.find_last_of("/")  + 1);

	//Load object from obj file
	LoadObjFile(localFile);

	//Report total loaded items
	cout << endl << "Loaded Total Verticies: " << m_vertices.size() << endl;
	cout << "Loaded Total Normals: " << m_normals.size() << endl;
	cout << "Loaded Total UVs: " << m_uvs.size() << endl;

	//Compile the new model
	Model *model = CompileModel();

	//Clear all buffer objects after finished
	m_vertices.clear();
	m_uvs.clear();
	m_normals.clear();
	m_faceVertices.clear();
	m_faceUvs.clear();
	m_faceNormals.clear();
	m_materialDiffuseMaps.clear();
	m_materialNormalMaps.clear();
	m_materialSpecularMaps.clear();
	m_materialDiffuse.clear();
	m_materialAmbient.clear();
	m_materialSpecular.clear();

	return model;
}

//Load contents of file into string line by line
void ModelFactory::LoadObjFile(string _path)
{
	//Setup buffers and streams
	ifstream stream;
	string buffer;

	//Loop through each line and make a string
    stream.open(m_loadPath + _path, std::ifstream::in);
	while (getline(stream, buffer)){
		if (buffer != ""){
			
			//Feed each of the strings through the string loading method
			LoadObjLine(buffer);
		}
    }

	//Close the file and compile the data
	stream.close();
}

//Load single line of a obj file into the buffer vertexes
void ModelFactory::LoadObjLine(string _contents)
{
	//Seperate the given string contents by spaces and store in the `sections` vector
    string buffer;
    stringstream stream(_contents);
    vector<string> sections;
    while (stream >> buffer){
		sections.push_back(buffer);
	}

	//Get prefix and remove it from the sections
	string prefix = sections[0];
	sections.erase(sections.begin());

	//Check the prefix of section and direct to pass it into the required load function
	if (prefix == "v")
	{
		//Verticies
		LoadObjVertex(sections);
	}
	else if (prefix == "vt")
	{
		//UV
		LoadObjUv(sections);
	}
	else if (prefix == "vn")
	{
		//Normal
		LoadObjNormal(sections);
	}
	else if (prefix == "f")
	{
		//Faces
		LoadObjFace(sections);
	}
	else if (prefix == "mtllib")
	{
		//Load a new material from mtl file
		LoadObjFile(sections[0]);
	}
	else if (prefix == "usemtl" || prefix == "newmtl")
	{
		//External material has changed or is being created
		m_currentMaterial = sections[0];
	}
	else if (prefix == "map_Kd")
	{
		//Texture
		LoadMaterialDiffuseMap(sections[0]);
	}
	else if (prefix == "map_Bump")
	{
		//Texture
		LoadMaterialNormalMap(sections[0]);
	}
	else if (prefix == "map_Ks")
	{
		//Texture
		LoadMaterialSpecularMap(sections[0]);
	}
	else if (prefix == "Kd")
	{
		//Diffuse
		LoadMaterialDiffuse(sections);
	}
	else if (prefix == "Ka")
	{
		//Ambient
		LoadMaterialAmbient(sections);
	}
	else if (prefix == "Ks")
	{
		//Specular
		LoadMaterialSpecular(sections);
	}
}

//Load a vertex from obj file (v * * *)
void ModelFactory::LoadObjVertex(vector<string> _vertex)
{
	//Load XYZ co-ordinate positions
	float x = stof(_vertex[0]);
	float y = stof(_vertex[1]);
	float z = stof(_vertex[2]);

	//Add vertex to the model
	m_vertices.push_back(glm::vec3(x, y, z));
}

//Load uv co-ordinate from obj file (vt * *)
void ModelFactory::LoadObjUv(vector<string> _uv)
{
	//Load XY co-ordinate positions
	float x = stof(_uv[0]);
	float y = stof(_uv[1]);

	//Add uv to the model
	m_uvs.push_back(glm::vec2(x, y));
}

//Load normal co-ordinates from obj file (vn * *)
void ModelFactory::LoadObjNormal(vector<string> _normal)
{
	//Load XYZ co-ordinate positions
	float x = stof(_normal[0]);
	float y = stof(_normal[1]);
	float z = stof(_normal[2]);

	//Add uv to the model
	m_normals.push_back(glm::vec3(x, y, z));
}

//Load a face from obj file (f * * *)
void ModelFactory::LoadObjFace(vector<string> _face)
{
	//Prepare destination for 
	glm::vec3 faceVertex;
	glm::vec3 faceUv;
	glm::vec3 faceNormal;

	//Split string using / into a vector

	//Split and add X position
	string buffer;
	stringstream stream(_face[0]);
	vector<string> subsections;
	while(getline(stream, buffer, '/'))
	{
		subsections.push_back(buffer);
	}

	//Set each vector value of the face
	faceVertex.x = stoi(subsections[0]);
	faceUv.x = stoi(subsections[1]);
	if (subsections.size() >= 3)
	{
		//Add normal values if they were found
		faceNormal.x = stoi(subsections[2]);
	}
	
	//Split and add Y position
	stringstream stream2(_face[1]);
	subsections.clear();
	while(getline(stream2, buffer, '/'))
	{
		//Add normal values if they were found
		subsections.push_back(buffer);
	}
	
	//Set each vector value of the face
	faceVertex.y = stoi(subsections[0]);
	faceUv.y = stoi(subsections[1]);
	if (subsections.size() >= 3)
	{
		faceNormal.y = stoi(subsections[2]);
	}

	//Split and add Z position
	stringstream stream3(_face[2]);
	subsections.clear();
	while(getline(stream3, buffer, '/'))
	{
		//Add normal values if they were found
		subsections.push_back(buffer);
	}
	
	//Set each vector value of the face
	faceVertex.z = stoi(subsections[0]);
	faceUv.z = stoi(subsections[1]);
	if (subsections.size() >= 3)
	{
		faceNormal.z = stoi(subsections[2]);
	}

	//Add verticies and faces to the model
	m_faceVertices[m_currentMaterial].push_back(faceVertex);
	m_faceUvs[m_currentMaterial].push_back(faceUv);
	m_faceNormals[m_currentMaterial].push_back(faceNormal);
}

//====================-------------------- Material Loading --------------------====================

//Load in a material from texture file
void ModelFactory::LoadMaterialDiffuseMap(string _path)
{
	//Load texture into SDL surface
	string texturePath = m_loadPath + _path;
	SDL_Surface *surface = IMG_Load(texturePath.c_str());
	
	//Check if surface was found and add it to materials
	if (surface)
	{
		m_materialDiffuseMaps[m_currentMaterial] = surface;
	}
	else
	{
		cout << "Error loading texture surface " << texturePath;	
	}
}

void ModelFactory::LoadMaterialNormalMap(string _path)
{
	//Load texture into SDL surface
	string texturePath = m_loadPath + _path;
	SDL_Surface *surface = IMG_Load(texturePath.c_str());
	
	//Check if surface was found and add it to materials
	if (surface)
	{
		m_materialNormalMaps[m_currentMaterial] = surface;
	}
	else
	{
		cout << "Error loading texture surface " << texturePath;	
	}
}

void ModelFactory::LoadMaterialSpecularMap(string _path)
{
	//Load texture into SDL surface
	string texturePath = m_loadPath + _path;
	SDL_Surface *surface = IMG_Load(texturePath.c_str());
	
	//Check if surface was found and add it to materials
	if (surface)
	{
		m_materialSpecularMaps[m_currentMaterial] = surface;
	}
	else
	{
		cout << "Error loading texture surface " << texturePath;	
	}
}

//Load in a materials diffuse
void ModelFactory::LoadMaterialDiffuse(vector<string> _diffuse)
{
	//Load XYZ
	float x = stof(_diffuse[0]);
	float y = stof(_diffuse[1]);
	float z = stof(_diffuse[2]);

	//Add to the material
	m_materialDiffuse[m_currentMaterial] = glm::vec3(x, y, z);
}

//Load in a materials ambient
void ModelFactory::LoadMaterialAmbient(vector<string> _ambient)
{
	//Load XYZ
	float x = stof(_ambient[0]);
	float y = stof(_ambient[1]);
	float z = stof(_ambient[2]);

	//Add to the material
	m_materialAmbient[m_currentMaterial] = glm::vec3(x, y, z);
}

//Load in a materials specular
void ModelFactory::LoadMaterialSpecular(vector<string> _specular)
{
	//Load XYZ
	float x = stof(_specular[0]);
	float y = stof(_specular[1]);
	float z = stof(_specular[2]);

	//Add to the material
	m_materialSpecular[m_currentMaterial] = glm::vec3(x, y, z);
}

//====================-------------------- Model Compiling --------------------====================

//Compile buffers and create model object
Model* ModelFactory::CompileModel()
{
	//Create new model buffer and list of textures
	Model *model = new Model();

	//Loop through each of the materials and create a new part
	for(map<string, SDL_Surface *>::iterator iterator = m_materialDiffuseMaps.begin(); iterator != m_materialDiffuseMaps.end(); iterator++)
	{ 
		//Report loaded part and material
		cout << endl << "Material Group: " << iterator->first << endl;

		//Compile the model for this material
		ModelPart *modelPart = CompilePart(iterator->first);
		
		//Report loaded faces
		cout << endl << "Loaded Faces: " << m_vertexData.size() << endl;

		//Add part onto model
		model->PushModelPart(modelPart);
	}

	return model;
}

//Compile each model part which relates to a specific material
ModelPart* ModelFactory::CompilePart(string _material)
{
	//Clear all the GLData buffers before compilation
	m_vertexData.clear();
	m_uvData.clear();
	m_normalData.clear();
	m_tangentData.clear();
	m_bitangentData.clear();

	//Create new VBO
	GLuint tempData = 0;
	glGenVertexArrays(1, &tempData);
	glBindVertexArray(tempData);

	//Compile all verticies into GLData
	CompileVerticies(_material);
		
	//Compile uv co-ordinates into GLData
	if (m_uvs.size() > 0)
	{
		CompileUvs(_material);
	}	
		
	//Compile normal co-ordniates into GLData, making sure that normals exist
	if (m_normals.size() > 0)
	{
		CompileNormals(_material);
	}

	CompileTangents();

	//Compile everything into GLData
	CompileData();

	//Setup the model part and assign compiled data
	ModelPart *modelPart = new ModelPart();
	modelPart->SetMaterial(CompileMaterial(_material));
	modelPart->SetData(tempData, m_vertexData.size());

	//Reset VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glDisableVertexAttribArray(0);

	return modelPart;
}

Material* ModelFactory::CompileMaterial(string _material)
{
	//Get texture and normal maps
	SDL_Surface* diffuse = m_materialDiffuseMaps[_material];
	SDL_Surface* normal = m_materialNormalMaps[_material];
	SDL_Surface* specular = m_materialSpecularMaps[_material];

	//Create the material object and compile texture
	Material *material = new Material();

	//Apply diffuse map to material
	if (diffuse)
	{
		material->SetTexture(CompileTexture(0, diffuse));
	}

	//Apply normal map to material
	if (normal)
	{
		material->SetNormal(CompileTexture(1, normal));
	}

	//Apply specular map to material
	if (specular)
	{
		material->SetSpecular(CompileTexture(2, specular));
	}

	//Apply material properties
	if (m_materialDiffuse.count(_material) > 0)
	{
		material->m_diffuse.value = m_materialDiffuse[_material];
	}
	if (m_materialAmbient.count(_material) > 0)
	{
		material->m_ambient.value = m_materialAmbient[_material];
	}
	if (m_materialSpecular.count(_material) > 0)
	{
		material->m_specular.value = m_materialSpecular[_material];
	}

	return material;
}

//Compile the current model part into GLData
void ModelFactory::CompileData()
{
	//Vertex ID 0
	CompileDataAtPosition(0, m_vertexData.size() * sizeof(glm::vec3), &m_vertexData[0], 3);

	//UVS ID 1
	if (m_uvData.size() > 0)
	{
		CompileDataAtPosition(1, m_uvData.size() * sizeof(glm::vec2), &m_uvData[0], 2);
	}
	
	//Normals ID 2
	if (m_normalData.size() > 0)
	{
		CompileDataAtPosition(2, m_normalData.size() * sizeof(glm::vec3), &m_normalData[0], 3);
	}

	//Tangents ID 3
	if (m_tangentData.size() > 0)
	{
		CompileDataAtPosition(3, m_tangentData.size() * sizeof(glm::vec3), &m_tangentData[0], 3);
	}

	//Bittangents ID 4
	if (m_bitangentData.size() > 0)
	{
		CompileDataAtPosition(4, m_bitangentData.size() * sizeof(glm::vec3), &m_bitangentData[0], 3);
	}
}

//Send the GLData to shader at specific layout id
void ModelFactory::CompileDataAtPosition(int _id, int _size, const void *_data, int _indexes)
{
	//Setup buffer with data
	GLuint buffer = 0;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	
	//Send each vertex to the VBO
	glBufferData(GL_ARRAY_BUFFER, _size, _data, GL_STATIC_DRAW);

	//Send buffer object to VBO at shader index
	glVertexAttribPointer(_id, _indexes, GL_FLOAT, GL_FALSE, 0, 0 );
	glEnableVertexAttribArray(_id);
}

//====================-------------------- Part Compiling --------------------====================

//Compile and index vertex co-ordinates which belong to a material
void ModelFactory::CompileVerticies(string _material)
{
	//Loop through each of the vertex positions found for the material
	vector<glm::vec3> faceVerticiesMat = m_faceVertices[_material];
	for(int i=0; i<faceVerticiesMat.size(); i++)
	{ 
		//Find and add coresponding vertex points to the vertex list
		glm::vec3 face = faceVerticiesMat[i];

		//Add each to vertex data
		m_vertexData.push_back(m_vertices[face.x-1]);
		m_vertexData.push_back(m_vertices[face.y-1]);
		m_vertexData.push_back(m_vertices[face.z-1]);
	}
}

//Compile and index uv co-ordinates which belong to a material
void ModelFactory::CompileUvs(string _material)
{
	//Loop through each of the uv co-ordinates found for the material
	vector<glm::vec3> faceUvsMat = m_faceUvs[_material];
	for(int i=0; i<faceUvsMat.size(); i++)
	{ 
		//Find and add coresponding vertex points to the vertex list
		glm::vec3 face = faceUvsMat[i];

		//Add each to UV data
		//We ues 1.0f- to invert the texture to correct the orientation caused by SDL IMG_LOAD function
		m_uvData.push_back(glm::vec2(m_uvs[face.x-1].x, 1.0f - m_uvs[face.x-1].y));
		m_uvData.push_back(glm::vec2(m_uvs[face.y-1].x, 1.0f - m_uvs[face.y-1].y));
		m_uvData.push_back(glm::vec2(m_uvs[face.z-1].x, 1.0f - m_uvs[face.z-1].y));
	}
}

//Compile and index normal co-ordinates which belong to a material
void ModelFactory::CompileNormals(string _material)
{
	//Loop through each of the normal co-ordinate found for the material
	vector<glm::vec3> faceNormalsMat = m_faceNormals[_material];
	for(int i=0; i<faceNormalsMat.size(); i++)
	{ 
		//Find and add coresponding normal co-ordinates to the normal list
		glm::vec3 face = faceNormalsMat[i];

		//Add each to normal data
		m_normalData.push_back(m_normals[face.x-1]);
		m_normalData.push_back(m_normals[face.y-1]);
		m_normalData.push_back(m_normals[face.z-1]);
	}
}

void ModelFactory::CompileTangents()
{
	//Loop through each vertex and calculate its tangent and bitangent data
	for (int i=0; i<m_vertexData.size(); i+=3)
	{
		//Calculate the delta vertex position
		glm::vec3 deltaVertex1 = m_vertexData[i+1] - m_vertexData[i+0];
		glm::vec3 deltaVertex2 = m_vertexData[i+2] - m_vertexData[i+0];
 
		//Calculate the delta uv position
		glm::vec2 deltaUV1 = m_uvData[i+1] - m_uvData[i+0];
		glm::vec2 deltaUV2 = m_uvData[i+2] - m_uvData[i+0];

		//Calculate tangent and bittangent
		float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
		glm::vec3 tangent = (deltaVertex1 * deltaUV2.y   - deltaVertex2 * deltaUV1.y) * r;
		glm::vec3 bitangent = (deltaVertex2 * deltaUV1.x   - deltaVertex1 * deltaUV2.x) * r;

		//Add tangents to data
		m_tangentData.push_back(tangent);
		m_tangentData.push_back(tangent);
		m_tangentData.push_back(tangent);

		//Add bitangents to data
		m_bitangentData.push_back(bitangent);
		m_bitangentData.push_back(bitangent);
		m_bitangentData.push_back(bitangent);
	}
}

//Compile texture from an SDL surface to GLData
GLuint ModelFactory::CompileTexture(int _unit, SDL_Surface *_surface)
{
	//Setup the temporary buffer for texture
	GLuint texture = 0;
	glActiveTexture(GL_TEXTURE0 + _unit);
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	//Check the format and channel values for the surface
	int textureMode = GL_BGR;

	//Texture has an alpha channel
	if (_surface->format->BytesPerPixel == 4)
	{
		//Check the order of alpha channels
		if (_surface->format->Rmask == 0x000000ff)
		{
			textureMode = GL_RGBA;
		}
		else
		{
			textureMode = GL_BGRA;
		}
	}
	//Texture doesn't have an alpha channel
	else if (_surface->format->BytesPerPixel == 3)
	{
		//Check the order of alpha channels
		if (_surface->format->Rmask == 0x000000ff)
		{
			textureMode = GL_RGB;
		}
		else
		{
			textureMode = GL_BGR;
		}
	}

	//Generate the texture
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _surface->w, _surface->h, 0, textureMode, GL_UNSIGNED_BYTE, _surface->pixels);

	//Set texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//Generate mipmaps
	glGenerateMipmap(GL_TEXTURE_2D);

	//Unbind texture
	glBindTexture(GL_TEXTURE_2D, 0);

	return texture;
}
