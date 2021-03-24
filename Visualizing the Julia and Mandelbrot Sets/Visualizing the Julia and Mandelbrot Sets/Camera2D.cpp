/*********************************************************************
** Author: Ibrahim Mahmoud
** Date: 9/22/21
** Description: This is the implementation file for Camera.h
*********************************************************************/

#include "Camera2D.h"

/*********************************************************************
** Function: Camera
** Description: Base constructor for a Camera class object
** Parameters: none
** Pre-Conditions: The GLFW window must have already been created
** Post-Conditions: Camera is reset to default values
*********************************************************************/
Camera::Camera(GLFWwindow* window)
{
	this->window = window;

	this->position = glm::vec3(0.0, 0.0, 5.0);
	//this->position = glm::vec3(4.0f, 0.5f, 1.0);

	UpdateTargetDistance();

	ResetSettings();
	ResetKeyMap();

	shader = new Shader("Shaders/axis.vert", "Shaders/axis.frag");

	LoadObjects();
}
/*********************************************************************
** Function: Camera
** Description: Constructor for a Camera class object
** Parameters: none
** Pre-Conditions: The GLFW window must have already been created
** Post-Conditions: Camera is reset to default values
*********************************************************************/
Camera::Camera(GLFWwindow* window, glm::vec3 position)
{
	this->window = window;

	this->position = position;
	
	UpdateTargetDistance();

	ResetSettings();
	ResetKeyMap();

	shader = new Shader("Shaders/axis.vert", "Shaders/axis.frag");

	LoadObjects();
}
/*********************************************************************
** Function: ~Camera
** Description: Destructor for a Camera class object
** Parameters: none
** Post-Conditions: Frees the memory allocated for the shader
*********************************************************************/
Camera::~Camera()
{
	delete shader;
}

/*********************************************************************
** Function: ToString
** Description: Returns a string that displays the camera's position
**				and orientation
** Parameters: none
*********************************************************************/
std::string Camera::ToString()
{
	return "";
}

/*********************************************************************
** Function: ResetSettings
** Description: Resets settings to their original defaults.
** Parameters: none
*********************************************************************/
void Camera::ResetSettings()
{
	this->settings.clear();

	this->settings.insert({ "fov", glm::radians(57.0f) });
	this->settings.insert({ "aspectRatio", 1 });
	this->settings.insert({ "nearClipping", 0.1 });
	this->settings.insert({ "farClipping", 100 });
	this->settings.insert({ "fullscreen", 0 });
	this->settings.insert({ "lock", 0 });
	this->settings.insert({ "mode", FPS });
	this->settings.insert({ "toggleTarget", 0 });
	this->settings.insert({ "toggleAxis", 1 });
	this->settings.insert({ "moveSpeed", 2 });
	this->settings.insert({ "zoomSpeed", .1 });
	this->settings.insert({ "sensitivity", .2 });
}
/*********************************************************************
** Function: ResetKeyMap
** Description: Resets key mapping to their original defaults.
** Parameters: none
*********************************************************************/
void Camera::ResetKeyMap()
{
	this->keyMap.clear();

	this->keyMap.insert({ "fullscreen", GLFW_KEY_F11 });
	this->keyMap.insert({ "displayInfo", GLFW_KEY_SLASH });
	this->keyMap.insert({ "mode", GLFW_KEY_B });
	this->keyMap.insert({ "lock", GLFW_KEY_N });

	this->keyMap.insert({ "toggleTarget", GLFW_KEY_APOSTROPHE });
	this->keyMap.insert({ "toggleAxis", GLFW_KEY_SEMICOLON });

	this->keyMap.insert({ "moveForward", GLFW_KEY_W });
	this->keyMap.insert({ "moveBackward", GLFW_KEY_S });
	this->keyMap.insert({ "moveUp", GLFW_KEY_SPACE });
	this->keyMap.insert({ "moveDown", GLFW_KEY_LEFT_SHIFT });
	this->keyMap.insert({ "moveLeft", GLFW_KEY_A });
	this->keyMap.insert({ "moveRight", GLFW_KEY_D });

	this->keyMap.insert({ "zoomIn", GLFW_KEY_R });
	this->keyMap.insert({ "zoomOut", GLFW_KEY_F });

	this->keyMap.insert({ "rotateX", GLFW_KEY_X });
	this->keyMap.insert({ "rotateY", GLFW_KEY_Y });
	this->keyMap.insert({ "rotateZ", GLFW_KEY_Z });
}

