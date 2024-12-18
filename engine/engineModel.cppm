module;

#include <cstring>
#include <unordered_map>
#include <vector>
#include <memory>

#include <vulkan/vulkan.hpp>
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

#include "engineUtils.hpp"
#include "macro.hpp"

export module engineModel;
import engine.device;
import engine.logger;

namespace gears {

   export class EngineModel {
   public:
      struct Vertex {
         glm::vec3 position{};
         glm::vec3 color{};
         glm::vec3 normal{};
         glm::vec2 uv{};

         static std::vector<VkVertexInputBindingDescription> getBindingDescriptions();
         static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();

         bool operator==(const Vertex& other) const {
            return position == other.position && color == other.color && normal == other.normal && uv == other.uv;
         }
      };

      struct Data {
         std::vector<Vertex> vertices{};
         std::vector<uint32_t> indices{};

         void loadModel(const std::string& filepath);
      };

      EngineModel(PhysicalDevice& device, const EngineModel::Data& data);
      ~EngineModel();

      EngineModel(const EngineModel&) = delete;
      EngineModel& operator=(const EngineModel&) = delete;

      static std::unique_ptr<EngineModel> createModelFromFile(PhysicalDevice& device, const std::string& filepath);

      void bind(VkCommandBuffer commandBuffer);
      void draw(VkCommandBuffer commandBuffer);

   private:
      PhysicalDevice& _device;

      VkBuffer _vertexBuffer;
      VkDeviceMemory _vertexBufferMemory;
      uint32_t _vertexCount;
      void _createVertexBuffers(const std::vector<Vertex>& vertices);

      bool _hasIndexBuffer = false;
      VkBuffer _indexBuffer;
      VkDeviceMemory _indexBufferMemory;
      uint32_t _indexCount;
      void _createIndexBuffer(const std::vector<uint32_t>& indices);
   };

} // namespace gears

namespace std {
   export template <>
   struct hash<gears::EngineModel::Vertex> {
      size_t operator()(gears::EngineModel::Vertex const& vertex) const {
         size_t seed = 0;
         gears::hashCombine(seed, vertex.position, vertex.color, vertex.normal, vertex.uv);
         return seed;
      }
   };
} // namespace std

namespace gears {

   EngineModel::EngineModel(PhysicalDevice& device, const EngineModel::Data& data) : _device(device) {
      _createVertexBuffers(data.vertices);
      _createIndexBuffer(data.indices);
   }

   EngineModel::~EngineModel() {
      vkDestroyBuffer(_device.device(), _vertexBuffer, nullptr);
      vkFreeMemory(_device.device(), _vertexBufferMemory, nullptr);

      if (_hasIndexBuffer) {
         vkDestroyBuffer(_device.device(), _indexBuffer, nullptr);
         vkFreeMemory(_device.device(), _indexBufferMemory, nullptr);
      }
   }

   std::unique_ptr<EngineModel> EngineModel::createModelFromFile(PhysicalDevice& device, const std::string& filepath) {
      Data data{};
      data.loadModel(filepath);
      logger->logTrace("vertex count: {}", data.vertices.size());
      return std::make_unique<EngineModel>(device, data);
   }

   void EngineModel::_createVertexBuffers(const std::vector<Vertex>& vertices) {
      _vertexCount = static_cast<uint32_t>(vertices.size());
      GRS_LOG_ASSERT(_vertexCount >= 3, "Vertex count must be at least 3");

      VkDeviceSize bufferSize = sizeof(vertices[0]) * _vertexCount;

      VkBuffer stagingBuffer;
      VkDeviceMemory stagingBufferMemory;

      _device.createBuffer(
          bufferSize,
          VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
          VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
          stagingBuffer,
          stagingBufferMemory);

      void* data;
      vkMapMemory(_device.device(), stagingBufferMemory, 0, bufferSize, 0, &data);
      memcpy(data, vertices.data(), static_cast<size_t>(bufferSize));
      vkUnmapMemory(_device.device(), stagingBufferMemory);

      _device.createBuffer(
          bufferSize,
          VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
          VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
          _vertexBuffer,
          _vertexBufferMemory);

      _device.copyBuffer(stagingBuffer, _vertexBuffer, bufferSize);

      vkDestroyBuffer(_device.device(), stagingBuffer, nullptr);
      vkFreeMemory(_device.device(), stagingBufferMemory, nullptr);
   }

