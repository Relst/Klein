// ============= Renderer.cpp =============
#include "Renderer.h"
#include "Logger.h"
#include "Shader.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Klein {

    Renderer::Renderer() {}

    Renderer::~Renderer() {
        Shutdown();
    }

    void Renderer::Init() {
        // Create default shaders
        ShaderLibrary::Get().CreateDefaultShaders();

        // Create default mesh
        m_defaultCubeMesh = Mesh::CreateCube();

        // Create default material
        m_defaultMaterial = std::make_shared<Material>();

        KleinLogger::Logger::EngineLog("Renderer initialized");
    }

    void Renderer::Shutdown() {
        m_defaultCubeMesh.reset();
        m_defaultMaterial.reset();
        KleinLogger::Logger::EngineLog("Renderer shutdown");
    }

    void Renderer::Clear(const glm::vec4& color) {
        glClearColor(color.r, color.g, color.b, color.a);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void Renderer::RenderScene(Scene* scene, float aspectRatio) {
        if (!scene) return;

        ResetStats();

        // Get primary camera
        Entity cameraEntity = scene->GetPrimaryCamera();
        if (!cameraEntity) {
            KleinLogger::Logger::EngineWarn("No primary camera found in scene");
            return;
        }

        auto& camera = cameraEntity.GetComponent<CameraComponent>();
        auto& camTransform = cameraEntity.GetComponent<TransformComponent>();

        // Calculate view and projection matrices
        glm::mat4 view = glm::lookAt(
            camTransform.position,
            camTransform.position + camTransform.GetForward(),
            camTransform.GetUp()
        );
        glm::mat4 projection = camera.GetProjection(aspectRatio);
        glm::mat4 viewProj = projection * view;

        // Get shader
        auto shader = ShaderLibrary::Get().Get("default");
        shader->Bind();

        // Setup lighting
        SetupLighting(scene, shader, camTransform.position);

        // Render all entities with MeshRenderer
        auto renderables = scene->GetEntitiesWithComponent<MeshRendererComponent>();
        for (auto& entity : renderables) {
            RenderEntity(entity, viewProj, camTransform.position);
        }

        shader->Unbind();
    }

    void Renderer::RenderEntity(Entity entity, const glm::mat4& viewProj, const glm::vec3& cameraPos) {
        if (!entity.HasComponent<TransformComponent>() ||
            !entity.HasComponent<MeshRendererComponent>()) {
            return;
        }

        auto& transform = entity.GetComponent<TransformComponent>();
        auto& meshRenderer = entity.GetComponent<MeshRendererComponent>();

        if (!meshRenderer.mesh) {
            meshRenderer.mesh = m_defaultCubeMesh;
        }
        if (!meshRenderer.material) {
            meshRenderer.material = m_defaultMaterial;
        }

        auto shader = ShaderLibrary::Get().Get(meshRenderer.material->shaderName);
        if (!shader) {
            shader = ShaderLibrary::Get().Get("default");
        }

        shader->Bind();

        // Set transform matrices
        glm::mat4 model = transform.GetTransform();
        shader->SetMat4("u_Model", model);
        shader->SetMat4("u_ViewProjection", viewProj);
        shader->SetVec3("u_CameraPos", cameraPos);

        // Set material properties
        shader->SetVec3("u_Material.albedo", meshRenderer.material->albedo);
        shader->SetFloat("u_Material.metallic", meshRenderer.material->metallic);
        shader->SetFloat("u_Material.roughness", meshRenderer.material->roughness);
        shader->SetFloat("u_Material.ao", meshRenderer.material->ao);

        // Bind textures if available
        if (meshRenderer.material->albedoMap) {
            meshRenderer.material->albedoMap->Bind(0);
            shader->SetInt("u_AlbedoMap", 0);
            shader->SetInt("u_UseAlbedoMap", 1);
        } else {
            shader->SetInt("u_UseAlbedoMap", 0);
        }

        // Draw mesh
        meshRenderer.mesh->Draw();

        m_stats.drawCalls++;
        m_stats.triangles += meshRenderer.mesh->indices.size() / 3;
        m_stats.vertices += meshRenderer.mesh->vertices.size();
    }

    void Renderer::SetupLighting(Scene* scene, std::shared_ptr<Shader> shader, const glm::vec3& cameraPos) {
        auto lights = scene->GetLights();

        int dirLightCount = 0;
        int pointLightCount = 0;

        for (auto& lightEntity : lights) {
            auto& light = lightEntity.GetComponent<LightComponent>();
            auto& transform = lightEntity.GetComponent<TransformComponent>();

            if (light.type == LightComponent::Type::Directional && dirLightCount < 4) {
                std::string base = "u_DirLights[" + std::to_string(dirLightCount) + "]";
                shader->SetVec3(base + ".direction", transform.GetForward());
                shader->SetVec3(base + ".color", light.color);
                shader->SetFloat(base + ".intensity", light.intensity);
                dirLightCount++;
            }
            else if (light.type == LightComponent::Type::Point && pointLightCount < 8) {
                std::string base = "u_PointLights[" + std::to_string(pointLightCount) + "]";
                shader->SetVec3(base + ".position", transform.position);
                shader->SetVec3(base + ".color", light.color);
                shader->SetFloat(base + ".intensity", light.intensity);
                shader->SetFloat(base + ".range", light.range);
                pointLightCount++;
            }
        }

        shader->SetInt("u_DirLightCount", dirLightCount);
        shader->SetInt("u_PointLightCount", pointLightCount);
    }

    void Renderer::ResetStats() {
        m_stats = RenderStats();
    }

    void Renderer::SetWireframe(bool enabled) {
        m_wireframe = enabled;
        glPolygonMode(GL_FRONT_AND_BACK, enabled ? GL_LINE : GL_FILL);
    }

} // namespace Klein