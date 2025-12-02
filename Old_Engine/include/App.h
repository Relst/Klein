// ============= App.h =============
#ifndef APP_H
#define APP_H

#include <memory>
#include "Scene.h"
#include "Window.h"
#include "Renderer.h"

namespace Klein {
    // Global app configuration
    inline const char* appWindowName = nullptr;
    inline int appWindowX = 1280;
    inline int appWindowY = 640;
    inline const char* appDefaultName = "Klein Application";

    class App {
    public:
        App();
        virtual ~App();

        void Run();

        // User-defined hooks
        virtual void OnStart() {}
        virtual void OnUpdate(float deltaTime) {}
        virtual void OnUI() {}
        virtual void OnShutdown() {}

        // Scene management
        std::shared_ptr<Scene> GetActiveScene() { return m_activeScene; }
        void SetScene(std::shared_ptr<Scene> scene);

        // Access to subsystems
        Window* GetWindow() { return m_window.get(); }
        Renderer* GetRenderer() { return m_renderer.get(); }

        static float GetDeltaTime() { return s_deltaTime; }

    private:
        void Init();
        void Shutdown();

        std::unique_ptr<Window> m_window;
        std::unique_ptr<Renderer> m_renderer;
        std::shared_ptr<Scene> m_activeScene;

        bool m_running = true;
        static inline float s_deltaTime = 0.0f;
        static inline float s_lastFrame = 0.0f;
    };

    // Implemented by client
    App* CreateApp();
}

#endif // APP_H