/*********************************************************************
** Function: LoadObjects
** Description: Initializes the data for displaying the world axis
**				and camera's target
** Parameters: none
** Pre-Conditions: The GLFW window must have already been created
** Post-Conditions: Camera's VAO's will be ready to use
*********************************************************************/
void Camera::LoadObjects()
{
	float vertices_target[]
	{
		// Position			   Color
		-0.1f,  0.0f,  0.0f,   1.0f, 1.0f, 1.0f,
		 0.1f,  0.0f,  0.0f,   1.0f, 1.0f, 1.0f,
		 0.0f, -0.1f,  0.0f,   1.0f, 1.0f, 1.0f,
		 0.0f,  0.1f,  0.0f,   1.0f, 1.0f, 1.0f,
		 0.0f,  0.0f, -0.1f,   1.0f, 1.0f, 1.0f,
		 0.0f,  0.0f,  0.1f,   1.0f, 1.0f, 1.0f,
	};
	float vertices_axis[]
	{
		// Position			   Color
		-1.0f,  0.0f,  0.0f,   0.6f, 0.0f, 0.0f,
		 1.0f,  0.0f,  0.0f,   0.6f, 0.0f, 0.0f,
		 0.0f, -1.0f,  0.0f,   0.0f, 0.6f, 0.0f,
		 0.0f,  1.0f,  0.0f,   0.0f, 0.6f, 0.0f,
		 0.0f,  0.0f, -1.0f,   0.0f, 0.0f, 0.6f,
		 0.0f,  0.0f,  1.0f,   0.0f, 0.0f, 0.6f,
	};

	glGenVertexArrays(1, &VAO_target);
	glGenBuffers(1, &VBO_target);

	glBindVertexArray(VAO_target);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_target);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_target), vertices_target, GL_STATIC_DRAW);

	// position attribute
	int targetLocation = glGetAttribLocation(shader->ID, "aPosition");
	glVertexAttribPointer(targetLocation, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(targetLocation);

	// color attribute
	int targetColor = glGetAttribLocation(shader->ID, "aColor");
	glVertexAttribPointer(targetColor, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) (3 * sizeof(float)));
	glEnableVertexAttribArray(targetColor);



	glGenVertexArrays(1, &VAO_axis);
	glGenBuffers(1, &VBO_axis);

	glBindVertexArray(VAO_axis);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_axis);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_axis), vertices_axis, GL_STATIC_DRAW);

	// position attribute
	int axisLocation = glGetAttribLocation(shader->ID, "aPosition");
	glVertexAttribPointer(axisLocation, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(axisLocation);

	// color attribute
	int axisColor = glGetAttribLocation(shader->ID, "aColor");
	glVertexAttribPointer(axisColor, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) (3 * sizeof(float)));
	glEnableVertexAttribArray(axisColor);
}
/*********************************************************************
** Function: Render
** Description: Displays the world axis and camera's target
** Parameters: none
** Pre-Conditions: Camera's VAO's must have been created
** Post-Conditions: The world axis and camera's target will be drawn
*********************************************************************/
void Camera::Render()
{
	if (settings["toggleTarget"] || settings["toggleAxis"])
	{
		shader->Use();

		shader->SetMat4("projection", GetProjectionMatrix());
		//shader->SetMat4("projection", GetOrthographicMatrix(720,720));
		shader->SetMat4("view", GetViewMatrix());

		glLineWidth(3);
	}

	if (settings["toggleAxis"])
	{
		glBindVertexArray(VAO_axis);

		shader->SetMat4("model", glm::mat4(1.0f));

		glDrawArrays(GL_LINES, 0, 6);
	}

	if (settings["toggleTarget"])
	{
		glBindVertexArray(VAO_target);

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, target);
		model = glm::rotate(model, (float)glfwGetTime(), unitRight);
		model = glm::rotate(model, (float)glfwGetTime() / 2, unitUp);
		model = glm::rotate(model, (float)glfwGetTime() / 4, unitFront);
		model = glm::scale(model, glm::vec3(targetDistance / 3));	//@TODO: when uncommented, the target doesn't immediately show up
		shader->SetMat4("model", model);

		glDrawArrays(GL_LINES, 0, 6);
	}
}

