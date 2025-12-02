#include "Window.h"
#include "Logger.h"
#include <stdexcept>

namespace Klein {

    Window::Window(const WindowProps& props)
        : m_title(props.title)
        , m_width(props.width)
        , m_height(props.height)
        , m_vsync(props.vsync)
        , m_window(nullptr)
    {
        Init();
    }

    Window::~Window() {
        Shutdown();
    }

    void Window::Init() {
        if (s_windowCount == 0) {
            if (!glfwInit()) {
                KleinLogger::Logger::EngineError("Failed to initialize GLFW!");
                throw std::runtime_error("GLFW initialization failed");
            }
            KleinLogger::Logger::EngineLog("GLFW initialized successfully");
        }

        // OpenGL version hints
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        
#ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

        // Create window
        m_window = glfwCreateWindow(m_width, m_height, m_title.c_str(), nullptr, nullptr);
        if (!m_window) {
            KleinLogger::Logger::EngineError("Failed to create GLFW window!");
            glfwTerminate();
            throw std::runtime_error("Window creation failed");
        }

        s_windowCount++;
        glfwMakeContextCurrent(m_window);

        // Load OpenGL functions
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            KleinLogger::Logger::EngineError("Failed to initialize GLAD!");
            throw std::runtime_error("GLAD initialization failed");
        }

        KleinLogger::Logger::EngineLog("OpenGL loaded: %s", glGetString(GL_VERSION));
        KleinLogger::Logger::EngineLog("GPU: %s", glGetString(GL_RENDERER));

        SetVSync(m_vsync);
        SetupCallbacks();

        // Enable depth testing by default
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);

        // Store this pointer in GLFW window
        glfwSetWindowUserPointer(m_window, this);
    }

    void Window::Shutdown() {
        if (m_window) {
            glfwDestroyWindow(m_window);
            s_windowCount--;
        }

        if (s_windowCount == 0) {
            glfwTerminate();
            KleinLogger::Logger::EngineLog("GLFW terminated");
        }
    }

    void Window::SetupCallbacks() {
        glfwSetFramebufferSizeCallback(m_window, FramebufferSizeCallbackImpl);
        glfwSetKeyCallback(m_window, KeyCallbackImpl);
        glfwSetMouseButtonCallback(m_window, MouseButtonCallbackImpl);
        glfwSetCursorPosCallback(m_window, CursorPosCallbackImpl);
        glfwSetScrollCallback(m_window, ScrollCallbackImpl);
    }

    void Window::PollEvents() {
        glfwPollEvents();
    }

    void Window::SwapBuffers() {
        glfwSwapBuffers(m_window);
    }

    bool Window::ShouldClose() const {
        return glfwWindowShouldClose(m_window);
    }

    void Window::Close() {
        glfwSetWindowShouldClose(m_window, GLFW_TRUE);
    }

    void Window::SetVSync(bool enabled) {
        glfwSwapInterval(enabled ? 1 : 0);
        m_vsync = enabled;
    }

    bool Window::IsKeyPressed(int key) const {
        return glfwGetKey(m_window, key) == GLFW_PRESS;
    }

    bool Window::IsMouseButtonPressed(int button) const {
        return glfwGetMouseButton(m_window, button) == GLFW_PRESS;
    }

    void Window::GetCursorPos(double& x, double& y) const {
        glfwGetCursorPos(m_window, &x, &y);
    }

    void Window::SetCursorMode(int mode) {
        glfwSetInputMode(m_window, GLFW_CURSOR, mode);
    }

    // Static callback implementations
    void Window::FramebufferSizeCallbackImpl(GLFWwindow* window, int width, int height) {
        Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
        win->m_width = width;
        win->m_height = height;
        glViewport(0, 0, width, height);
        
        if (win->m_resizeCallback) {
            win->m_resizeCallback(width, height);
        }
    }

    void Window::KeyCallbackImpl(GLFWwindow* window, int key, int scancode, int action, int mods) {
        Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
        if (win->m_keyCallback) {
            win->m_keyCallback(key, scancode, action, mods);
        }
    }

    void Window::MouseButtonCallbackImpl(GLFWwindow* window, int button, int action, int mods) {
        Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
        if (win->m_mouseButtonCallback) {
            win->m_mouseButtonCallback(button, action, mods);
        }
    }

    void Window::CursorPosCallbackImpl(GLFWwindow* window, double xpos, double ypos) {
        Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
        if (win->m_cursorPosCallback) {
            win->m_cursorPosCallback(xpos, ypos);
        }
    }

    void Window::ScrollCallbackImpl(GLFWwindow* window, double xoffset, double yoffset) {
        Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
        if (win->m_scrollCallback) {
            win->m_scrollCallback(xoffset, yoffset);
        }
    }

} // namespace Klein