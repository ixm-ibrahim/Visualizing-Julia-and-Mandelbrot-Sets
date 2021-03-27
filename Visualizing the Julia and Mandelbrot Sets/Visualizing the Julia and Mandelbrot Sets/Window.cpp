/*********************************************************************
** Author: Ibrahim Mahmoud
** Date: 9/13/21
** Description: This is the implementation file for Window.h
*********************************************************************/

#include "Window.h"

//@TODO: is there a way to handle mouse movement without global variables?
double lastMouseX = 0;
double lastMouseY = 0;
double deltaX = 0;
double deltaY = 0;
bool firstMouse = true;

double scrollOffset = 0;
double rollAngle = 0;

float gameTime = 0;
float animTime = 0;
int animate = 0;

int id;

/*********************************************************************
** Function: Window
** Description: Base constructor for a Window class object
** Parameters: none
** Pre-Conditions: The GLFW context hasn't already been created
** Post-Conditions: A new camera object is created
*********************************************************************/
Window::Window()
{
	this->window = NULL;

	this->major = 4;
	this->minor = 5;

	this->width = 720;
	this->height = 720;

	Init();
}
/*********************************************************************
** Function: Window
** Description: Constructor for a Window class object
** Parameters: int, int, int, int
** Pre-Conditions: The GLFW context hasn't already been created
** Post-Conditions: A new camera object is created
*********************************************************************/
Window::Window(int width, int height, int major, int minor)
{
	this->window = NULL;

	this->width = width;
	this->height = height;

	this->major = major;
	this->minor = minor;

	Init();
}
/*********************************************************************
** Function: ~Window
** Description: Destructor for a Window class object
** Parameters: none
** Post-Conditions: Clears all allocated memory for the window
*********************************************************************/
Window::~Window()
{
	Terminate();
}

/*********************************************************************
** Function: ResetKeyState
** Description: Resets the state of each key (the ones that just look
**				for key presses) to be released
** Parameters: none
** Pre-Conditions: The camera must have already been initialized
** Post-Conditions: Each of the keys in keyState are set to GLFW_RELEASE
*********************************************************************/
void Window::ResetKeyState()
{
	this->keyState.clear();

	this->keyState.insert({GLFW_KEY_W, GLFW_RELEASE});				// move up on complex plane
	this->keyState.insert({GLFW_KEY_A, GLFW_RELEASE});				// move left on complex plane
	this->keyState.insert({GLFW_KEY_S, GLFW_RELEASE});				// move down on complex plane
	this->keyState.insert({GLFW_KEY_D, GLFW_RELEASE});				// move right on complex plane
	this->keyState.insert({GLFW_KEY_E, GLFW_RELEASE});				// roll complex plane to the right
	this->keyState.insert({GLFW_KEY_Q, GLFW_RELEASE});				// roll complex plane to the left
	this->keyState.insert({GLFW_KEY_I, GLFW_RELEASE});				// move up on complex plane
	this->keyState.insert({GLFW_KEY_J, GLFW_RELEASE});				// move left on complex plane
	this->keyState.insert({GLFW_KEY_K, GLFW_RELEASE});				// move down on complex plane
	this->keyState.insert({GLFW_KEY_L, GLFW_RELEASE});				// move right on complex plane
	this->keyState.insert({GLFW_KEY_U, GLFW_RELEASE});				// zoom in on complex plane
	this->keyState.insert({GLFW_KEY_O, GLFW_RELEASE});				// zoom out on complex plane
	this->keyState.insert({GLFW_KEY_LEFT, GLFW_RELEASE});			// continue animation
	this->keyState.insert({GLFW_KEY_DOWN, GLFW_RELEASE});			// stop animation
	this->keyState.insert({GLFW_KEY_RIGHT, GLFW_RELEASE});			// reverse animation
	this->keyState.insert({GLFW_KEY_1, GLFW_RELEASE});				// change maxIterations
	this->keyState.insert({GLFW_KEY_2, GLFW_RELEASE});				// change bailout
		this->keyState.insert({GLFW_KEY_COMMA, GLFW_RELEASE});		//@change bailout.y
		this->keyState.insert({GLFW_KEY_PERIOD, GLFW_RELEASE});		//@change bailout.z
		this->keyState.insert({GLFW_KEY_SLASH, GLFW_RELEASE});		//@change bailout.w
	this->keyState.insert({GLFW_KEY_3, GLFW_RELEASE});				// change maxDistance
	this->keyState.insert({GLFW_KEY_4, GLFW_RELEASE});				// change distFineness
	this->keyState.insert({GLFW_KEY_5, GLFW_RELEASE});				// change power
	this->keyState.insert({GLFW_KEY_6, GLFW_RELEASE});				// change c power
	this->keyState.insert({GLFW_KEY_7, GLFW_RELEASE});				// change fold angle
	this->keyState.insert({GLFW_KEY_8, GLFW_RELEASE});				// change fold count
	this->keyState.insert({GLFW_KEY_9, GLFW_RELEASE});				// change fold offset along the real axis
	this->keyState.insert({GLFW_KEY_0, GLFW_RELEASE});				// change fold offset along the imaginary axis
	this->keyState.insert({GLFW_KEY_MINUS, GLFW_RELEASE});			// toggle distance estimation
	this->keyState.insert({GLFW_KEY_EQUAL, GLFW_RELEASE});			// toggle conjugate
	this->keyState.insert({GLFW_KEY_TAB, GLFW_RELEASE});			// toggle julia set
	this->keyState.insert({GLFW_KEY_ENTER, GLFW_RELEASE});			// change fractal type
	this->keyState.insert({GLFW_KEY_LEFT_BRACKET, GLFW_RELEASE});	// change exterior coloring
	this->keyState.insert({GLFW_KEY_RIGHT_BRACKET, GLFW_RELEASE});	// change interior coloring
	this->keyState.insert({GLFW_KEY_BACKSPACE, GLFW_RELEASE});		// toggle centering on julia critical point
	this->keyState.insert({GLFW_KEY_BACKSLASH, GLFW_RELEASE});		// change orbit trap
	this->keyState.insert({GLFW_KEY_SPACE, GLFW_RELEASE});			// toggle riemann sphere
	this->keyState.insert({GLFW_KEY_PERIOD, GLFW_RELEASE});			// toggle terrain
	this->keyState.insert({GLFW_KEY_COMMA, GLFW_RELEASE});			// toggle terrain color
	this->keyState.insert({GLFW_KEY_SLASH, GLFW_RELEASE});			// toggle lighting
	this->keyState.insert({GLFW_KEY_T, GLFW_RELEASE});				// reset values
	this->keyState.insert({GLFW_KEY_GRAVE_ACCENT, GLFW_RELEASE});	// display information
	this->keyState.insert({GLFW_KEY_LEFT_CONTROL, GLFW_RELEASE});
	this->keyState.insert({GLFW_KEY_LEFT_SHIFT, GLFW_RELEASE});
	this->keyState.insert({GLFW_KEY_LEFT_ALT, GLFW_RELEASE});
	this->keyState.insert({GLFW_KEY_RIGHT_CONTROL, GLFW_RELEASE});
}

