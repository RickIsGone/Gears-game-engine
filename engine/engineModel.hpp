#pragma once

#include "engineDevice.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

#include <vector>

namespace gears{

    class EngineModel{
    public:

        struct Vertex{
            glm::vec3 position;
            glm::vec3 color;
            
            static std::vector<VkVertexInputBindingDescription> getBindingDescriptions();
            static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();
        };

        struct Data{
            std::vector<Vertex> vertices{};
            std::vector<uint32_t> indices{};
        };

        EngineModel(EngineDevice &device, const EngineModel::Data &data);
        ~EngineModel();

        EngineModel(const EngineModel &) = delete;
        EngineModel &operator=(const EngineModel &) = delete;

        void bind(VkCommandBuffer commandBuffer);
        void draw(VkCommandBuffer commandBuffer);
        
    private:    
        EngineDevice &engineDevice;

        VkBuffer vertexBuffer;
        VkDeviceMemory vertexBufferMemory;
        uint32_t vertexCount;
        void createVertexBuffers(const std::vector<Vertex> &vertices);

        bool hasIndexBuffer = false;
        VkBuffer indexBuffer;
        VkDeviceMemory indexBufferMemory;
        uint32_t indexCount;
        void createIndexBuffer(const std::vector<uint32_t> &indices);
    };

}