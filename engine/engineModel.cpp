#include "engineModel.hpp"

#include <cassert>
#include <cstring>

namespace gears{

    EngineModel::EngineModel(EngineDevice &device, const std::vector<Vertex> &vertecies) : engineDevice(device){
        createVertexBuffers(vertecies);
    }

    EngineModel::~EngineModel(){
        vkDestroyBuffer(engineDevice.device(), vertexBuffer, nullptr);
        vkFreeMemory(engineDevice.device(), vertexBufferMemory, nullptr);
    }

    void EngineModel::createVertexBuffers(const std::vector<Vertex> &verticies){
        vertexCount = static_cast<uint32_t>(verticies.size());
        assert(vertexCount >=3 && "Vertex count must be at least 3");

        VkDeviceSize bufferSize = sizeof(verticies[0]) * vertexCount;

        engineDevice.createBuffer(  bufferSize, 
                                    VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, 
                                    VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, 
                                    vertexBuffer,
                                    vertexBufferMemory);

        void *data;
        vkMapMemory(engineDevice.device(), vertexBufferMemory, 0, bufferSize, 0, &data);
        memcpy(data, verticies.data(), static_cast<size_t>(bufferSize));
        vkUnmapMemory(engineDevice.device(), vertexBufferMemory);
    }

    void EngineModel::draw(VkCommandBuffer commandBuffer){
        vkCmdDraw(commandBuffer, vertexCount, 1, 0, 0);
    }

    void EngineModel::bind(VkCommandBuffer commandBuffer){
        VkBuffer buffers[] = {vertexBuffer};
        VkDeviceSize offset[] = {0};
        vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offset);
    }

    std::vector<VkVertexInputBindingDescription> EngineModel::Vertex::getBindingDescriptions(){
        std::vector<VkVertexInputBindingDescription> bindingDescriptions(1);
        bindingDescriptions[0].binding = 0;
        bindingDescriptions[0].stride = sizeof(Vertex);
        bindingDescriptions[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
        
        return bindingDescriptions;
    }

    std::vector<VkVertexInputAttributeDescription> EngineModel::Vertex::getAttributeDescriptions(){
        std::vector<VkVertexInputAttributeDescription> attributeDescriptions(1);
        attributeDescriptions[0].binding = 0;
        attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
        attributeDescriptions[0].location = 0;
        attributeDescriptions[0].offset = 0;
        return attributeDescriptions;
    }

}