/*********************************************************************
** Function: RightAxis
** Description: Returns the horizontal axis of the camera's orientation
** Parameters: none
*********************************************************************/
glm::vec3 Camera::RightAxis()
{
	return this->right;
}
/*********************************************************************
** Function: UpAxis
** Description: Returns the vertical axis of the camera's orientation
** Parameters: none
*********************************************************************/
glm::vec3 Camera::UpAxis()
{
	return this->up;
}
/*********************************************************************
** Function: Direction
** Description: Returns the direction of the camera's orientation
** Parameters: none
*********************************************************************/
glm::vec3 Camera::Direction()
{
	return this->direction;
}

/*********************************************************************
** Function: GetSetting
** Description: Returns the value of the provided setting name
** Parameters: std::string
*********************************************************************/
float Camera::GetSetting(std::string key)
{
	return settings[key];
}
/*********************************************************************
** Function: SetSetting
** Description: Sets the value of the provided setting name
** Parameters: std::string, float
** Post-Conditions: Camera's orientation is fixed to be within the
**					new setting
*********************************************************************/
void Camera::SetSetting(std::string key, float value)
{
	settings[key] = value;

	Update(0,0);
}

/*********************************************************************
** Function: GetKey
** Description: Returns the key of the provided action name
** Parameters: std::string
*********************************************************************/
float Camera::GetKey(std::string key)
{
	return keyMap[key];
}
/*********************************************************************
** Function: SetKey
** Description: Sets the key of the provided action name
** Parameters: std::string, int
*********************************************************************/
void Camera::SetKey(std::string key, int button)
{
	keyMap[key] = button;
}

/*********************************************************************
** Function: GetPosition
** Description: Returns the camera's position
** Parameters: none
*********************************************************************/
glm::vec3 Camera::GetPosition()
{
	return this->position;
}
/*********************************************************************
** Function: GetPosition
** Description: Returns the camera's position
** Parameters: glm::vec3
*********************************************************************/
void Camera::SetPosition(glm::vec3 position)
{
	this->position = position;
}

/*********************************************************************
** Function: GetYaw
** Description: Returns the yaw of the camera's orientation
** Parameters: none
*********************************************************************/
float Camera::GetYaw()
{
	return glm::degrees(this->yaw);
}
/*********************************************************************
** Function: SetYaw
** Description: Sets the yaw of the camera's orientation, and rotates
**				the camera accordingly
** Parameters: float
** Post-Conditions: Camera's orientation is adjusted
*********************************************************************/
void Camera::SetYaw(float yaw)
{
	this->yaw = glm::radians(yaw);

	RotateVectors();
}
/*********************************************************************
** Function: AdjustYaw
** Description: Adds to the yaw of the camera's orientation, and
**				rotates the camera accordingly
** Parameters: float
** Post-Conditions: Camera's orientation is adjusted
*********************************************************************/
void Camera::AdjustYaw(float angle)
{
	this->yaw += glm::radians(angle);

	RotateVectors();
}

/*********************************************************************
** Function: GetPitch
** Description: Returns the pitch of the camera's orientation, and
**				rotates the camera accordingly
** Parameters: none
*********************************************************************/
float Camera::GetPitch()
{
	return glm::degrees(this->pitch);
}
/*********************************************************************
** Function: SetPitch
** Description: Sets the pitch of the camera's orientation, and
**				rotates the camera accordingly
** Parameters: float
** Post-Conditions: Camera's orientation is adjusted
*********************************************************************/
void Camera::SetPitch(float pitch)
{
	this->pitch = glm::radians(std::max(-89.0f, std::min(pitch, 89.0f)));

	RotateVectors();
}
/*********************************************************************
** Function: AdjustPitch
** Description: Adds to the pitch of the camera's orientation, and
**				rotates the camera accordingly
** Parameters: float
** Post-Conditions: Camera's orientation is adjusted
*********************************************************************/
void Camera::AdjustPitch(float angle)
{
	this->pitch += glm::radians(angle);
	this->pitch = glm::radians(std::max(-89.0f, std::min(glm::degrees(this->pitch), 89.0f)));

	RotateVectors();
}

