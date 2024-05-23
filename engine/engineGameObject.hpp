#pragma once

#include "engineModel.hpp"

#include <memory>


namespace gears{

    struct Transform2dComponents{
        glm::vec2 translation{};
        glm::vec2 scale{1.0f, 1.0f};
        float rotation;

        glm::mat2 mat2() { 
            const float s = glm::sin(rotation);
            const float c = glm::cos(rotation);
            glm::mat2 rotationMat{{c, s},{-s, c}};

            glm::mat2 scaleMat{{scale.x, 0.0f},{0.0f, scale.y}};
            return rotationMat * scaleMat; 
        }
    };

    class EngineGameObject{
    public:
        using id_t = unsigned int;

        static EngineGameObject createGameObject(){
            static id_t currentId = 0;
            return EngineGameObject{currentId++};
        }

        EngineGameObject(const EngineGameObject &) = delete;
        EngineGameObject &operator=(const EngineGameObject &) = delete;
        EngineGameObject(EngineGameObject &&) = default;
        EngineGameObject &operator=(EngineGameObject &&) = default;

        id_t getId() { return id; };

        std::shared_ptr<EngineModel> model{};
        glm::vec3 color{};

        Transform2dComponents transform2d{};

    private:

        EngineGameObject(id_t objId) : id{objId}{}
        id_t id;
    };
}