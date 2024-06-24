#pragma once

#include "engineDevice.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

#include <vector>
#include <memory>

namespace gears {

   class EngineModel {
   public:
      struct Vertex {
         glm::vec3 position{};
         glm::vec3 color{};
         glm::vec3 normal{};
         glm::vec2 uv{};

         static std::vector<VkVertexInputBindingDescription> getBindingDescriptions();
         static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();

         bool operator==(const Vertex &other) const {
            return position == other.position && color == other.color && normal == other.normal && uv == other.uv;
         }
      };

      struct Data {
         std::vector<Vertex> vertices{};
         std::vector<uint32_t> indices{};

         void loadModel(const std::string &filepath);
      };

      EngineModel(EngineDevice &device, const EngineModel::Data &data);
      ~EngineModel();

      EngineModel(const EngineModel &) = delete;
      EngineModel &operator=(const EngineModel &) = delete;

      static std::unique_ptr<EngineModel> createModelFromFile(EngineDevice &device, const std::string &filepath);

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

} // namespace gears