#include "Header.h"

//Getter for GLData
GLuint Shader::GetData()
{
	return m_data;
}

//Setters for vertex string shaders
void Shader::SetVertexShader(string _path)
{
	m_vertexShader = ReadFile(_path);
}
void Shader::SetFragmentShader(string _path)
{
	m_fragmentShader = ReadFile(_path);
}

//Populate string shader from file
string Shader::ReadFile(string _path)
{
	//Setup file and string streams
	ifstream stream;
	string buffer;
	string output;

	//Loop through each line and make a string
    stream.open(_path, std::ifstream::in);
	while (getline(stream, buffer))
	{
		output.append(buffer + '\n');
    }

	//Close the file and return
	stream.close();
	return output;
}

//Compile string shader into GLData
void Shader::CompileData()
{
	//Init shader program with string data
	m_data = glCreateProgram();
	const char *vertexShaderData = m_vertexShader.c_str();
	const char *fragmentShaderData = m_fragmentShader.c_str();

	//Create the and validate vertex shader
	GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vShader, 1, &vertexShaderData, NULL);
	glCompileShader(vShader);
	if(!CheckShaderValid(vShader))
	{
		m_data = 0;
	}
	glAttachShader(m_data, vShader);

	//Create and validate fragment shader
	GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fShader, 1, &fragmentShaderData, NULL);
	glCompileShader(fShader);
	if(!CheckShaderValid(fShader))
	{
		m_data = 0;
	}
	glAttachShader(m_data, fShader);

	//Link both compiled shaders
	glLinkProgram(m_data);
	GLint linked;
	glGetProgramiv(m_data, GL_LINK_STATUS, &linked);
	if (!linked)
	{
		GLsizei len;
		glGetProgramiv(m_data, GL_INFO_LOG_LENGTH, &len);
		GLchar* log = new GLchar[len+1];
		glGetProgramInfoLog(m_data, len, &len, log );
		std::cerr << "ERROR: Shader linking failed: " << log << std::endl;
		delete [] log;
		m_data = 0;
	}
}

//Check vertex has compiled correctly
bool Shader::CheckShaderValid(GLint _shader)
{
	GLint compiled;
	glGetShaderiv(_shader, GL_COMPILE_STATUS, &compiled);
	if (!compiled)
	{
		GLsizei len;
		glGetShaderiv(_shader, GL_INFO_LOG_LENGTH, &len);
		GLchar* log = new GLchar[len+1];
		glGetShaderInfoLog(_shader, len, &len, log);
		std::cerr << "ERROR: Shader compilation failed: " << log << std::endl;
		delete [] log;
		return false;
	}
	return true;
}