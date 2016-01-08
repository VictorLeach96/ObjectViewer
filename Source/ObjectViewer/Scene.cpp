#include "Header.h"

//Apply shader to this scene
void Scene::SetShader(Shader* _shader)
{
	//Assign the shader and bind it
	m_shader = _shader;
	glUseProgram(m_shader->GetData());

	//Attach matrices to shader
	m_modelMatrix.location = glGetUniformLocation(m_shader->GetData(), "uModelMat");
	m_viewMatrix.location = glGetUniformLocation(m_shader->GetData(), "uViewMat");
	m_projMatrix.location = glGetUniformLocation(m_shader->GetData(), "uProjMat");

	//Attach vectors to shader
	m_lightPos.location = glGetUniformLocation(m_shader->GetData(), "uLightPos");
	m_lightColor.location = glGetUniformLocation(m_shader->GetData(), "uLightColor");
	m_lightPower.location = glGetUniformLocation(m_shader->GetData(), "uLightPower");
}

//Reset all matrix values to their defaults
void Scene::ResetView(glm::vec3 _offset)
{
	//Default translations for matrixes
	m_modelMatrix.value = glm::translate(glm::mat4(1.0f), _offset);
	m_viewMatrix.value = glm::translate(glm::mat4(1.0f), glm::vec3(0,0,-2.5f));
	m_projMatrix.value = glm::perspective(45.0f, 1.0f, 0.1f, 100.0f);

	//Default values for lighting
	m_lightPos.value = glm::vec3(5.0f, 5.0f, 5.0f);
	m_lightColor.value = glm::vec3(0.5f, 0.5f, 0.5f);
	m_lightPower.value = 50.0f;
}

//Sets the pivot point for the model
void Scene::SetPivot(glm::vec3 _pivot)
{
	m_modelMatrix.pivot = _pivot;
}

//Load in new model with filename
void Scene::LoadModel(string _filename)
{
	//Message to tell user to wait as loading takes a while
	cout << endl << "Loading in new model, please wait..." << endl;

	//Load in the new model from the factory with given filename
	m_model = ModelFactory::Instance()->LoadObj(_filename);
	m_model->SetShader(m_shader);

	//Reset the matrix locations and key states
	m_curKey.Reset();
}

//Load the default model from the model factory 
void Scene::Setup()
{
	LoadModel("models/gun/Handgun.obj");
	ResetView(glm::vec3(0.0f, -0.2f, 1.0f));
}

//Game update loop (handle input and refresh logic)
bool Scene::Update(float _deltaTime)
{
	//Handle keyboard input
	SDL_Event incomingEvent;
	while (SDL_PollEvent(&incomingEvent)) 
	{
		switch (incomingEvent.type)
		{
		case SDL_QUIT:
			return false;
			break;

			//Key has been pressed down
		case SDL_KEYDOWN:
			switch (incomingEvent.key.keysym.sym)
			{
				//Reset matrix keys
			case SDLK_SPACE:
			case SDLK_BACKSPACE:
			case SDLK_RETURN:
				ResetView(glm::vec3(0.0f, -0.2f, 1.0f));
				break;

				//Escape key
			case SDLK_ESCAPE:
				return false;
				break;

				//Up / W key
			case SDLK_UP:
			case SDLK_w:
					m_curKey.panUp = true;
				break;

				//Down / S key
			case SDLK_DOWN:
			case SDLK_s:
					m_curKey.panDown = true;
				break;

				//Left / A key
			case SDLK_LEFT:
			case SDLK_a:
					m_curKey.panLeft = true;
				break;

				//Right  / D key
			case SDLK_RIGHT:
			case SDLK_d:
					m_curKey.panRight = true;
				break;

				//Q key for zoom in
			case SDLK_q:
					m_curKey.zoomIn = true;
				break;

				//E key for zoom out
			case SDLK_e:
					m_curKey.zoomOut = true;
				break;
			}
			break;

			//Key has been released
		case SDL_KEYUP:
			switch (incomingEvent.key.keysym.sym)
			{
				//Escape Key
			case SDLK_ESCAPE:
				return false;
				break;

				//Up / w key
			case SDLK_UP:
			case SDLK_w:
					m_curKey.panUp = false;
				break;

				//Down / s key
			case SDLK_DOWN:
			case SDLK_s:
					m_curKey.panDown = false;
				break;

				//Left /a key
			case SDLK_LEFT:
			case SDLK_a:
					m_curKey.panLeft = false;
				break;

				//Right / d key
			case SDLK_RIGHT:
			case SDLK_d:
					m_curKey.panRight = false;
				break;

				//Q key for zoom in
			case SDLK_q:
					m_curKey.zoomIn = false;
				break;

				//E key for zoom out
			case SDLK_e:
					m_curKey.zoomOut = false;
				break;
			}
			break;
		}
	}

	//Matrix maths for zooming in and out of model
	if (m_curKey.zoomIn)
	{
		m_viewMatrix.translate(glm::vec3(0.0f, 0.0f, 0.2f));
	}
	else if (m_curKey.zoomOut)
	{
		m_viewMatrix.translate(glm::vec3(0.0f, 0.0f, -0.2f));
	}
	
	//Matrix maths for panning around models pivot
	else if (m_curKey.panRight)
	{
		m_modelMatrix.rotate(2.0f * _deltaTime, glm::vec3(0.0f, 1.0f, 0.0f));
	}
	else if (m_curKey.panLeft)
	{
		m_modelMatrix.rotate(2.0f * _deltaTime, glm::vec3(0.0f, -1.0f, 0.0f));
	}
	else if (m_curKey.panUp)
	{
		m_modelMatrix.rotate(2.0f * _deltaTime, glm::vec3(1.0f, 0.0f, 0.0f));
	}
	else if (m_curKey.panDown)
	{
		m_modelMatrix.rotate(2.0f * _deltaTime, glm::vec3(-1.0f, 0.0f, 0.0f));
	}

	//Default rotation to apply when no button is being pressed
	else
	{
		m_modelMatrix.rotate(0.4f * _deltaTime, glm::vec3(0.0f, 1.0f, 0.0f));
	}

	return true;
}

//Update shader with new matrix locations and send to the shader
void Scene::Draw()
{
	//Bind default shader
	glUseProgram(m_shader->GetData());
	
	//Update matrix locations with new values
	glUniformMatrix4fv(m_modelMatrix.location, 1, GL_FALSE, glm::value_ptr(m_modelMatrix.value));
	glUniformMatrix4fv(m_viewMatrix.location, 1, GL_FALSE, glm::value_ptr(m_viewMatrix.value));
	glUniformMatrix4fv(m_projMatrix.location, 1, GL_FALSE, glm::value_ptr(m_projMatrix.value));

	//Update lighting locations with new values
	glUniform3f(m_lightPos.location, m_lightPos.value.x, m_lightPos.value.y, m_lightPos.value.z);
	glUniform3f(m_lightColor.location, m_lightColor.value.x, m_lightColor.value.y, m_lightColor.value.z);
	glUniform1f(m_lightPower.location, m_lightPower.value);

	//Draw model to scene
	m_model->Draw();
	
	//Unbind default shader
	glUseProgram(0);
}