/*********************************************************************
** Function: Init
** Description: Initializes the GLFW context, loads shades, and creates the window (OpenGL 4.5)
** Parameters: none
** Pre-Conditions: The GLFW context hasn't already been created
** Post-Conditions: Memory is allocated for the GLFW window
*********************************************************************/
void Window::Init()
{
	srand(time(NULL));
	id = rand() % 10000;

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, this->major);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, this->minor);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif


	this->window = glfwCreateWindow(this->width, this->height, "Visualizing Julia and Mandelbrot Sets", NULL, NULL);

	if (this->window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		exit(-1);
	}

	glfwMakeContextCurrent(this->window);
	glfwSetFramebufferSizeCallback(this->window, framebuffer_size_callback);
	glfwSetCursorPosCallback(this->window, mouse_callback);
	glfwSetScrollCallback(this->window, scroll_callback);
	glfwSetKeyCallback(this->window, key_callback);
	glfwSetWindowCloseCallback(this->window, close_callback);

	glfwSetWindowUserPointer(this->window, this);

	GladLoader();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);


	deltaX = 0;
	deltaY = 0;
	lastMouseX = this->width / 2;
	lastMouseY = this->height / 2;

	this->camera = new Camera(this->window);
	this->camera->SetSetting("lock", true);

	ResetKeyState();

	InitObjects();
	InitShaders();
}
/*********************************************************************
** Function: Terminate
** Description: Destructor for a Window class object, clearing all previously allocated GLFW resources
** Parameters: none
** Post-Conditions: Clears all allocated memory used for the window
*********************************************************************/
void Window::Terminate()
{
	glDeleteVertexArrays(1, &this->VAO_plane);
	glDeleteBuffers(1, &this->VBO_plane);
	glDeleteVertexArrays(1, &this->VAO_sphere);
	glDeleteBuffers(1, &this->VBO_sphere);

	delete icosphere;
	delete camera;
	delete julia;
	delete mandelbrot;

	glfwTerminate();
}

/*********************************************************************
** Function: GetWidth
** Description: Returns the width of the window in pixels
** Parameters: none
*********************************************************************/
int Window::GetWidth()
{
	return this->width;
}
/*********************************************************************
** Function: GetHeight
** Description: Returns the height of the window in pixels
** Parameters: none
*********************************************************************/
int Window::GetHeight()
{
	return this->height;
}

/*********************************************************************
** Function: GladLoader
** Description: Load all OpenGL function pointers for the device's drivers
** Parameters: none
** Pre-Conditions: The glad pointers haven't already been created
** Post-Conditions: The required pointers for the device's drivers are allcoated
*********************************************************************/
void Window::GladLoader()
{
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		glfwTerminate();
		exit(-1);
	}
}

