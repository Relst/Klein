#ifndef COMPONENTS_H
#define COMPONENTS_H

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <string>
#include <vector>
#include <memory>

namespace Klein {

    // Forward declarations
    class Mesh;
    class Material;

    // Tag Component - Every entity has this
    struct TagComponent {
        std::string tag;
        TagComponent() = default;
        TagComponent(const std::string& tag) : tag(tag) {}
    };

    // Transform Component - Position, Rotation, Scale
    struct TransformComponent {
        glm::vec3 position{0.0f, 0.0f, 0.0f};
        glm::quat rotation{1.0f, 0.0f, 0.0f, 0.0f}; // Identity quaternion
        glm::vec3 scale{1.0f, 1.0f, 1.0f};

        TransformComponent() = default;
        TransformComponent(const glm::vec3& pos) : position(pos) {}

        glm::mat4 GetTransform() const;
        glm::vec3 GetForward() const;
        glm::vec3 GetRight() const;
        glm::vec3 GetUp() const;
    };

    // Camera Component
    struct CameraComponent {
        enum class ProjectionType { Perspective, Orthographic };

        ProjectionType projectionType = ProjectionType::Perspective;
        
        // Perspective
        float fov = 45.0f;
        float nearClip = 0.1f;
        float farClip = 1000.0f;
        
        // Orthographic
        float orthoSize = 10.0f;
        
        bool primary = true; // Is this the main camera?
        bool fixedAspectRatio = false;

        CameraComponent() = default;

        glm::mat4 GetProjection(float aspectRatio) const;
    };

    // Mesh Renderer Component
    struct MeshRendererComponent {
        std::shared_ptr<Mesh> mesh;
        std::shared_ptr<Material> material;
        bool castShadows = true;
        bool receiveShadows = true;

        MeshRendererComponent() = default;
        MeshRendererComponent(std::shared_ptr<Mesh> m, std::shared_ptr<Material> mat)
            : mesh(m), material(mat) {}
    };

    // Light Component
    struct LightComponent {
        enum class Type { Directional, Point, Spot };
        
        Type type = Type::Directional;
        glm::vec3 color{1.0f, 1.0f, 1.0f};
        float intensity = 1.0f;
        
        // Point & Spot
        float range = 10.0f;
        float constant = 1.0f;
        float linear = 0.09f;
        float quadratic = 0.032f;
        
        // Spot
        float innerCutoff = 12.5f; // degrees
        float outerCutoff = 17.5f; // degrees

        LightComponent() = default;
    };

    // Rigidbody Component (for physics - placeholder for now)
    struct RigidbodyComponent {
        enum class BodyType { Static, Dynamic, Kinematic };
        
        BodyType type = BodyType::Dynamic;
        float mass = 1.0f;
        float drag = 0.0f;
        float angularDrag = 0.05f;
        bool useGravity = true;
        bool isKinematic = false;
        
        glm::vec3 velocity{0.0f};
        glm::vec3 angularVelocity{0.0f};

        RigidbodyComponent() = default;
    };

    // Box Collider Component
    struct BoxColliderComponent {
        glm::vec3 size{1.0f, 1.0f, 1.0f};
        glm::vec3 offset{0.0f, 0.0f, 0.0f};
        bool isTrigger = false;

        BoxColliderComponent() = default;
    };

    // Script Component (for custom behavior)
    struct ScriptComponent {
        // Will be used for attaching custom C++ behavior classes
        // For now, just a placeholder
        void* instance = nullptr;
        
        // Function pointers for script lifecycle
        void (*onCreateFunc)(void*) = nullptr;
        void (*onUpdateFunc)(void*, float) = nullptr;
        void (*onDestroyFunc)(void*) = nullptr;

        ScriptComponent() = default;
    };

} // namespace Klein

#endif // COMPONENTS_H