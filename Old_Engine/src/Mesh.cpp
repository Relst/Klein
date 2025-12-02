#include "Mesh.h"
#include "Logger.h"
#include <cmath>

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#endif

namespace Klein {

    // ===== Texture Implementation =====
    Texture::Texture(const std::string& path, Type type)
        : m_path(path), m_type(type)
    {
        stbi_set_flip_vertically_on_load(true);
        
        unsigned char* data = stbi_load(path.c_str(), &m_width, &m_height, &m_channels, 0);
        
        if (!data) {
            KleinLogger::Logger::EngineError("Failed to load texture: %s", path.c_str());
            m_textureID = 0;
            return;
        }

        glGenTextures(1, &m_textureID);
        glBindTexture(GL_TEXTURE_2D, m_textureID);

        GLenum format = GL_RGB;
        if (m_channels == 1) format = GL_RED;
        else if (m_channels == 3) format = GL_RGB;
        else if (m_channels == 4) format = GL_RGBA;

        glTexImage2D(GL_TEXTURE_2D, 0, format, m_width, m_height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
        KleinLogger::Logger::EngineLog("Texture loaded: %s (%dx%d, %d channels)", 
            path.c_str(), m_width, m_height, m_channels);
    }

    Texture::~Texture() {
        if (m_textureID != 0) {
            glDeleteTextures(1, &m_textureID);
        }
    }

    void Texture::Bind(unsigned int slot) const {
        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(GL_TEXTURE_2D, m_textureID);
    }

    void Texture::Unbind() const {
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    std::shared_ptr<Texture> Texture::CreateFromData(
        unsigned char* data, 
        int width, 
        int height, 
        int channels,
        Type type)
    {
        auto texture = std::make_shared<Texture>("", type);
        
        glGenTextures(1, &texture->m_textureID);
        glBindTexture(GL_TEXTURE_2D, texture->m_textureID);

        GLenum format = channels == 4 ? GL_RGBA : GL_RGB;
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        return texture;
    }

    // ===== Material Implementation =====
    Material::Material() {}

    // ===== Mesh Implementation =====
    Mesh::Mesh(const std::vector<Vertex>& verts, const std::vector<unsigned int>& inds)
        : vertices(verts), indices(inds)
    {
        SetupMesh();
    }

    Mesh::~Mesh() {
        glDeleteVertexArrays(1, &m_VAO);
        glDeleteBuffers(1, &m_VBO);
        glDeleteBuffers(1, &m_EBO);
    }

    void Mesh::SetupMesh() {
        glGenVertexArrays(1, &m_VAO);
        glGenBuffers(1, &m_VBO);
        glGenBuffers(1, &m_EBO);

        glBindVertexArray(m_VAO);

        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

        // Position
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
        
        // Normal
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
        
        // TexCoords
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));
        
        // Tangent
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent));
        
        // Bitangent
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, bitangent));

        glBindVertexArray(0);
    }

    void Mesh::Draw() const {
        glBindVertexArray(m_VAO);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

    // Primitive generators
    std::shared_ptr<Mesh> Mesh::CreateCube() {
        std::vector<Vertex> vertices = {
            // Front face
            {{-0.5f, -0.5f,  0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}},
            {{ 0.5f, -0.5f,  0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}},
            {{ 0.5f,  0.5f,  0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
            {{-0.5f,  0.5f,  0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},
            // Back, Top, Bottom, Left, Right faces...
            // (simplified for brevity - full cube has 24 vertices)
        };
        
        std::vector<unsigned int> indices = {
            0, 1, 2, 2, 3, 0  // Front face
            // Additional faces...
        };

        return std::make_shared<Mesh>(vertices, indices);
    }

    std::shared_ptr<Mesh> Mesh::CreateSphere(int segments) {
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;

        for (int y = 0; y <= segments; y++) {
            for (int x = 0; x <= segments; x++) {
                float xSegment = (float)x / (float)segments;
                float ySegment = (float)y / (float)segments;
                float xPos = std::cos(xSegment * 2.0f * 3.14159f) * std::sin(ySegment * 3.14159f);
                float yPos = std::cos(ySegment * 3.14159f);
                float zPos = std::sin(xSegment * 2.0f * 3.14159f) * std::sin(ySegment * 3.14159f);

                Vertex vertex;
                vertex.position = {xPos, yPos, zPos};
                vertex.normal = {xPos, yPos, zPos};
                vertex.texCoords = {xSegment, ySegment};
                vertices.push_back(vertex);
            }
        }

        for (int y = 0; y < segments; y++) {
            for (int x = 0; x < segments; x++) {
                indices.push_back(y * (segments + 1) + x);
                indices.push_back((y + 1) * (segments + 1) + x);
                indices.push_back((y + 1) * (segments + 1) + x + 1);

                indices.push_back(y * (segments + 1) + x);
                indices.push_back((y + 1) * (segments + 1) + x + 1);
                indices.push_back(y * (segments + 1) + x + 1);
            }
        }

        return std::make_shared<Mesh>(vertices, indices);
    }

    std::shared_ptr<Mesh> Mesh::CreatePlane(float width, float height) {
        std::vector<Vertex> vertices = {
            {{-width/2, 0.0f, -height/2}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
            {{ width/2, 0.0f, -height/2}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
            {{ width/2, 0.0f,  height/2}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f}},
            {{-width/2, 0.0f,  height/2}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f}}
        };

        std::vector<unsigned int> indices = {0, 1, 2, 2, 3, 0};
        return std::make_shared<Mesh>(vertices, indices);
    }

    std::shared_ptr<Mesh> Mesh::CreateQuad() {
        return CreatePlane(2.0f, 2.0f);
    }

} // namespace Klein