/*********************************************************************
** Function: GetViewMatrix
** Description: Returns the matrix that transforms world cooridnates
**				to view space.
** Parameters: none
*********************************************************************/
glm::mat4 Camera::GetViewMatrix()
{
	return glm::lookAt(this->position, this->direction + this->position, this->up);
}

/*********************************************************************
** Function: GetProjectionMatrix
** Description: Returns the matrix that transforms view space to clip
**				space, defining the visible coordinates specified by 
**				a width, a height and a near and far plane.
** Parameters: float, float
*********************************************************************/
glm::mat4 Camera::GetOrthographicMatrix(float width, float height)
{
	return glm::ortho(0.0f, width, 0.0f, height, settings["nearClipping"], settings["farClipping"]);
}

/*********************************************************************
** Function: GetProjectionMatrix
** Description: Returns the matrix that transforms view space to clip
**				space.
** Parameters: none
*********************************************************************/
glm::mat4 Camera::GetProjectionMatrix()
{
	return glm::perspective(settings["fov"], settings["aspectRatio"], settings["nearClipping"], settings["farClipping"]);
}

bool Camera::JustPressed(std::map<int,int> keyState, std::string setting)
{
	return glfwGetKey(window, this->keyMap[setting]) == GLFW_PRESS && keyState[this->keyMap[setting]] == GLFW_RELEASE;
}

/*********************************************************************
** Function: KeyboardInput
** Description: Handles input from keyboard
** Parameters: GLFWwindow*, std::map<int,int>&, float
** Post-Conditions: Camera's settings and position may be changed
*********************************************************************/
void Camera::KeyboardInput(std::map<int, int>& keyState, float frameTime)
{
	/*
	 * SETTINGS
	 */
	// Camera mode
	if (JustPressed(keyState, "mode"))
	{
		if (settings["mode"] == FPS)
			SetSetting("mode", FREE);
		else
			SetSetting("mode", FPS);

		keyState[this->keyMap["mode"]] = GLFW_PRESS;
	}
	else if (glfwGetKey(window, this->keyMap["mode"]) == GLFW_RELEASE)
		keyState[this->keyMap["mode"]] = GLFW_RELEASE;

	// Camera lock
	if (JustPressed(keyState, "lock"))
	{
		SetSetting("lock", !settings["lock"]);

		keyState[this->keyMap["lock"]] = GLFW_PRESS;
	}
	else if (glfwGetKey(window, this->keyMap["lock"]) == GLFW_RELEASE)
		keyState[this->keyMap["lock"]] = GLFW_RELEASE;

	// Toggle target
	if (JustPressed(keyState, "toggleTarget"))
	{
		SetSetting("toggleTarget", !settings["toggleTarget"]);

		keyState[this->keyMap["toggleTarget"]] = GLFW_PRESS;
	}
	else if (glfwGetKey(window, this->keyMap["toggleTarget"]) == GLFW_RELEASE)
		keyState[this->keyMap["toggleTarget"]] = GLFW_RELEASE;

	// Toggle axis
	if (JustPressed(keyState, "toggleAxis"))
	{
		SetSetting("toggleAxis", !settings["toggleAxis"]);

		keyState[this->keyMap["toggleAxis"]] = GLFW_PRESS;
	}
	else if (glfwGetKey(window, this->keyMap["toggleAxis"]) == GLFW_RELEASE)
		keyState[this->keyMap["toggleAxis"]] = GLFW_RELEASE;

	//@TODO: error when holding down arrow keys and change camera orientation
	/*
	 * MOVEMENT
	 */
	if (glfwGetKey(window, this->keyMap["moveForward"]) == GLFW_PRESS)
		MoveAlongAxis(this->Direction(), frameTime);
	if (glfwGetKey(window, this->keyMap["moveBackward"]) == GLFW_PRESS)
		MoveAlongAxis(this->Direction(), -frameTime);
	if (glfwGetKey(window, this->keyMap["moveRight"]) == GLFW_PRESS)
		MoveAlongAxis(this->RightAxis(), frameTime);
	if (glfwGetKey(window, this->keyMap["moveLeft"]) == GLFW_PRESS)
		MoveAlongAxis(this->RightAxis(), -frameTime);
	if (glfwGetKey(window, this->keyMap["zoomIn"]) == GLFW_PRESS)
		Zoom(frameTime);
	if (glfwGetKey(window, this->keyMap["zoomOut"]) == GLFW_PRESS)
		Zoom(-frameTime);

	if (settings["mode"] == FPS)
	{
		if (glfwGetKey(window, this->keyMap["moveUp"]) == GLFW_PRESS)
			MoveAlongAxis(unitUp, frameTime);
		if (glfwGetKey(window, this->keyMap["moveDown"]) == GLFW_PRESS)
			MoveAlongAxis(unitUp, -frameTime);
	}
	else
	{
		if (glfwGetKey(window, this->keyMap["moveUp"]) == GLFW_PRESS)
			MoveAlongAxis(this->UpAxis(), frameTime);
		if (glfwGetKey(window, this->keyMap["moveDown"]) == GLFW_PRESS)
			MoveAlongAxis(this->UpAxis(), -frameTime);
	}
}
/*********************************************************************
** Function: MouseInput
** Description: Handles input from mouse
** Parameters: float, float, bool, bool
** Post-Conditions: Camera's orientation may be changed
*********************************************************************/
void Camera::MouseInput(float deltaX, float deltaY, bool leftClick, bool rightClick)
{
	deltaX *= settings["sensitivity"];
	deltaY *= settings["sensitivity"];

	if (rightClick)
		Update(deltaX, deltaY);
}
/*********************************************************************
** Function: ScrollInput
** Description: Handles input from mouse's scroll wheel
** Parameters: float
** Post-Conditions: Camera's orientation may be changed
*********************************************************************/
void Camera::ScrollInput(float offset)
{
	Zoom(offset * settings["zoomSpeed"]);
}