/*********************************************************************
** Function: LoadObjects
** Description: Set up textures, vertex data, buffers, and configure vertex attributes,
** Parameters: none
** Pre-Conditions: The glad pointers haven't already been created
** Post-Conditions: The required pointers for the device's drivers are allcoated
*********************************************************************/
void Window::InitObjects()
{
	// 2D

	TexturedVertex points[6];

	//	(-1,1)   (1,1)
	//		-----
	//		| \ |
	//		-----
	//	(-1,-1)	 (1,-1)

	// first triangle
	points[0] = TexturedVertex(glm::vec3(-1, 1, 0), glm::vec3(0, 0, 1), glm::vec2(0, 0));
	points[1] = TexturedVertex(glm::vec3(-1, -1, 0), glm::vec3(0, 0, 1), glm::vec2(0, 1));
	points[2] = TexturedVertex(glm::vec3(1, -1, 0), glm::vec3(0, 0, 1), glm::vec2(1, 1));
	// second triangle
	points[3] = TexturedVertex(glm::vec3(1, -1, 0), glm::vec3(0, 0, 1), glm::vec2(1, 1));
	points[4] = TexturedVertex(glm::vec3(1, 1, 0), glm::vec3(0, 0, 1), glm::vec2(1, 0));
	points[5] = TexturedVertex(glm::vec3(-1, 1, 0), glm::vec3(0, 0, 1), glm::vec2(0, 0));

	glGenVertexArrays(1, &VAO_plane);
	glGenBuffers(1, &VBO_plane);

	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO_plane);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_plane);
	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(TexturedVertex), points, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// texture coord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(1);


	// 3D

	icosphere = new IcoSphere(4, 1);

	glGenVertexArrays(1, &VAO_sphere);
	glGenBuffers(1, &VBO_sphere);

	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO_sphere);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_sphere);
	glBufferData(GL_ARRAY_BUFFER, icosphere->GetSize(), icosphere->vertices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// texture coord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);


	// Riemann Mandelbrot initial settings
	settings.Init();
}

/*********************************************************************
** Function: InitShaders
** Description: Set up shaders
** Parameters: none
** Pre-Conditions: The shaders haven't already been created
** Post-Conditions: The shader pointers are allcoated
*********************************************************************/
void Window::InitShaders()
{
	//mandelbrot = new Shader("Shaders/fractal_plane.vert", "Shaders/mandelbrot.frag");
	//julia = new Shader("Shaders/fractal_plane.vert", "Shaders/julia.frag");
	mandelbrot = new Shader("Shaders/fractal.vert", "Shaders/riemann_sphere.tesc", "Shaders/riemann_mandelbrot.tese", "Shaders/mandelbrot.frag");
	julia = new Shader("Shaders/fractal.vert", "Shaders/riemann_sphere.tesc", "Shaders/riemann_julia.tese", "Shaders/julia.frag");
}

bool Window::JustPressed(int key)
{
	return glfwGetKey(window, key) == GLFW_PRESS && keyState[key] == GLFW_RELEASE;
}

