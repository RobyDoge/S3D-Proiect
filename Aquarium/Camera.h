#pragma once

#define GLM_FORCE_CTOR_INIT
#include <GLM.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>


class Camera
{
public:
	enum MovementType
	{
		Forward,
		Backward,
		Left,
		Right,
		Up,
		Down
	};

public:
	Camera(int width, int height, const glm::vec3& position);
	void Set(int width, int height, const glm::vec3& position);
	void Reset(int width, int height);
	void Reshape(int windowWidth, int windowHeight);
	glm::vec3 GetPosition() const;
	glm::mat4 GetViewMatrix() const;
	glm::mat4 GetProjectionMatrix() const;
	void ProcessKeyboard(MovementType direction, float deltaTime);
	void MouseControl(float xPos, float yPos, int isMoving);
	void ProcessMouseScroll(float yOffset);

private:
	void ProcessMouseMovement(float xOffset, float yOffset, bool constrainPitch = true);
	void UpdateCameraVectors();

private:
	static constexpr float Z_NEAR = 0.1f;
	static constexpr float Z_FAR = 500.f;
	static constexpr float YAW = -90.0f;
	static constexpr float PITCH = 0.0f;
	static constexpr float FOV = 45.0f;
	static constexpr float CAMERA_SPEED_FACTOR = 2.5f;
	static constexpr float MOUSE_SENSITIVITY = 0.1f;

private:
	bool m_isPerspective;
	float m_zNear;
	float m_zFar;
	float m_yFov;
	int m_width;
	int m_height;

	glm::vec3 m_position;
	glm::vec3 m_forward;
	glm::vec3 m_right;
	glm::vec3 m_up;
	glm::vec3 m_worldUp;
	glm::vec3 m_startPosition;

	float m_yaw;
	float m_pitch;

	bool m_firstMouseMove;
	float m_lastX;
	float m_lastY;
	int m_isMoving;
};