/*********************************************************************
** Function: Update
** Description: Updates the camera's orientation to match yaw and
**				pitch
** Parameters: float, float
*********************************************************************/
void Camera::Update(float yaw, float pitch)
{
	if (settings["mode"] == Z)
		RotateZ(yaw - pitch);
	else if (settings["lock"] == 1)
		switch ((int)settings["mode"])
		{
		case X:
			ArcBallYaw(yaw - pitch, true);
			break;
		case Y:
			ArcBallPitch(pitch - pitch, true);
			break;
		default:
			//ArcBallYaw(yaw, true);
			//ArcBallPitch(pitch, true);
			ArcBallYaw(yaw);
			ArcBallPitch(pitch);
		}
	else
		switch ((int)settings["mode"])
		{
		case X:
			RotateX(yaw - pitch);
			break;
		case Y:
			RotateY(pitch - pitch);
			break;
		default:
			RotateYaw(yaw, true);
			RotatePitch(pitch, true);
		}
}

/*********************************************************************
** Function: GetRadius
** Description: Returns the camera's distance from the origin
** Parameters: none
*********************************************************************/
float Camera::GetRadius()
{
	return glm::length(this->position);
}

/*********************************************************************
** Function: MoveAlongAxis
** Description: Moves the camera's position along provided axis by 1
**				unit.
** Parameters: glm::vec3, bool
*********************************************************************/
void Camera::MoveAlongAxis(glm::vec3 axis, bool updateTarget)
{
	MoveAlongAxis(axis, 1, updateTarget);
}
/*********************************************************************
** Function: MoveAlongAxis
** Description: Moves the camera's position along provided axis.
** Parameters: glm::vec3, float, bool
*********************************************************************/
void Camera::MoveAlongAxis(glm::vec3 axis, float distance, bool updateTarget)
{
	UpdateTargetDistance();
	position += axis * distance * settings["moveSpeed"];

	if (updateTarget)
		UpdateTarget();
}
/*********************************************************************
** Function: MoveAlongAxis
** Description: Returns the position when moving a point along the
**				provided axis.
** Parameters: glm::vec3, glm::vec3, float
*********************************************************************/
glm::vec3 Camera::MoveAlongAxis(glm::vec3 position, glm::vec3 axis, float distance)
{
	return position + (axis * distance);
}