/*********************************************************************
** Function: ProcessInput
** Description: Process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
** Parameters: none
** Pre-Conditions: The Glad pointers haven't already been created
** Post-Conditions: The required pointers for the device's drivers are allcoated
*********************************************************************/
void Window::ProcessInput(float deltaInput)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	float zoomFactor = pow(2, -settings.zoom);

	if (zoomFactor > 1)
		zoomFactor = 1;

	// Mouse controls
	{
		if (settings.useRiemannSphere && glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL) != GLFW_PRESS)
			camera->KeyboardInput(keyState, deltaTime);

		bool leftClick = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;
		bool rightClick = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS;

		if (rightClick)
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		else
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

		if (deltaX != 0 || deltaY != 0)
		{
			if (settings.useRiemannSphere)
				camera->MouseInput(deltaX, deltaY, leftClick, rightClick);
			else if (rightClick)
			{
				float rad = rollAngle * M_PI / 180;
				float rad90 = rad + M_PI / 2;
				float factor = zoomFactor / 100.f;

				//settings.center -= glm::vec2(deltaX, deltaY) * zoomFactor / 100.f;
				settings.center -= glm::vec2(cos(rad), sin(rad)) * (float)deltaX * factor;
				settings.center -= glm::vec2(cos(rad90), sin(rad90)) * (float)deltaY * factor;
			}

			deltaX = 0;
			deltaY = 0;
		}

		if (scrollOffset != 0)
		{
			if (settings.useRiemannSphere)
				camera->ScrollInput(scrollOffset);
			else
			{
				float rad = rollAngle * M_PI / 180;
				float rad90 = rad + M_PI / 2;

				glm::vec2 xRoll = glm::vec2(cos(rad), sin(rad));
				glm::vec2 yRoll = glm::vec2(cos(rad90), sin(rad90));

				glm::vec2 normalizedMousePos = glm::vec2((float)lastMouseX / width, (float)lastMouseY / height) * 2.f - 1.f;
				glm::vec2 aspectRatio = glm::vec2((float)width / height, -1);
				glm::vec2 offset = normalizedMousePos * settings.radius * aspectRatio;

				//glm::vec2 mousePos = settings.center + offset / (float)pow(2,settings.zoom);
				glm::vec2 mousePos = settings.center + (xRoll * offset.x + yRoll * offset.y) / (float)pow(2, settings.zoom);
				settings.zoom += scrollOffset / 10.f;
				//settings.center = mousePos - offset / (float)pow(2, settings.zoom);
				settings.center = mousePos - (xRoll * offset.x + yRoll * offset.y) / (float)pow(2, settings.zoom);
			}

			scrollOffset = 0;
		}
	}

	// Keyboard controls
	{
		float change = 1 * deltaInput;

		// Change speed
		if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
			change /= 4;
		if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
			change *= 4;
		if (glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS)
			change *= -1;


		// Reset values
		if (JustPressed(GLFW_KEY_T))
		{
			// Riemann Mandelbrot initial settings
			settings.Init();
			rollAngle = 0;
			glCullFace(GL_BACK);

			keyState[GLFW_KEY_T] = GLFW_PRESS;
		}
		else if (glfwGetKey(window, GLFW_KEY_T) == GLFW_RELEASE)
			keyState[GLFW_KEY_T] = GLFW_RELEASE;

		// Display values or take screenshot
		if (JustPressed(GLFW_KEY_GRAVE_ACCENT))
		{
			if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
			{
				if (Texture::SaveScreenshot("D:/Documents/Test Animation", id++))
					std::cout << "\nSuccessfully saved image.\n";
				else
					std::cout << "\nFailed to save image.\n";
			}
			else
			{
				std::cout << "PARAMETERS:\n"
					      << "\tCenter: " << settings.center.x << ", " << settings.center.y << "\n"
						  << "\tRoll Angle: " << rollAngle << "\n"
						  << "\tInitial Radius: " << settings.radius << "\n"
						  << "\tZoom Factor: " << settings.zoom << "\n"
						  << "\tMaximum Iterations: " << settings.maxIterations << "\n"
						  << "\tBailout:\n"
						  << "\t\tX: " << settings.bailout.x << "\n"
						  << "\t\tY: " << settings.bailout.y << "\n"
						  << "\t\tZ: " << settings.bailout.z << "\n"
						  << "\t\tW: " << settings.bailout.w << "\n"
						  << "\tPower: " << settings.power << "\n"
						  << "\tC Power: " << settings.c_power << "\n"
						  << "\tIs Conjugate: " << (settings.isConjugate ? "YES" : "NO") << "\n"
						  << "\tFractal Type: " << FracToStr(settings.fractalType) << "\n"
						  << "\tOrbit Trap: " << TrapToStr(settings.orbitTrap) << "\n"
						  << "\tExterior Coloring: " << ColToStr(settings.exteriorColoring) << "\n"
						  << "\tInterior Coloring: " << ColToStr(settings.interiorColoring) << "\n"
						  << "\tFold Angle: " << settings.foldAngle << "\n"
						  << "\tFold Count: " << settings.foldCount << "\n"
						  << "\tFold Offset: " << settings.foldOffset.x << ", " << settings.foldOffset.y << "\n"
						  << "\tProjecting onto Riemann Sphere: " << (settings.useRiemannSphere ? "YES" : "NO") << "\n";

				if (settings.useRiemannSphere)
				{
					std::cout << "\t\tUsing Lighting: " << (settings.useLighting ? "YES" : "NO") << "\n"
							  << "\t\tUsing Terrain Color: " << (settings.useTerrainColor ? "YES" : "NO") << "\n"
							  << "\t\tTerrain Scale: " << settings.terrain << "\n";
				}
				if (settings.useDistance)
				{
					std::cout << "\tMaximum Distance: " << settings.maxDistance << "\n"
							  << "\tDistance Fineness: " << settings.distFineness << "\n";
				}
				if (settings.isJulia)
				{
					std::cout << "\tJulia Set:\n"
							  << "\t\tValue: " << settings.julia.x << ", " << settings.julia.y << "\n"
							  << "\t\tCentered at Critical Point: " << (settings.isJuliaCentered ? "YES" : "NO") << "\n";
				}	  
			}
			

			keyState[GLFW_KEY_GRAVE_ACCENT] = GLFW_PRESS;
		}
		else if (glfwGetKey(window, GLFW_KEY_GRAVE_ACCENT) == GLFW_RELEASE)
			keyState[GLFW_KEY_GRAVE_ACCENT] = GLFW_RELEASE;



		// Toggle Riemann Sphere
		if (JustPressed(GLFW_KEY_SPACE))
		{
			settings.useRiemannSphere = !settings.useRiemannSphere;
			/**/
			if (settings.useRiemannSphere)
				glCullFace(GL_FRONT);
			else
				glCullFace(GL_BACK);
			
			keyState[GLFW_KEY_SPACE] = GLFW_PRESS;
		}
		else if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE)
			keyState[GLFW_KEY_SPACE] = GLFW_RELEASE;

		// Toggle terrain
		if (glfwGetKey(window, GLFW_KEY_PERIOD) == GLFW_PRESS)
			settings.terrain += change / 10.f;

		// Toggle terrain color
		if (JustPressed(GLFW_KEY_COMMA))
		{
			settings.useTerrainColor = !settings.useTerrainColor;

			keyState[GLFW_KEY_COMMA] = GLFW_PRESS;
		}
		else if (glfwGetKey(window, GLFW_KEY_COMMA) == GLFW_RELEASE)
			keyState[GLFW_KEY_COMMA] = GLFW_RELEASE;

		// Toggle 3D lighting
		if (JustPressed(GLFW_KEY_SLASH))
		{
			settings.useLighting = !settings.useLighting;

			keyState[GLFW_KEY_SLASH] = GLFW_PRESS;
		}
		else if (glfwGetKey(window, GLFW_KEY_SLASH) == GLFW_RELEASE)
			keyState[GLFW_KEY_SLASH] = GLFW_RELEASE;

		// Change fractal type
		if (JustPressed(GLFW_KEY_ENTER))
		{
			if (glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS)
				settings.fractalType--;
			else
				settings.fractalType++;

			if (settings.fractalType >= FRAC_COUNT)
				settings.fractalType = 0;
			if (settings.fractalType < 0)
				settings.fractalType = FRAC_COUNT - 1;

			keyState[GLFW_KEY_ENTER] = GLFW_PRESS;
		}
		else if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_RELEASE)
			keyState[GLFW_KEY_ENTER] = GLFW_RELEASE;

		// Change exterior coloring
		if (JustPressed(GLFW_KEY_LEFT_BRACKET))
		{
			if (glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS)
				settings.interiorColoring--;
			else
				settings.interiorColoring++;

			if (settings.interiorColoring >= COL_COUNT)
				settings.interiorColoring = 0;
			if (settings.interiorColoring < 0)
				settings.interiorColoring = COL_COUNT - 1;

			keyState[GLFW_KEY_LEFT_BRACKET] = GLFW_PRESS;
		}
		else if (glfwGetKey(window, GLFW_KEY_LEFT_BRACKET) == GLFW_RELEASE)
			keyState[GLFW_KEY_LEFT_BRACKET] = GLFW_RELEASE;
		// Change interior coloring
		if (JustPressed(GLFW_KEY_RIGHT_BRACKET))
		{
			if (glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS)
				settings.exteriorColoring--;
			else
				settings.exteriorColoring++;

			if (settings.exteriorColoring >= COL_COUNT)
				settings.exteriorColoring = 0;
			if (settings.exteriorColoring < 0)
				settings.exteriorColoring = COL_COUNT - 1;

			keyState[GLFW_KEY_RIGHT_BRACKET] = GLFW_PRESS;
		}
		else if (glfwGetKey(window, GLFW_KEY_RIGHT_BRACKET) == GLFW_RELEASE)
			keyState[GLFW_KEY_RIGHT_BRACKET] = GLFW_RELEASE;

		// Change orbit trap
		if (JustPressed(GLFW_KEY_BACKSLASH))
		{
			if (glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS)
				settings.orbitTrap--;
			else
				settings.orbitTrap++;

			if (settings.orbitTrap >= TRAP_COUNT)
				settings.orbitTrap = 0;
			if (settings.orbitTrap < 0)
				settings.orbitTrap = TRAP_COUNT - 1;

			keyState[GLFW_KEY_BACKSLASH] = GLFW_PRESS;
		}
		else if (glfwGetKey(window, GLFW_KEY_BACKSLASH) == GLFW_RELEASE)
			keyState[GLFW_KEY_BACKSLASH] = GLFW_RELEASE;

		// Toggle julia set
		if (JustPressed(GLFW_KEY_TAB))
		{
			settings.isJulia = !settings.isJulia;

			keyState[GLFW_KEY_TAB] = GLFW_PRESS;
		}
		else if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_RELEASE)
			keyState[GLFW_KEY_TAB] = GLFW_RELEASE;

		// Toggle center on julia critical point
		if (JustPressed(GLFW_KEY_BACKSPACE))
		{
			settings.isJuliaCentered = !settings.isJuliaCentered;

			keyState[GLFW_KEY_BACKSPACE] = GLFW_PRESS;
		}
		else if (glfwGetKey(window, GLFW_KEY_BACKSPACE) == GLFW_RELEASE)
			keyState[GLFW_KEY_BACKSPACE] = GLFW_RELEASE;

		float rad = rollAngle * M_PI/180;
		float rad90 = rad +  M_PI/2;

		// Complex plane movement
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS && (!settings.useRiemannSphere || glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS))
			settings.center -= change * zoomFactor * glm::vec2(cos(rad), sin(rad));
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS && (!settings.useRiemannSphere || glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS))
			settings.center += change * zoomFactor * glm::vec2(cos(rad), sin(rad));
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS && (!settings.useRiemannSphere || glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS))
			settings.center -= change * zoomFactor * glm::vec2(cos(rad90), sin(rad90));
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS && (!settings.useRiemannSphere || glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS))
			settings.center += change * zoomFactor * glm::vec2(cos(rad90), sin(rad90));
		if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS && (!settings.useRiemannSphere || glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS))
			rollAngle += change * 50;
		else if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS && (!settings.useRiemannSphere || glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS))
			rollAngle -= change * 50;
		else
			rollAngle = (int)((rollAngle + (rollAngle > 0 ? 2 : -2)) / 5) * 5;

		// Julia position movement
		if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
			settings.julia.x -= change * zoomFactor;
		if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
			settings.julia.x += change * zoomFactor;
		if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
			settings.julia.y -= change * zoomFactor;
		if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
			settings.julia.y += change * zoomFactor;
		if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
			settings.zoom -= change;
		if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
			settings.zoom += change;

		// Animation control
		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
			animate = -1;
		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
			animate = 0;
		if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
			animate = 1;

		// Mode

		// Settings
		if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
			settings.maxIterations += change * 10;
		if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
			settings.bailout.x += change * 10;
		if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
			settings.maxDistance += change * 10;
		if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
			settings.distFineness += change;

		if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)
			settings.power += change * zoomFactor;
		else
			settings.power = (int)(settings.power * 10) / 10.f;
		if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS)
			settings.c_power += change * zoomFactor;
		else
			settings.c_power = (int)(settings.c_power * 10) / 10.f;


		if (glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS)
			settings.foldAngle += change * 50 * zoomFactor;
		else
			settings.foldAngle = (int)(settings.foldAngle * 100) / 100.f;
		if (glfwGetKey(window, GLFW_KEY_8) == GLFW_PRESS)
			settings.foldCount += change * zoomFactor;
		else
			settings.foldCount = (int)(settings.foldCount * 100) / 100.f;
		/*
		if (JustPressed(GLFW_KEY_8))
		{
			if (glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS)
				settings.foldCount--;
			else
				settings.foldCount++;

			if (settings.foldCount >= 20)
				settings.foldCount = 20;
			if (settings.foldCount < 0)
				settings.foldCount = 0;

			keyState[GLFW_KEY_8] = GLFW_PRESS;
		}
		else if (glfwGetKey(window, GLFW_KEY_8) == GLFW_RELEASE)
			keyState[GLFW_KEY_8] = GLFW_RELEASE;
		*/
		if (glfwGetKey(window, GLFW_KEY_9) == GLFW_PRESS)
			settings.foldOffset.x += change * zoomFactor / 5.f;
		else
			settings.foldOffset.x = (int)(settings.foldOffset.x * 10) / 10.f;
		if (glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS)
			settings.foldOffset.y += change * zoomFactor / 5.f;
		else
			settings.foldOffset.y = (int)(settings.foldOffset.y * 10) / 10.f;


		if (JustPressed(GLFW_KEY_EQUAL))
		{
			settings.isConjugate = !settings.isConjugate;

			keyState[GLFW_KEY_EQUAL] = GLFW_PRESS;
		}
		else if (glfwGetKey(window, GLFW_KEY_EQUAL) == GLFW_RELEASE)
			keyState[GLFW_KEY_EQUAL] = GLFW_RELEASE;

		if (JustPressed(GLFW_KEY_MINUS))
		{
			settings.useDistance = !settings.useDistance;

			keyState[GLFW_KEY_MINUS] = GLFW_PRESS;
		}
		else if (glfwGetKey(window, GLFW_KEY_MINUS) == GLFW_RELEASE)
			keyState[GLFW_KEY_MINUS] = GLFW_RELEASE;
	}
}

