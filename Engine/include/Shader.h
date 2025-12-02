#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <unordered_map>
#include <glad/glad.h>
#include <glm/glm.hpp>

namespace Klein {

    class Shader {
    public:
        Shader(const std::string& vertexSrc, const std::string& fragmentSrc);
        ~Shader();

        void Bind() const;
        void Unbind() const;

        // Uniform setters
        void SetInt(const std::string& name, int value);
        void SetFloat(const std::string& name, float value);
        void SetVec2(const std::string& name, const glm::vec2& value);
        void SetVec3(const std::string& name, const glm::vec3& value);
        void SetVec4(const std::string& name, const glm::vec4& value);
        void SetMat3(const std::string& name, const glm::mat3& value);
        void SetMat4(const std::string& name, const glm::mat4& value);

        GLuint GetProgramID() const { return m_program; }

        // Load from files
        static std::shared_ptr<Shader> LoadFromFiles(
            const std::string& vertexPath,
            const std::string& fragmentPath
        );

    private:
        GLuint CompileShader(GLenum type, const std::string& source);
        GLint GetUniformLocation(const std::string& name);

        GLuint m_program;
        std::unordered_map<std::string, GLint> m_uniformLocationCache;
    };

    // Shader Library for managing shaders
    class ShaderLibrary {
    public:
        static ShaderLibrary& Get();

        void Add(const std::string& name, std::shared_ptr<Shader> shader);
        std::shared_ptr<Shader> Load(const std::string& name, 
                                      const std::string& vertPath, 
                                      const std::string& fragPath);
        std::shared_ptr<Shader> Get(const std::string& name);
        
        void CreateDefaultShaders();

    private:
        ShaderLibrary() = default;
        std::unordered_map<std::string, std::shared_ptr<Shader>> m_shaders;
    };

} // namespace Klein

#endif // SHADER_H