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

float animTime = 0;
int animate = 0;


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

	this->keyState.insert({ GLFW_KEY_W, GLFW_RELEASE });			// move up on julia coordinate
	this->keyState.insert({ GLFW_KEY_A, GLFW_RELEASE });			// move left on julia coordinate
	this->keyState.insert({ GLFW_KEY_S, GLFW_RELEASE });			// move down on julia coordinate
	this->keyState.insert({ GLFW_KEY_D, GLFW_RELEASE });			// move right on julia coordinate
	this->keyState.insert({ GLFW_KEY_I, GLFW_RELEASE });			// move up on complex plane
	this->keyState.insert({ GLFW_KEY_J, GLFW_RELEASE });			// move left on complex plane
	this->keyState.insert({ GLFW_KEY_K, GLFW_RELEASE });			// move down on complex plane
	this->keyState.insert({ GLFW_KEY_L, GLFW_RELEASE });			// move right on complex plane
	this->keyState.insert({ GLFW_KEY_U, GLFW_RELEASE });			// zoom in on complex plane
	this->keyState.insert({ GLFW_KEY_O, GLFW_RELEASE });			// zoom out on complex plane
	this->keyState.insert({ GLFW_KEY_LEFT, GLFW_RELEASE });			// continue animation
	this->keyState.insert({ GLFW_KEY_DOWN, GLFW_RELEASE });			// stop animation
	this->keyState.insert({ GLFW_KEY_RIGHT, GLFW_RELEASE });		// reverse animation
	this->keyState.insert({ GLFW_KEY_1, GLFW_RELEASE });			// change maxIterations
	this->keyState.insert({ GLFW_KEY_2, GLFW_RELEASE });			// change bailout
	this->keyState.insert({ GLFW_KEY_3, GLFW_RELEASE });			// change maxDistance
	this->keyState.insert({ GLFW_KEY_4, GLFW_RELEASE });			// change distFineness
	this->keyState.insert({ GLFW_KEY_LEFT_CONTROL, GLFW_RELEASE });
	this->keyState.insert({ GLFW_KEY_LEFT_SHIFT, GLFW_RELEASE });
	this->keyState.insert({ GLFW_KEY_LEFT_ALT, GLFW_RELEASE });
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
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, this->major);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, this->minor);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif


	this->window = glfwCreateWindow(this->width, this->height, "GLFW Mandelbrot Project", NULL, NULL);

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


	deltaX = 0;
	deltaY = 0;
	lastMouseX = this->width / 2;
	lastMouseY = this->height / 2;

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

	// Riemann Mandelbrot initial settings
	settings.Init();
	//settings.center = glm::vec2(0.3660809, -0.3140693);
	//settings.radius = 1;
	//settings.isJuliaCentered = true;
	settings.isJulia = true;
	//settings.useDistance = true;
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
	mandelbrot = new Shader("Shaders/fractal_plane.vert", "Shaders/julia.frag");
	julia = new Shader("Shaders/fractal_plane.vert", "Shaders/julia.frag");
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

	// Mouse controls
	{
		bool leftClick = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;
		bool rightClick = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS;

		if (rightClick)
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		else
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

		if (deltaX != 0 || deltaY != 0)
		{
			if (rightClick)
			{
				settings.center -= glm::vec2(deltaX, deltaY) * zoomFactor / 100.f;

				glm::vec2 cX = glm::vec2(settings.center_ED.x, settings.center_ED.y);
				glm::vec2 cY = glm::vec2(settings.center_ED.z, settings.center_ED.w);
				//glm::vec2 dX = ED_mul(ED_set(deltaX), ED_div(settings.zoom_ED, ED_set(100.f)));
				//glm::vec2 dY = ED_mul(ED_set(deltaY), ED_div(settings.zoom_ED, ED_set(100.f)));
				glm::vec2 dX = ED_mul(ED_set(deltaX), ED_set(zoomFactor / 100.f));
				glm::vec2 dY = ED_mul(ED_set(deltaY), ED_set(zoomFactor / 100.f));
				settings.center_ED = glm::vec4(ED_sub(cX, dX), ED_sub(cY, dY));
			}
				
			
			deltaX = 0;
			deltaY = 0;
		}

		if (scrollOffset != 0)
		{
			glm::vec2 normalizedMousePos = glm::vec2((float)lastMouseX / width, (float)lastMouseY / height) * 2.f - 1.f;
			glm::vec2 aspectRatio = glm::vec2((float)width / height, -1);

			glm::vec2 mousePos = settings.center + normalizedMousePos * settings.radius * aspectRatio / (float)pow(2,settings.zoom);
			settings.zoom += scrollOffset / 10.f;
			settings.center = mousePos - normalizedMousePos * settings.radius * aspectRatio / (float)pow(2, settings.zoom);



			glm::vec2 normalizedMousePosX = ED_set(normalizedMousePos.x);
			glm::vec2 normalizedMousePosY = ED_set(normalizedMousePos.y);
			glm::vec2 aspectRatioX = ED_set(aspectRatio.x);
			glm::vec2 aspectRatioY = ED_set(aspectRatio.y);

			glm::vec2 cX = glm::vec2(settings.center_ED.x, settings.center_ED.y);
			glm::vec2 cY = glm::vec2(settings.center_ED.z, settings.center_ED.w);

			glm::vec2 mousePosX = ED_add(cX, ED_mul(normalizedMousePosX, ED_mul(settings.radius_ED, ED_div(aspectRatioX, ED_set(pow(2, settings.zoom - scrollOffset/10.f))))));
			glm::vec2 mousePosY = ED_add(cY, ED_mul(normalizedMousePosY, ED_mul(settings.radius_ED, ED_div(aspectRatioY, ED_set(pow(2, settings.zoom - scrollOffset/10.f))))));
			//settings.zoom_ED = ED_add(settings.zoom_ED, ED_set(scrollOffset / 10.f));
			//settings.zoom += scrollOffset / 10.f;
			cX = ED_sub(mousePosX, ED_mul(normalizedMousePosX, ED_mul(settings.radius_ED, ED_div(aspectRatioX, ED_set(pow(2, settings.zoom))))));
			cY = ED_sub(mousePosY, ED_mul(normalizedMousePosY, ED_mul(settings.radius_ED, ED_div(aspectRatioY, ED_set(pow(2, settings.zoom))))));

			settings.center_ED = glm::vec4(cX, cY);

			std::cout << settings.zoom << "\n";
			//std::cout << settings.center.x << ", " << settings.center.y << "\n";
			//std::cout << "\t" << settings.center_ED.x << ", " << settings.center_ED.y << "\n";
			//std::cout << "\t" << settings.center_ED.z << ", " << settings.center_ED.w << "\n";

			scrollOffset = 0;
		}
	}

	// Keyboard controls
	{
		int negative = 1;
		float change = 1 * deltaInput;

		// Toggle lighting
		/*
		if (JustPressed(GLFW_KEY_0))
		{
			lighting = !lighting;

			keyState[GLFW_KEY_0] = GLFW_PRESS;
		}
		else if (glfwGetKey(window, GLFW_KEY_0) == GLFW_RELEASE)
			keyState[GLFW_KEY_0] = GLFW_RELEASE;
		*/
		// Change speed
		if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
			change /= 4;
		if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
			change *= 4;
		if (glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS)
			change *= -1;

		// Complex plane movement
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			settings.center.x -= change * zoomFactor;
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			settings.center.x += change * zoomFactor;
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			settings.center.y -= change * zoomFactor;
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			settings.center.y += change * zoomFactor;
		if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
			settings.zoom -= change;
		if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
			settings.zoom += change;

		// Julia position movement
		if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
			settings.julia.x -= change * zoomFactor;
		if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
			settings.julia.x += change * zoomFactor;
		if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
			settings.julia.y -= change * zoomFactor;
		if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
			settings.julia.y += change * zoomFactor;

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
			settings.bailout += change * 10;
		if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
			settings.maxDistance += change * 10;
		if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
			settings.distFineness += change;
		/*
		std::cout << "zoom: " << settings.zoom << "\n"
			<< "maxIterations: " << settings.maxIterations << "\n"
			<< "bailout: " << settings.bailout << "\n"
			<< "maxDistance: " << settings.maxDistance << "\n"
			<< "distFineness: " << settings.distFineness << "\n"
			<< "\n";
		*/
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
	animTime += (animate == 0 ? 0 : (animate == 1 ? deltaTime : -deltaTime)) / pow(2, settings.zoom);

	// Julia Set animations
	if (settings.isJulia)
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
	while (!glfwWindowShouldClose(this->window))
	{
		// per-frame time logic
		// --------------------
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		UpdateFractal(deltaTime);

		// input
		// -----
		ProcessInput(deltaTime);
		// render
		// ------
		glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Fractal Display
		{
			Shader* currentShader = settings.isJulia ? julia : mandelbrot;
			currentShader->Use();

			// Shader variables
			glm::vec2 r = settings.radius * glm::vec2((float) width/height, 1);

			currentShader->SetFloat("time", currentFrame);
			currentShader->SetFloat("zoom", settings.zoom);
			currentShader->SetInt("maxIterations", settings.maxIterations);
			currentShader->SetFloat("bailout", settings.bailout);
			currentShader->SetVec2("center", settings.center);
			currentShader->SetVec2("radius", r);
			currentShader->SetBool("useDistance", settings.useDistance);
			currentShader->SetFloat("maxDistance", settings.maxDistance);
			currentShader->SetFloat("distFineness", settings.distFineness);

			if (settings.isJulia)
				currentShader->SetVec2("julia", settings.julia);


			// Emulated double
			//currentShader->SetVec2("zoom_ED", ED_set(pow(2, settings.zoom)));
			currentShader->SetVec2("zoom_ED", settings.zoom_ED);

			glm::vec4 tmp;
			tmp.x = (float)settings.center.x;
			tmp.y = settings.center.x - (double)tmp.x;
			tmp.z = (float)settings.center.y;
			tmp.w = settings.center.y - (double)tmp.z;
			//currentShader->SetVec4("center_ED", tmp);
			currentShader->SetVec4("center_ED", settings.center_ED);

			tmp.x = (float)r.x;
			tmp.y = r.x - (double)tmp.x;
			tmp.z = (float)r.y;
			tmp.w = r.y - (double)tmp.z;
			//currentShader->SetVec4("radius_ED", tmp);
			glm::vec2 tmpX = ED_mul(settings.radius_ED, ED_set((float) width/height));
			glm::vec2 tmpY = ED_mul(settings.radius_ED, ED_set(1));
			currentShader->SetVec4("radius_ED", glm::vec4(tmpX,tmpY));

			// testing
			{
				/*
				glm::vec2 aPos = glm::vec2(.5, .5);

				glm::vec2 FragPos = r * aPos / (float)pow(2, (float)settings.zoom) + settings.center;

				//glm::vec2 z = ED_set(1.f / (float) pow(2, (float) settings.zoom));
				glm::vec2 z = ED_set(pow(2, settings.zoom));
				glm::vec4 c = glm::vec4(ED_set(settings.center.x), ED_set(settings.center.y));
				glm::vec4 r4 = glm::vec4(ED_set(r.x), ED_set(r.y));
				glm::vec4 p = glm::vec4(ED_set(aPos.x), ED_set(aPos.y));

				glm::vec4 FragPos_ED = glm::vec4(ED_add(ED_div(ED_mul(glm::vec2(r4.x, r4.y), glm::vec2(p.x, p.y)), z), glm::vec2(c.x, c.y)), ED_add(ED_div(ED_mul(glm::vec2(r4.z, r4.w), glm::vec2(p.z, p.w)), z), glm::vec2(c.z, c.w)));

				glm::vec4 julia_ED = glm::vec4(ED_set(settings.julia.x), ED_set(settings.julia.y));
				*/
				

				int breakpoint = 0;
			}

			// Render plane
			glBindVertexArray(VAO_plane);
			glDrawArrays(GL_TRIANGLES, 0, 6);
		}

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}