/*********************************************************************
** Function: key_callback
** Description: whenever a key on the keyboard gets pressed, this callback function executes
** Parameters: GLFWwindow*, int, int, int, int
*********************************************************************/
void Window::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{

}


/*********************************************************************
** Function: mouse_callback
** Description: whenever the mouse moves within the window, this callback function executes
** Parameters: GLFWwindow*, double, double
*********************************************************************/
void Window::mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastMouseX = xpos;
		lastMouseY = ypos;
		firstMouse = false;
	}

	deltaX = xpos - lastMouseX;
	deltaY = lastMouseY - ypos;

	lastMouseX = xpos;
	lastMouseY = ypos;
}

/*********************************************************************
** Function: framebuffer_size_callback
** Description: whenever the window size changed (by OS or user resize) this callback function executes
** Parameters: GLFWwindow*, int, int
*********************************************************************/
void Window::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);

	Window* obj = (Window*) glfwGetWindowUserPointer(window);
	obj->width = width;
	obj->height = height;
	obj->camera->SetSetting("aspectRatio", (float) width/height);
}

/*********************************************************************
** Function: scroll_callback
** Description: whenever the mouse's scroll wheel moves within the window, this callback function executes
** Parameters: GLFWwindow*, double, double
*********************************************************************/
void Window::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	scrollOffset = yoffset;
}

