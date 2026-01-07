#include "OrthographicCamera.h"
#include <glm/gtc/matrix_transform.hpp>

namespace Engine {

OrthographicCamera::OrthographicCamera(float aspectRatio) : m_AspectRatio(aspectRatio) {
    SetProjection(aspectRatio, m_ZoomLevel);
}

void OrthographicCamera::SetPosition(const glm::vec3& position) {
    m_Position = position;
    RecalculateViewMatrix();
}

void OrthographicCamera::SetRotation(float rotation) {
    m_Rotation = rotation;
    RecalculateViewMatrix();
}

void OrthographicCamera::SetProjection(float aspectRatio, float zoomLevel) {
    m_AspectRatio = aspectRatio;
    m_ZoomLevel = zoomLevel;
    float left = -m_AspectRatio * m_ZoomLevel;
    float right = m_AspectRatio * m_ZoomLevel;
    float bottom = -m_ZoomLevel;
    float top = m_ZoomLevel;
    m_ProjectionMatrix = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
    m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
}

void OrthographicCamera::RecalculateViewMatrix() {
    glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position) *
                          glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation), glm::vec3(0, 0, 1));

    m_ViewMatrix = glm::inverse(transform);
    m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
}

}