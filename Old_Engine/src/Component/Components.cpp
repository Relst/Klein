#include "Components.h"
#include <glm/gtc/matrix_transform.hpp>

namespace Klein {

    glm::mat4 TransformComponent::GetTransform() const {
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position);
        transform *= glm::mat4_cast(rotation);
        transform = glm::scale(transform, scale);
        return transform;
    }

    glm::vec3 TransformComponent::GetForward() const {
        return glm::normalize(rotation * glm::vec3(0.0f, 0.0f, -1.0f));
    }

    glm::vec3 TransformComponent::GetRight() const {
        return glm::normalize(rotation * glm::vec3(1.0f, 0.0f, 0.0f));
    }

    glm::vec3 TransformComponent::GetUp() const {
        return glm::normalize(rotation * glm::vec3(0.0f, 1.0f, 0.0f));
    }

    glm::mat4 CameraComponent::GetProjection(float aspectRatio) const {
        if (projectionType == ProjectionType::Perspective) {
            return glm::perspective(glm::radians(fov), aspectRatio, nearClip, farClip);
        } else {
            float orthoLeft = -orthoSize * aspectRatio * 0.5f;
            float orthoRight = orthoSize * aspectRatio * 0.5f;
            float orthoBottom = -orthoSize * 0.5f;
            float orthoTop = orthoSize * 0.5f;
            return glm::ortho(orthoLeft, orthoRight, orthoBottom, orthoTop, nearClip, farClip);
        }
    }

} // namespace Klein