/*********************************************************************
** Author: Ibrahim Mahmoud
** Date: 9/22/21
** Description: This is the header file for Camera.h
*********************************************************************/

#ifndef CAMERA
#define CAMERA

#include <iostream>
#include <algorithm>
#include <string>
#include <map>
#include <math.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include "Shader.h"

class Camera
{
	private:
		GLFWwindow* window;

		glm::vec3 right = glm::vec3(1, 0, 0);
		glm::vec3 up = glm::vec3(0, 1, 0);
		glm::vec3 direction = glm::vec3(0, 0, -1);

		glm::vec3 position;
		glm::vec3 target = glm::vec3(0, 0, 0);
		float targetDistance;

		float yaw = glm::radians(-90.0f);
		float pitch = 0;

		std::map<std::string, int> keyMap;
		std::map<std::string, float> settings;

		unsigned int VBO_target, VAO_target;
		unsigned int VBO_axis, VAO_axis;

		Shader* shader;

		bool JustPressed(std::map<int, int>, std::string);

	public:
		enum Type
		{
			NONE = 0, FPS, FREE, X, Y, Z
		};

		const glm::vec3 unitRight = glm::vec3(1, 0, 0);
		const glm::vec3 unitUp = glm::vec3(0, 1, 0);
		const glm::vec3 unitFront = glm::vec3(0, 0, 1);

		glm::vec2 lastMousePos;
		float lastScrollPos;

		Camera(GLFWwindow*);
		Camera(GLFWwindow*, glm::vec3);
		~Camera();

		std::string ToString();

		void ResetSettings();
		void ResetKeyMap();

		void LoadObjects();
		void Render();

		glm::vec3 RightAxis();
		glm::vec3 UpAxis();
		glm::vec3 Direction();

		glm::vec3 GetPosition();
		void SetPosition(glm::vec3);

		float GetSetting(std::string);
		void SetSetting(std::string, float);

		float GetKey(std::string);
		void SetKey(std::string, int);

		float GetYaw();
		void SetYaw(float);
		void AdjustYaw(float);

		float GetPitch();
		void SetPitch(float);
		void AdjustPitch(float);

		glm::mat4 GetViewMatrix();

		glm::mat4 GetOrthographicMatrix(float, float);

		glm::mat4 GetProjectionMatrix();

		void KeyboardInput(std::map<int, int>&, float);
		void MouseInput(float, float, bool, bool);
		void ScrollInput(float);

		void Update(float, float);

		float GetRadius();

		void MoveAlongAxis(glm::vec3, bool = true);
		void MoveAlongAxis(glm::vec3, float, bool = true);
		static glm::vec3 MoveAlongAxis(glm::vec3, glm::vec3, float);

		//@TODO: adjust functions so that the zoom distance is handled within them
		//@TODO: also, why am I zooming without affecting the target?
		void Zoom(float = 1);
		void ZoomAlongAxis(glm::vec3, float);
		void ZoomTarget(glm::vec3, float, bool = true);

		void RotateX(float, bool = true);
		void RotateY(float, bool = true);
		void RotateZ(float, bool = true);

		void RotateYaw(float, bool = true);
		void RotatePitch(float, bool = true);
		void RotateRoll(float, bool = true);

		glm::vec3 Rotate(glm::vec3, glm::vec3, float) const;

		void ArcBall(glm::vec3, float, bool = true);
		void ArcBallYaw(float, bool = false);
		void ArcBallPitch(float, bool = false);

		void RotateVectors();
		void RotateVectors(glm::quat);

		void ApplyRotationToVector(glm::quat, glm::vec3&);

		void UpdateTarget();

		void UpdateTargetDistance();
};

#endif