/*********************************************************************
** Function: Zoom
** Description: Zooms camera's position
** Parameters: float
*********************************************************************/
void Camera::Zoom(float distance)
{
	MoveAlongAxis(direction, settings["zoomSpeed"] * distance * targetDistance, false);
}
/*********************************************************************
** Function: Zoom
** Description: Zooms camera's position along provided axis
** Parameters: glm::vec3, float
*********************************************************************/
void Camera::ZoomAlongAxis(glm::vec3 axis, float distance)
{
	MoveAlongAxis(axis, settings["zoomSpeed"] * distance * targetDistance, false);
}
/*********************************************************************
** Function: ZoomTarget
** Description: Zooms camera's position to target
** Parameters: glm::vec3, float, bool
*********************************************************************/
void Camera::ZoomTarget(glm::vec3 target, float distance, bool updateTarget)
{
	UpdateTargetDistance();
	MoveAlongAxis(glm::normalize(target - position), settings["zoomSpeed"] * distance * targetDistance, false);

	if (updateTarget)
		UpdateTarget();
}

/*********************************************************************
** Function: RotateX
** Description: Rotates the camera about the x-axis
** Parameters: float, bool
*********************************************************************/
void Camera::RotateX(float angle, bool updateTarget)
{
	UpdateTargetDistance();
	ArcBall(unitRight, angle);

	if (updateTarget)
		UpdateTarget();
}
/*********************************************************************
** Function: RotateY
** Description: Rotates the camera about the y-axis
** Parameters: float, bool
*********************************************************************/
void Camera::RotateY(float angle, bool updateTarget)
{
	UpdateTargetDistance();
	ArcBall(unitUp, angle);

	if (updateTarget)
		UpdateTarget();
}
/*********************************************************************
** Function: RotateZ
** Description: Rotates the camera about the z-axis
** Parameters: float, bool
*********************************************************************/
void Camera::RotateZ(float angle, bool updateTarget)
{
	UpdateTargetDistance();
	ArcBall(unitFront, angle);

	if (updateTarget)
		UpdateTarget();
}

/*********************************************************************
** Function: RotateYaw
** Description: Rotates the camera about the vertical axis
** Parameters: float, bool
*********************************************************************/
void Camera::RotateYaw(float angle, bool updateTarget)
{
	UpdateTargetDistance();

	if (settings["mode"] == FPS)
		AdjustYaw(angle);
	else
		RotateVectors(glm::angleAxis(glm::radians(-angle), glm::normalize(up)));

	if (updateTarget)
		UpdateTarget();
}
/*********************************************************************
** Function: RotatePitch
** Description: Rotates the camera about the horizontal axis
** Parameters: float, bool
*********************************************************************/
void Camera::RotatePitch(float angle, bool updateTarget)
{
	UpdateTargetDistance();

	if (settings["mode"] == FPS)
		AdjustPitch(angle);
	else
		RotateVectors(glm::angleAxis(glm::radians(angle), glm::normalize(right)));

	if (updateTarget)
		UpdateTarget();
}
/*********************************************************************
** Function: RotateRoll
** Description: Rotates the camera about the direction axis
** Parameters: float, bool
*********************************************************************/
void Camera::RotateRoll(float angle, bool updateTarget)
{
	UpdateTargetDistance();

	RotateVectors(glm::angleAxis(glm::radians(angle), glm::normalize(direction)));

	if (updateTarget)
		UpdateTarget();
}

/*********************************************************************
** Function: Rotate
** Description: Returns a point rotated about the provided axis
** Parameters: glm::vec3, glm::vec3, float
*********************************************************************/
glm::vec3 Camera::Rotate(glm::vec3 position, glm::vec3 axis, float angle) const
{
	/*Quaternion localRotation = Quaternion.FromAxisAngle(axis, MathHelper.DegreesToRadians(angle));
	return localRotation.Inverted() * (localRotation * position);*/
	
	glm::quat localRotation = glm::angleAxis(glm::radians(angle), glm::normalize(axis));

	glm::vec3 v = glm::inverse(localRotation) * (localRotation * position);

	position.x = (float) v.x;
	position.y = (float) v.y;
	position.z = (float) v.z;

	return position;
}

