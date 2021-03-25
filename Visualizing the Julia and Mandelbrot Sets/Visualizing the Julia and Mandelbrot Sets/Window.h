/*********************************************************************
** Author: Ibrahim Mahmoud
** Date: 9/13/21
** Description: This is the header file for Window.h
*********************************************************************/

#ifndef WINDOW
#define WINDOW

#define DEBUG
#define _CRTDBG_MAP_ALLOC
#define _CRTDBG_MAP_ALLOC_NEW
#include <stdlib.h>  
#include <crtdbg.h>  

#include <iostream>
#include <map>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "stb_image.h"
#include "Shader.h"
#include "Texture.h"
#include "Plot.h"

#define CURVE_NUM 10.0
#define CURVE_RES 10

class Window
{
	private:
		GLFWwindow* window;

		int major;
		int minor;

		int width;
		int height;

		float deltaTime = 0;
		float lastFrame = 0;

		std::map<int, int> keyState;

		void ResetKeyState();

		bool JustPressed(int);

		void GladLoader();

		static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
		static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
		static void scroll_callback(GLFWwindow* window, double xpos, double ypos);
		static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
		static void close_callback(GLFWwindow* window);


	public:
		unsigned int VBO_plane, VAO_plane;
		Shader* julia;
		Shader* mandelbrot;

		Fractal settings;

		Window();
		Window(int, int, int = 4, int = 5);
		~Window();

		int GetWidth();
		int GetHeight();

		void Init();
		void Terminate();

		void InitObjects();
		void InitShaders();

		void UpdateFractal(float = 0);

		void RenderLoop();
		void ProcessInput(float = 0);

};

#endif