#ifndef MESH_H
#define MESH_H

#include <vector>
#include <memory>
#include <string>
#include <glm/glm.hpp>
#include <glad/glad.h>

namespace Klein {

    struct Vertex {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 texCoords;
        glm::vec3 tangent;
        glm::vec3 bitangent;
    };

    class Texture {
    public:
        enum class Type {
            Diffuse,
            Specular,
            Normal,
            Height,
            Ambient
        };

        Texture(const std::string& path, Type type);
        ~Texture();

        void Bind(unsigned int slot = 0) const;
        void Unbind() const;

        GLuint GetID() const { return m_textureID; }
        Type GetType() const { return m_type; }
        const std::string& GetPath() const { return m_path; }

        // Create texture from raw data
        static std::shared_ptr<Texture> CreateFromData(
            unsigned char* data, 
            int width, 
            int height, 
            int channels,
            Type type = Type::Diffuse
        );

    private:
        GLuint m_textureID;
        Type m_type;
        std::string m_path;
        int m_width, m_height, m_channels;
    };

    class Material {
    public:
        Material();
        ~Material() = default;

        // PBR Material properties
        glm::vec3 albedo{1.0f, 1.0f, 1.0f};
        float metallic = 0.0f;
        float roughness = 0.5f;
        float ao = 1.0f; // Ambient occlusion

        // Textures
        std::shared_ptr<Texture> albedoMap;
        std::shared_ptr<Texture> normalMap;
        std::shared_ptr<Texture> metallicMap;
        std::shared_ptr<Texture> roughnessMap;
        std::shared_ptr<Texture> aoMap;

        // Shader to use (we'll implement a simple shader manager)
        std::string shaderName = "default";
    };

    class Mesh {
    public:
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;

        Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);
        ~Mesh();

        void Draw() const;

        // Primitive mesh generators
        static std::shared_ptr<Mesh> CreateCube();
        static std::shared_ptr<Mesh> CreateSphere(int segments = 32);
        static std::shared_ptr<Mesh> CreatePlane(float width = 1.0f, float height = 1.0f);
        static std::shared_ptr<Mesh> CreateQuad();

        GLuint GetVAO() const { return m_VAO; }

    private:
        void SetupMesh();

        GLuint m_VAO, m_VBO, m_EBO;
    };

} // namespace Klein

#endif // MESH_H