/*********************************************************************
** Function: ArcBall
** Description: Rotates the camera's position about its target by the
**				provided axis
** Parameters: glm::vec3, float, bool
*********************************************************************/
void Camera::ArcBall(glm::vec3 axis, float angle, bool aroundOrigin)
{
	RotateVectors(glm::angleAxis(glm::radians(angle), glm::normalize(axis)));
	
	position = (unitFront * -GetRadius()) + (aroundOrigin ? glm::vec3(0, 0, 0) : target);

	if (aroundOrigin)
		UpdateTarget();
}
/*********************************************************************
** Function: ArcBallYaw
** Description: Yaws the camera's position about its target
** Parameters: float, bool
*********************************************************************/
void Camera::ArcBallYaw(float angle, bool aroundOrigin)
{
	position -= (aroundOrigin ? glm::vec3(0,0,0) : target);

	if (settings["mode"] == FPS)
		AdjustYaw(angle);
	else
		RotateVectors(glm::angleAxis(glm::radians(-angle), glm::normalize(up)));
	
	position = (direction * -GetRadius()) + (aroundOrigin ? glm::vec3(0, 0, 0) : target);

	if (aroundOrigin)
		UpdateTarget();
}
/*********************************************************************
** Function: ArcBallPitch
** Description: Pitches the camera's position about its target
** Parameters: float, bool
*********************************************************************/
void Camera::ArcBallPitch(float angle, bool aroundOrigin)
{
	position -= (aroundOrigin ? glm::vec3(0, 0, 0) : target);

	if (settings["mode"] == FPS)
		AdjustPitch(angle);
	else
		RotateVectors(glm::angleAxis(glm::radians(angle), glm::normalize(right)));
	
	position = (direction * -GetRadius()) + (aroundOrigin ? glm::vec3(0, 0, 0) : target);

	if (aroundOrigin)
		UpdateTarget();
}

/*********************************************************************
** Function: RotateVectors
** Description: Performs the camera's rotation using Euler angles
** Parameters: none
*********************************************************************/
void Camera::RotateVectors()
{
	// First the direction matrix is calculated using some basic trigonometry
	direction.x = (float) cos(pitch) * (float) cos(yaw);
	direction.y = (float) sin(pitch);
	direction.z = (float) cos(pitch) * (float) sin(yaw);

	// We need to make sure the vectors are all normalized, as otherwise we would get some funky results
	direction = glm::normalize(direction);

	// Calculate both the right and the up vector using cross product
	//		Note that we are calculating the right from the global up, this behaviour might
	//		not be what you need for all cameras so keep this in mind if you do not want a FPS camera
	right = glm::normalize(glm::cross(direction, glm::vec3(0,1,0)));
	up = glm::normalize(glm::cross(right, direction));
}
/*********************************************************************
** Function: RotateVectors
** Description: Performs the camera's rotation using quaternions
** Parameters: glm::quat
*********************************************************************/
void Camera::RotateVectors(glm::quat localRotation)
{
	
	ApplyRotationToVector(localRotation, right);
	ApplyRotationToVector(localRotation, up);
	ApplyRotationToVector(localRotation, direction);
	/*
	glm::vec3 hAxis = right;
	glm::vec3 vAxis = up;
	glm::vec3 dir = direction;

	ApplyRotationToVector(localRotation, hAxis);
	ApplyRotationToVector(localRotation, vAxis);
	ApplyRotationToVector(localRotation, dir);
	/*
	right.x = (float) hAxis.x;
	right.y = (float) hAxis.y;
	right.z = (float) hAxis.z;

	up.x = (float) vAxis.x;
	up.y = (float) vAxis.y;
	up.z = (float) vAxis.z;

	direction.x = (float) dir.x;
	direction.y = (float) dir.y;
	direction.z = (float) dir.z;
	*\

	right = hAxis;
	up = vAxis;
	direction = dir;
	*/
	yaw = (float) atan2(direction.x, -direction.z) - glm::radians(90.0f);
	pitch = (float) asin(direction.y);
}

/*********************************************************************
** Function: ApplyRotationToVector
** Description: Returns a vector rotated by a quaternion
** Parameters: glm::quat, glm::vec3
*********************************************************************/
void Camera::ApplyRotationToVector(glm::quat rotation, glm::vec3& v)
{
	//v = glm::inverse(rotation) * (rotation * v); // GLM overrides this
	v = rotation * v;
}

/*********************************************************************
** Function: UpdateTargetDistance
** Description: Saves the distance between the camera's position and
**				its target
** Parameters: none
*********************************************************************/
void Camera::UpdateTargetDistance()
{
	this->targetDistance = glm::length(this->target - this->position);
}

/*********************************************************************
** Function: UpdateTarget
** Description: Updates the target's position based on targetDistance
** Parameters: none
*********************************************************************/
void Camera::UpdateTarget()
{
	this->target = MoveAlongAxis(this->position, this->direction, this->targetDistance);
}