   void EngineModel::_createIndexBuffer(const std::vector<uint32_t>& indices) {
      _indexCount = static_cast<uint32_t>(indices.size());
      _hasIndexBuffer = _indexCount > 0;
      if (!_hasIndexBuffer) return;

      VkDeviceSize bufferSize = sizeof(indices[0]) * _indexCount;

      VkBuffer stagingBuffer;
      VkDeviceMemory stagingBufferMemory;

      _device.createBuffer(
          bufferSize,
          VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
          VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
          stagingBuffer,
          stagingBufferMemory);

      void* data;
      vkMapMemory(_device.device(), stagingBufferMemory, 0, bufferSize, 0, &data);
      memcpy(data, indices.data(), static_cast<size_t>(bufferSize));
      vkUnmapMemory(_device.device(), stagingBufferMemory);

      _device.createBuffer(
          bufferSize,
          VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
          VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
          _indexBuffer,
          _indexBufferMemory);

      _device.copyBuffer(stagingBuffer, _indexBuffer, bufferSize);

      vkDestroyBuffer(_device.device(), stagingBuffer, nullptr);
      vkFreeMemory(_device.device(), stagingBufferMemory, nullptr);
   }

   void EngineModel::draw(VkCommandBuffer commandBuffer) {
      if (_hasIndexBuffer) vkCmdDrawIndexed(commandBuffer, _indexCount, 1, 0, 0, 0);
      else
         vkCmdDraw(commandBuffer, _vertexCount, 1, 0, 0);
   }

   void EngineModel::bind(VkCommandBuffer commandBuffer) {
      VkBuffer buffers[] = {_vertexBuffer};
      VkDeviceSize offset[] = {0};
      vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offset);

      if (_hasIndexBuffer) vkCmdBindIndexBuffer(commandBuffer, _indexBuffer, 0, VK_INDEX_TYPE_UINT32);
   }

   std::vector<VkVertexInputBindingDescription> EngineModel::Vertex::getBindingDescriptions() {
      std::vector<VkVertexInputBindingDescription> bindingDescriptions(1);
      bindingDescriptions[0].binding = 0;
      bindingDescriptions[0].stride = sizeof(Vertex);
      bindingDescriptions[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

      return bindingDescriptions;
   }

   std::vector<VkVertexInputAttributeDescription> EngineModel::Vertex::getAttributeDescriptions() {
      std::vector<VkVertexInputAttributeDescription> attributeDescriptions{0};

      attributeDescriptions.push_back({0, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, position)});
      attributeDescriptions.push_back({1, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, color)});
      attributeDescriptions.push_back({2, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, normal)});
      attributeDescriptions.push_back({3, 0, VK_FORMAT_R32G32_SFLOAT, offsetof(Vertex, uv)});

      return attributeDescriptions;
   }

   void EngineModel::Data::loadModel(const std::string& filepath) {
      tinyobj::attrib_t attrib;
      std::vector<tinyobj::shape_t> shapes;
      std::vector<tinyobj::material_t> materials;
      std::string warn, err;

      if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filepath.c_str())) throw Logger::Exception("{}{}", warn, err);

      vertices.clear();
      indices.clear();

      std::unordered_map<Vertex, uint32_t> uniqueVertices{};

      for (const auto& shape : shapes) {
         for (const auto& index : shape.mesh.indices) {
            Vertex vertex{};

            if (index.vertex_index >= 0) {
               vertex.position = {
                   attrib.vertices[3 * index.vertex_index + 0],
                   attrib.vertices[3 * index.vertex_index + 1],
                   attrib.vertices[3 * index.vertex_index + 2]};

               vertex.color = {
                   attrib.colors[3 * index.vertex_index + 0],
                   attrib.colors[3 * index.vertex_index + 2],
                   attrib.colors[3 * index.vertex_index + 1]};
            }

            if (index.normal_index >= 0) {
               vertex.normal = {
                   attrib.normals[3 * index.normal_index + 0],
                   attrib.normals[3 * index.normal_index + 1],
                   attrib.normals[3 * index.normal_index + 2]};
            }

            if (index.normal_index >= 0) {
               vertex.uv = {
                   attrib.texcoords[2 * index.texcoord_index + 0],
                   attrib.texcoords[2 * index.texcoord_index + 1]};
            }

            if (uniqueVertices.count(vertex) == 0) {
               uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
               vertices.push_back(vertex);
            }
            indices.push_back(uniqueVertices[vertex]);
         }
      }
   }
} // namespace gears
