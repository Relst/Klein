#include "Klein.h"
#include "App.h"
#include "Components.h"
#include "Mesh.h"
#include <glm/gtc/quaternion.hpp>
#include <GLFW/glfw3.h>

class MinecraftGame : public Klein::App {
public:
    void OnStart() override {
        KleinLogger::Logger::Log("Minecraft-like game starting...");

        auto scene = GetActiveScene();

        // Create a camera
        auto camera = scene->CreateEntity("Main Camera");
        camera.GetComponent<Klein::TransformComponent>().position = glm::vec3(0.0f, 2.0f, 5.0f);
        auto& camComp = camera.AddComponent<Klein::CameraComponent>();
        camComp.fov = 60.0f;
        camComp.primary = true;

        // Create a ground plane
        auto ground = scene->CreateEntity("Ground");
        ground.GetComponent<Klein::TransformComponent>().scale = glm::vec3(20.0f, 1.0f, 20.0f);
        auto groundMaterial = std::make_shared<Klein::Material>();
        groundMaterial->albedo = glm::vec3(0.3f, 0.6f, 0.3f); // Green grass-like
        ground.AddComponent<Klein::MeshRendererComponent>(
            Klein::Mesh::CreatePlane(1.0f, 1.0f),
            groundMaterial
        );

        // Create some blocks (cubes)
        for (int x = -5; x <= 5; x += 2) {
            for (int z = -5; z <= 5; z += 2) {
                auto block = scene->CreateEntity("Block_" + std::to_string(x) + "_" + std::to_string(z));
                auto& transform = block.GetComponent<Klein::TransformComponent>();
                transform.position = glm::vec3(x, 0.5f, z);

                auto material = std::make_shared<Klein::Material>();

                material->albedo = glm::vec3(
                    0.5f + (x + 5) * 0.05f,
                    0.3f + (z + 5) * 0.05f,
                    0.7f
                );
                material->roughness = 0.8f;

                block.AddComponent<Klein::MeshRendererComponent>(
                    Klein::Mesh::CreateCube(),
                    material
                );
            }
        }

        // Directional light (sun)
        auto sun = scene->CreateEntity("Sun");
        auto& sunTransform = sun.GetComponent<Klein::TransformComponent>();
        sunTransform.position = glm::vec3(0.0f, 10.0f, 0.0f);
        sunTransform.rotation = glm::quat(glm::vec3(glm::radians(-45.0f), glm::radians(-30.0f), 0.0f));

        auto& light = sun.AddComponent<Klein::LightComponent>();
        light.type = Klein::LightComponent::Type::Directional;
        light.color = glm::vec3(1.0f, 0.95f, 0.8f); // Warm sunlight
        light.intensity = 1.0f;

        KleinLogger::Logger::Log("Scene setup complete!");
        KleinLogger::Logger::Log("Controls: WASD to move camera (to be implemented)");
    }

    void OnUpdate(float deltaTime) override {
        // Simple camera rotation with mouse
        auto window = GetWindow();
        auto scene = GetActiveScene();

        if (!scene) return;

        auto camera = scene->GetPrimaryCamera();
        if (!camera) return;

        auto& transform = camera.GetComponent<Klein::TransformComponent>();

        // Simple orbit camera with arrow keys
        float rotSpeed = 1.0f * deltaTime;
        float moveSpeed = 5.0f * deltaTime;

        if (window->IsKeyPressed(GLFW_KEY_LEFT)) {
            float angle = rotSpeed;
            transform.position = glm::vec3(
                transform.position.x * cos(angle) - transform.position.z * sin(angle),
                transform.position.y,
                transform.position.x * sin(angle) + transform.position.z * cos(angle)
            );
        }
        if (window->IsKeyPressed(GLFW_KEY_RIGHT)) {
            float angle = -rotSpeed;
            transform.position = glm::vec3(
                transform.position.x * cos(angle) - transform.position.z * sin(angle),
                transform.position.y,
                transform.position.x * sin(angle) + transform.position.z * cos(angle)
            );
        }
        if (window->IsKeyPressed(GLFW_KEY_UP)) {
            transform.position.y += moveSpeed;
        }
        if (window->IsKeyPressed(GLFW_KEY_DOWN)) {
            transform.position.y -= moveSpeed;
        }


        if (window->IsKeyPressed(GLFW_KEY_ESCAPE)) {
            window->Close();
        }



        auto blocks = scene->GetEntitiesWithComponent<Klein::MeshRendererComponent>();
        for (auto& block : blocks) {
            if (block.GetComponent<Klein::TagComponent>().tag.find("Block_") != std::string::npos) {
                auto& t = block.GetComponent<Klein::TransformComponent>();
                t.rotation = glm::rotate(t.rotation, deltaTime * 0.5f, glm::vec3(0.0f, 1.0f, 0.0f));
            }
        }
    }

    void OnUI() override {

    }
};

Klein::App* Klein::CreateApp() {
    Klein::appWindowName = "Klein Engine - Minecraft Clone";
    Klein::appWindowX = 1600;
    Klein::appWindowY = 900;
    return new MinecraftGame();
}
