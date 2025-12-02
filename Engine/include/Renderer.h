
// ============= Renderer.h =============
#ifndef RENDERER_H
#define RENDERER_H

#include <memory>
#include <vector>
#include "Scene.h"
#include "Components.h"
#include "Mesh.h"
#include "Shader.h"
#include <glm/glm.hpp>

namespace Klein {

    struct RenderStats {
        uint32_t drawCalls = 0;
        uint32_t triangles = 0;
        uint32_t vertices = 0;
        float frameTime = 0.0f;
    };

    class Renderer {
    public:
        Renderer();
        ~Renderer();

        void Init();
        void Shutdown();

        // Main render function
        void RenderScene(Scene* scene, float aspectRatio);

        // Render individual entity
        void RenderEntity(Entity entity, const glm::mat4& viewProj, const glm::vec3& cameraPos);

        // Clear screen
        void Clear(const glm::vec4& color = glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
        
        // Stats
        const RenderStats& GetStats() const { return m_stats; }
        void ResetStats();

        // Settings
        void SetWireframe(bool enabled);
        bool IsWireframe() const { return m_wireframe; }

    private:
        void SetupLighting(Scene* scene, std::shared_ptr<Shader> shader, const glm::vec3& cameraPos);

        RenderStats m_stats;
        bool m_wireframe = false;
        
        // Default resources
        std::shared_ptr<Mesh> m_defaultCubeMesh;
        std::shared_ptr<Material> m_defaultMaterial;
    };

} // namespace Klein

#endif // RENDERER_H
