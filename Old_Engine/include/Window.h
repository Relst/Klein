#ifndef WINDOW_H
#define WINDOW_H

#include <string>
#include <functional>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Klein {

    struct WindowProps {
        std::string title;
        int width;
        int height;
        bool vsync;

        WindowProps(const std::string& title = "Klein Engine",
                   int width = 1280,
                   int height = 720,
                   bool vsync = true)
            : title(title), width(width), height(height), vsync(vsync) {}
    };

    class Window {
    public:
        using ResizeCallback = std::function<void(int, int)>;
        using KeyCallback = std::function<void(int, int, int, int)>;
        using MouseButtonCallback = std::function<void(int, int, int)>;
        using CursorPosCallback = std::function<void(double, double)>;
        using ScrollCallback = std::function<void(double, double)>;

        Window(const WindowProps& props);
        ~Window();

        void PollEvents();
        void SwapBuffers();
        bool ShouldClose() const;
        void Close();

        int GetWidth() const { return m_width; }
        int GetHeight() const { return m_height; }
        float GetAspectRatio() const { return (float)m_width / (float)m_height; }
        
        GLFWwindow* GetNativeWindow() const { return m_window; }

        void SetVSync(bool enabled);
        bool IsVSync() const { return m_vsync; }

        // Event callbacks
        void SetResizeCallback(ResizeCallback callback) { m_resizeCallback = callback; }
        void SetKeyCallback(KeyCallback callback) { m_keyCallback = callback; }
        void SetMouseButtonCallback(MouseButtonCallback callback) { m_mouseButtonCallback = callback; }
        void SetCursorPosCallback(CursorPosCallback callback) { m_cursorPosCallback = callback; }
        void SetScrollCallback(ScrollCallback callback) { m_scrollCallback = callback; }

        // Input state queries
        bool IsKeyPressed(int key) const;
        bool IsMouseButtonPressed(int button) const;
        void GetCursorPos(double& x, double& y) const;
        void SetCursorMode(int mode); // GLFW_CURSOR_NORMAL, GLFW_CURSOR_DISABLED, etc.

    private:
        void Init();
        void Shutdown();
        void SetupCallbacks();


        static void FramebufferSizeCallbackImpl(GLFWwindow* window, int width, int height);
        static void KeyCallbackImpl(GLFWwindow* window, int key, int scancode, int action, int mods);
        static void MouseButtonCallbackImpl(GLFWwindow* window, int button, int action, int mods);
        static void CursorPosCallbackImpl(GLFWwindow* window, double xpos, double ypos);
        static void ScrollCallbackImpl(GLFWwindow* window, double xoffset, double yoffset);


        GLFWwindow* m_window;
        std::string m_title;
        int m_width;
        int m_height;
        bool m_vsync;

        // Callbacks
        ResizeCallback m_resizeCallback;
        KeyCallback m_keyCallback;
        MouseButtonCallback m_mouseButtonCallback;
        CursorPosCallback m_cursorPosCallback;
        ScrollCallback m_scrollCallback;

        static inline int s_windowCount = 0;
    };

} // namespace Klein

#endif // WINDOW_H