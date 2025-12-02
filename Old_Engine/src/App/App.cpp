
// ============= App.cpp =============
#include "App.h"
#include "Logger.h"
#include "AssetManager.h"
#include "Shader.h"
#include <GLFW/glfw3.h>

namespace Klein {

    App::App() {
        Init();
    }

    App::~App() {
        Shutdown();
    }

    void App::Init() {
        // Create window
        WindowProps props(
            appWindowName ? appWindowName : appDefaultName,
            appWindowX,
            appWindowY
        );
        m_window = std::make_unique<Window>(props);

        // Create renderer
        m_renderer = std::make_unique<Renderer>();
        m_renderer->Init();

        // Setup default scene
        m_activeScene = std::make_shared<Scene>("Default Scene");

        // Setup ImGui if needed
        // ImGuiInit(m_window->GetNativeWindow());

        KleinLogger::Logger::EngineLog("Application initialized successfully");
    }

    void App::Shutdown() {
        OnShutdown();
        
        m_activeScene.reset();
        m_renderer->Shutdown();
        m_renderer.reset();
        m_window.reset();

        KleinLogger::Logger::EngineLog("Application shutdown complete");
    }

    void App::Run() {
        OnStart();

        while (m_running && !m_window->ShouldClose()) {
            float currentFrame = (float)glfwGetTime();
            s_deltaTime = currentFrame - s_lastFrame;
            s_lastFrame = currentFrame;

            // Poll input
            m_window->PollEvents();

            // User update
            OnUpdate(s_deltaTime);

            // Update scene (physics, scripts, etc.)
            if (m_activeScene) {
                m_activeScene->OnUpdate(s_deltaTime);
            }

            // Render
            m_renderer->Clear();
            if (m_activeScene) {
                m_renderer->RenderScene(m_activeScene.get(), m_window->GetAspectRatio());
            }

            // UI pass
            OnUI();

            // Swap buffers
            m_window->SwapBuffers();
        }
    }

    void App::SetScene(std::shared_ptr<Scene> scene) {
        m_activeScene = scene;
        KleinLogger::Logger::EngineLog("Active scene changed to: %s", 
            scene ? scene->GetName().c_str() : "None");
    }

} // namespace Klein