/*********************************************************************
** Function: close_callback
** Description: whenever the mouse's scroll wheel moves within the window, this callback function executes
** Parameters: GLFWwindow*, double, double
*********************************************************************/
void Window::close_callback(GLFWwindow* window)
{
	//_CrtDumpMemoryLeaks();
	//std::cin.get();

	/*
	if (!time_to_close)
		glfwSetWindowShouldClose(window, GLFW_FALSE);
	*/
}

glm::vec2 Sqrt(glm::vec2 c, bool switchSign = false)
{
	if (isnan(c.x) || isnan(c.y) || isinf(c.x) || isinf(c.y))
		return glm::vec2(pow(2, 32), 0);

	float r = glm::length(c);

	//return new Complex(.5 * Math.Sqrt(2 * (r + z.R)), .5 * Sign(new Complex(z.I, -r)) * Math.Sqrt(2 * (r - z.R)));
	//return .5 * Math.Sqrt(2) * new Complex(Math.Sqrt(r + z.R), Math.Sign(z.I) * Math.Sqrt(r - z.R));
	return (switchSign ? -1 : 1) * .5f * (float)sqrt(2) * glm::vec2(sqrt(r + c.x), (c.y > 0 ? 1 : -1) * sqrt(r - c.x));
}

void Window::UpdateFractal(float deltaTime)
{
	float zoomFactor = pow(2, settings.zoom * (settings.power < 0 ? -1 : 1));
	
	if (zoomFactor < 1)
			zoomFactor = 1;

	float powerFactor = abs(settings.power) - 1;

	if (powerFactor <= 1)
		powerFactor = 1;

	animTime += (animate == 0 ? 0 : (animate == 1 ? deltaTime : -deltaTime)) / zoomFactor / powerFactor;

	// Julia Set animations
	if (settings.isJulia && animate != 0)
	{
		// Circle
		float radius = M_PI / 4;

		settings.julia = glm::vec2(radius * cos(animTime), radius * sin(animTime));



		// Center Julia Set
		if (settings.isJuliaCentered)
		{
			bool switchSign = fmod(animTime, 4 * M_PI) >= 2 * M_PI;
			glm::vec2 center = (glm::vec2(1, 0) - Sqrt(glm::vec2(1, 0) - (4.f * settings.julia), switchSign)) / 2.f;
			settings.center = center;
		}

		//std::cout << "animTime: " << animTime << "\n";
	}
}


