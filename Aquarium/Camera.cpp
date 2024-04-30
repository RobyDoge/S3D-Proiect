#include "Camera.h"
#include "Acvariu.h"
#include <glew.h>

Camera::Camera(const int width, const int height, const glm::vec3& position)
{
    m_startPosition = position;
    Set(width, height, position);
}

void Camera::Set(const int width, const int height, const glm::vec3& position)
{
    m_isPerspective = true;
    m_yaw = YAW;
    m_pitch = PITCH;

    m_yFov = FOV;
    this->m_width = width;
    this->m_height = height;
    m_zNear = Z_NEAR;
    m_zFar = Z_FAR;

    m_worldUp = glm::vec3(0, 1, 0);
    this->m_position = position;

    m_lastX = width / 2.0f;
    m_lastY = height / 2.0f;
    m_firstMouseMove = true;

    UpdateCameraVectors();
}

void Camera::Reset(const int width, const int height)
{
    Set(width, height, m_startPosition);
}

void Camera::Reshape(const int windowWidth, const int windowHeight)
{
    m_width = windowWidth;
    m_height = windowHeight;

    // define the viewport transformation
    glViewport(0, 0, windowWidth, windowHeight);
}

glm::vec3 Camera::GetPosition() const
{
    return m_position;
}

glm::mat4 Camera::GetViewMatrix() const
{
    // Returns the View Matrix
    return glm::lookAt(m_position, m_position + m_forward, m_up);
}

glm::mat4 Camera::GetProjectionMatrix() const
{
	auto proj = glm::mat4(1);
    if (m_isPerspective) {
	    const float aspectRatio = static_cast<float>(m_width) / m_height;
        proj = glm::perspective(glm::radians(m_yFov), aspectRatio, m_zNear, m_zFar);
    }
    else {
	    constexpr float scaleFactor = 2000.f;
        proj = glm::ortho<float>(
            -m_width / scaleFactor, m_width / scaleFactor,
            -m_height / scaleFactor, m_height / scaleFactor, -m_zFar, m_zFar);
    }
    return proj;
}

void Camera::ProcessKeyboard(const MovementType direction, const float deltaTime)
{
	const float velocity = CAMERA_SPEED_FACTOR * deltaTime;
    switch (direction) {
    case Forward:
        m_position += m_forward * velocity;
        break;
    case Backward:
        m_position -= m_forward * velocity;
        break;
    case Left:
        m_position -= m_right * velocity;
        break;
    case Right:
        m_position += m_right * velocity;
        break;
    case Up:
        m_position += m_up * velocity;
        break;
    case Down:
        m_position -= m_up * velocity;
        break;
    }
}

void Camera::MouseControl(const float xPos, const float yPos)
{
    if (m_firstMouseMove) {
        m_lastX = xPos;
        m_lastY = yPos;
        m_firstMouseMove = false;
    }

    float xChange = xPos - m_lastX;
    float yChange = m_lastY - yPos;
    m_lastX = xPos;
    m_lastY = yPos;

    if (fabs(xChange) <= 1e-6 && fabs(yChange) <= 1e-6) {
        return;
    }
    xChange *= MOUSE_SENSITIVITY;
    yChange *= MOUSE_SENSITIVITY;

    ProcessMouseMovement(xChange, yChange);
}

void Camera::ProcessMouseScroll(const float yOffset)
{
    if (m_yFov >= 1.0f && m_yFov <= 90.0f) {
        m_yFov -= yOffset;
    }
    if (m_yFov <= 1.0f)
        m_yFov = 1.0f;
    if (m_yFov >= 90.0f)
        m_yFov = 90.0f;
}

void Camera::ProcessMouseMovement(float xOffset, const float yOffset, const bool constrainPitch)
{
    m_yaw += xOffset;
    m_pitch += yOffset;

    if (constrainPitch) {
        if (m_pitch > 89.0f)
            m_pitch = 89.0f;
        if (m_pitch < -89.0f)
            m_pitch = -89.0f;
    }
    UpdateCameraVectors();
}

void Camera::UpdateCameraVectors()
{
    m_forward.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    m_forward.y = sin(glm::radians(m_pitch));
    m_forward.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    m_forward = glm::normalize(m_forward);

    m_right = glm::normalize(glm::cross(m_forward, m_worldUp));
    m_up = glm::normalize(glm::cross(m_right, m_forward));
}