//
// Created by Karanbir Singh on 26/11/2025.
//

#include "AssetManager.h"
#include <iostream>

namespace Klein
{
    AssetManager& AssetManager::GetInstance()
    {
        static AssetManager instance;
        return instance;
    }

    const aiScene* AssetManager::LoadModel(const std::string& path)
    {
        if (m_modelCache.contains(path))
            return m_modelCache[path];

        const aiScene* scene = m_importer.ReadFile(
            path,
            aiProcess_Triangulate |
            aiProcess_FlipUVs |
            aiProcess_CalcTangentSpace
        );

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            std::cerr << "ASSIMP ERROR: " << m_importer.GetErrorString() << std::endl;
            return nullptr;
        }

        m_modelCache[path] = scene;
        return scene;
    }
}