/*********************************************************************
** Function: RenderLoop
** Description: Render loop
** Parameters: GLFWwindow*, int, int
*********************************************************************/
void Window::RenderLoop()
{
	// Lighting
	glm::vec3 pointLightPos[] =
	{
		//glm::vec3(0.7f,  1.2f,  2.0f),
		//glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.3f, -3.3f, -4.0f),
		glm::vec3(2.0f, 0.1f, 0.0f),
		glm::vec3(-4.0f,  2.0f, -12.0f),
		glm::vec3(0.0f,  0.0f, -3.0f)
	};

	// Set lighting
	{
		mandelbrot->Use();
		{
			// directional light
			mandelbrot->SetInt("dirLight.type", 1);
			mandelbrot->SetVec3("dirLight.direction", 0.0f, -1.0f, -0.0f);
			mandelbrot->SetVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
			mandelbrot->SetVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
			mandelbrot->SetVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
			// point light 1
			mandelbrot->SetInt("pointLights[0].type", 2);
			mandelbrot->SetVec3("pointLights[0].position", pointLightPos[0]);
			mandelbrot->SetVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
			//shader->SetVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
			mandelbrot->SetVec3("pointLights[0].diffuse", 0.8f, 0.6f, 0.0f);
			//shader->SetVec3("pointLights[0].specular", 0.8f, 0.8f, 0.8f);
			mandelbrot->SetVec3("pointLights[0].specular", 0.8f, 0.6f, 0.0f);
			mandelbrot->SetFloat("pointLights[0].constant", 1.0f);
			mandelbrot->SetFloat("pointLights[0].linear", 0.09);
			mandelbrot->SetFloat("pointLights[0].quadratic", 0.032);
			// point light 2
			mandelbrot->SetInt("pointLights[1].type", 2);
			mandelbrot->SetVec3("pointLights[1].position", pointLightPos[1]);
			mandelbrot->SetVec3("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
			mandelbrot->SetVec3("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
			mandelbrot->SetVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
			mandelbrot->SetFloat("pointLights[1].constant", 1.0f);
			mandelbrot->SetFloat("pointLights[1].linear", 0.09);
			mandelbrot->SetFloat("pointLights[1].quadratic", 0.032);
			// point light 3
			mandelbrot->SetInt("pointLights[2].type", 2);
			mandelbrot->SetVec3("pointLights[2].position", pointLightPos[2]);
			mandelbrot->SetVec3("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
			mandelbrot->SetVec3("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
			mandelbrot->SetVec3("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
			mandelbrot->SetFloat("pointLights[2].constant", 1.0f);
			mandelbrot->SetFloat("pointLights[2].linear", 0.09);
			mandelbrot->SetFloat("pointLights[2].quadratic", 0.032);
			// point light 4
			mandelbrot->SetInt("pointLights[3].type", 2);
			mandelbrot->SetVec3("pointLights[3].position", pointLightPos[3]);
			mandelbrot->SetVec3("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
			mandelbrot->SetVec3("pointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
			mandelbrot->SetVec3("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
			mandelbrot->SetFloat("pointLights[3].constant", 1.0f);
			mandelbrot->SetFloat("pointLights[3].linear", 0.09);
			mandelbrot->SetFloat("pointLights[3].quadratic", 0.032);
			// spotLight
			mandelbrot->SetInt("spotLight.type", 3);
			mandelbrot->SetVec3("spotLight.position", camera->GetPosition());
			mandelbrot->SetVec3("spotLight.direction", camera->Direction());
			mandelbrot->SetVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
			mandelbrot->SetVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
			mandelbrot->SetVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
			mandelbrot->SetFloat("spotLight.constant", 1.0f);
			//shader->SetFloat("spotLight.linear", 0.09);
			mandelbrot->SetFloat("spotLight.linear", 0.15);
			mandelbrot->SetFloat("spotLight.quadratic", 0.032);
			mandelbrot->SetFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
			mandelbrot->SetFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));

			// material properties
			mandelbrot->SetFloat("material.shininess", 64.0f);

			/*
			shader->SetVec3("material.ambient", 1.0f, 0.5f, 0.31f);
			shader->SetVec3("material.diffuse", 1.0f, 0.5f, 0.31f);
			shader->SetVec3("material.specular", 0.5f, 0.5f, 0.5f); // specular lighting doesn't have full effect on this object's material
			*/
		}
		julia->Use();
		{
			// directional light
			julia->SetInt("dirLight.type", 1);
			julia->SetVec3("dirLight.direction", 0.0f, -1.0f, -0.0f);
			julia->SetVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
			julia->SetVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
			julia->SetVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
			// point light 1
			julia->SetInt("pointLights[0].type", 2);
			julia->SetVec3("pointLights[0].position", pointLightPos[0]);
			julia->SetVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
			//shader->SetVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
			julia->SetVec3("pointLights[0].diffuse", 0.8f, 0.6f, 0.0f);
			//shader->SetVec3("pointLights[0].specular", 0.8f, 0.8f, 0.8f);
			julia->SetVec3("pointLights[0].specular", 0.8f, 0.6f, 0.0f);
			julia->SetFloat("pointLights[0].constant", 1.0f);
			julia->SetFloat("pointLights[0].linear", 0.09);
			julia->SetFloat("pointLights[0].quadratic", 0.032);
			// point light 2
			julia->SetInt("pointLights[1].type", 2);
			julia->SetVec3("pointLights[1].position", pointLightPos[1]);
			julia->SetVec3("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
			julia->SetVec3("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
			julia->SetVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
			julia->SetFloat("pointLights[1].constant", 1.0f);
			julia->SetFloat("pointLights[1].linear", 0.09);
			julia->SetFloat("pointLights[1].quadratic", 0.032);
			// point light 3
			julia->SetInt("pointLights[2].type", 2);
			julia->SetVec3("pointLights[2].position", pointLightPos[2]);
			julia->SetVec3("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
			julia->SetVec3("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
			julia->SetVec3("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
			julia->SetFloat("pointLights[2].constant", 1.0f);
			julia->SetFloat("pointLights[2].linear", 0.09);
			julia->SetFloat("pointLights[2].quadratic", 0.032);
			// point light 4
			julia->SetInt("pointLights[3].type", 2);
			julia->SetVec3("pointLights[3].position", pointLightPos[3]);
			julia->SetVec3("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
			julia->SetVec3("pointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
			julia->SetVec3("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
			julia->SetFloat("pointLights[3].constant", 1.0f);
			julia->SetFloat("pointLights[3].linear", 0.09);
			julia->SetFloat("pointLights[3].quadratic", 0.032);
			// spotLight
			julia->SetInt("spotLight.type", 3);
			julia->SetVec3("spotLight.position", camera->GetPosition());
			julia->SetVec3("spotLight.direction", camera->Direction());
			julia->SetVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
			julia->SetVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
			julia->SetVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
			julia->SetFloat("spotLight.constant", 1.0f);
			//shader->SetFloat("spotLight.linear", 0.09);
			julia->SetFloat("spotLight.linear", 0.15);
			julia->SetFloat("spotLight.quadratic", 0.032);
			julia->SetFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
			julia->SetFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));

			// material properties
			julia->SetFloat("material.shininess", 64.0f);

			/*
			shader->SetVec3("material.ambient", 1.0f, 0.5f, 0.31f);
			shader->SetVec3("material.diffuse", 1.0f, 0.5f, 0.31f);
			shader->SetVec3("material.specular", 0.5f, 0.5f, 0.5f); // specular lighting doesn't have full effect on this object's material
			*/
		}
	}

	while (!glfwWindowShouldClose(this->window))
	{
		// per-frame time logic
		// --------------------
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
			deltaTime *= (glfwGetKey(window, GLFW_KEY_RIGHT_ALT) == GLFW_PRESS) ? -10 : 10;

		gameTime += deltaTime;

		UpdateFractal(deltaTime);

		// input
		// -----
		ProcessInput(deltaTime);
		// render
		// ------
		glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if (settings.useRiemannSphere)
			camera->Render();

		// Fractal Display
		{
			Shader* currentShader = settings.isJulia ? julia : mandelbrot;
			currentShader->Use();

			// Shader variables
			currentShader->SetMat4("projection", camera->GetProjectionMatrix());
			currentShader->SetMat4("view", camera->GetViewMatrix());
			currentShader->SetMat4("model", glm::mat4(1));

			currentShader->SetVec3("eyePos", camera->GetPosition());
			currentShader->SetBool("lighting", settings.useLighting);
			currentShader->SetBool("riemannSphere", settings.useRiemannSphere);
			currentShader->SetBool("terrainColor", settings.useTerrainColor);
			currentShader->SetFloat("terrain", settings.terrain);

			glm::vec2 r = settings.radius * glm::vec2((float)width / height, 1);

			currentShader->SetFloat("time", gameTime);
			currentShader->SetFloat("zoom", pow(2, settings.zoom));
			currentShader->SetInt("maxIterations", settings.maxIterations);
			currentShader->SetVec4("bailout", settings.bailout);
			currentShader->SetVec2("center", settings.center);
			currentShader->SetVec2("radius", r);
			currentShader->SetFloat("power", settings.power);
			currentShader->SetFloat("c_power", settings.c_power);
			currentShader->SetBool("isConjugate", settings.isConjugate);
			currentShader->SetBool("useDistance", settings.useDistance);
			currentShader->SetFloat("maxDistance", settings.maxDistance);
			currentShader->SetFloat("distFineness", settings.distFineness);
			currentShader->SetInt("fractalType", settings.fractalType);
			currentShader->SetInt("orbitTrap", settings.orbitTrap);
			currentShader->SetInt("exteriorColoring", settings.exteriorColoring);
			currentShader->SetInt("interiorColoring", settings.interiorColoring);

			currentShader->SetVec2("julia", settings.julia);
			currentShader->SetFloat("foldCount", settings.foldCount);
			currentShader->SetFloat("foldAngle", glm::radians(settings.foldAngle));
			currentShader->SetVec2("foldOffset", settings.foldOffset);

			currentShader->SetFloat("rollAngle", rollAngle);

			// Render plane
			if (settings.useRiemannSphere)
			{
				glBindVertexArray(VAO_sphere);
				glDrawArrays(GL_PATCHES, 0, icosphere->GetVertexCount());
			}
			else
			{
				glBindVertexArray(VAO_plane);
				glDrawArrays(GL_PATCHES, 0, 6);
			}
		}

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}
