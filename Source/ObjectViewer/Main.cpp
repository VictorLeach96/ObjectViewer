#include "Header.h"

int main(int argc, char *argv[])
{
	//Setup SDL video
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		cerr << "SDL Initialization failed!" << endl;
		return -1;
	}

	//Setup SDL attributes and versions
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	//Attach renderer and context to window
	SDL_Window *window = SDL_CreateWindow("Object Loader", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 840, 840, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
	SDL_Renderer * renderer = SDL_CreateRenderer( window, -1, 0 );
	SDL_GLContext glcontext = SDL_GL_CreateContext( window );

	//Setup glew
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if( GLEW_OK != err )
	{
		cerr<<"Glew Initializatoin failed!" << endl << glewGetErrorString(err) << endl;
		return -1;
	}

	//Enable OpenGL options
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_DEPTH_TEST);

	//Log OpenGL information
	cout << "GLEW Version: " << glewGetString(GLEW_VERSION)<< endl;
	cout << "OpenGL Version: " << glGetString( GL_VERSION ) << endl;
	cout << "OpenGL Vendor: " << glGetString( GL_VENDOR ) << endl;
	cout << "OpenGL Renderer: " << glGetString( GL_RENDERER ) << endl;
	cout << "OpenGL Shading Language Version: " << glGetString( GL_SHADING_LANGUAGE_VERSION ) << endl;

	//Create vertex and fragment shaders
	Shader *shader = new Shader();
	shader->SetFragmentShader("shaders/Fragment_Mapping.glsl");
	shader->SetVertexShader("shaders/Vertex_Mapping.glsl");
	shader->CompileData();
	
	//Create scene and assign shader/model
	Scene *scene = new Scene();
	scene->SetShader(shader);
	scene->Setup();

	//Execute the game loop
	bool isPlaying = true;
	int lastTick = SDL_GetTicks();
	while(isPlaying)
	{
		//Calculate delta time
		unsigned int currentTick = SDL_GetTicks();
		float deltaTime = (float) (currentTick - lastTick) / 1000.0f;
		lastTick = currentTick;

		//Clear the frame buffer to black
		glClearColor(0.0f,0.0f,0.0f,0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Update and draw the current scene to window
		//Check whether update method returned false in which case end the game
		isPlaying = scene->Update(deltaTime);
		scene->Draw();
		SDL_GL_SwapWindow(window);
		
		//Limit the fps if its running too high
		if(deltaTime < (1.0f/50.0f)) 
		{
			SDL_Delay((unsigned int) (((1.0f/50.0f) - deltaTime)*1000.0f) );
		}
	}

	//Cleanup and quit SDL
	SDL_GL_DeleteContext( glcontext );
	SDL_DestroyWindow( window );
	SDL_Quit();

	return 0;
}
