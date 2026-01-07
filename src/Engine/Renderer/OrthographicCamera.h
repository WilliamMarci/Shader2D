#pragma once
#include "Engine/Renderer/Camera.h"

namespace Engine {

class OrthographicCamera : public Camera {
public:
    OrthographicCamera(float aspectRatio);

    void SetPosition(const glm::vec3& position);
    const glm::vec3& GetPosition() const { return m_Position; }

    void SetRotation(float rotation);
    float GetRotation() const { return m_Rotation; }

    void SetProjection(float aspectRatio, float zoomLevel);
    
    void SetZoomLevel(float level) { m_ZoomLevel = level; RecalculateViewMatrix(); }
    float GetZoomLevel() const { return m_ZoomLevel; }

private:
    void RecalculateViewMatrix();

private:
    glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };
    float m_Rotation = 0.0f;
    
    float m_AspectRatio;
    float m_ZoomLevel = 1.0f;
};

}
