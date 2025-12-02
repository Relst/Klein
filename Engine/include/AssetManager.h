//
// Created by Karanbir Singh on 26/11/2025.
//

#ifndef ASSETMANAGER_H
#define ASSETMANAGER_H

#include <string>
#include <unordered_map>
#include <memory>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace Klein
{
    class AssetManager
    {
    public:
        static AssetManager& GetInstance();

        const aiScene* LoadModel(const std::string& path);

    private:
        AssetManager() = default;
        std::unordered_map<std::string, const aiScene*> m_modelCache;
        Assimp::Importer m_importer;
    };
}




#endif //ASSETMANAGER_H
