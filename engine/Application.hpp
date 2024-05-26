#pragma once

#include "engineWindow.hpp"
#include "engineDevice.hpp"
#include "engineGameObject.hpp"
#include "engineRenderer.hpp"

#include <memory>
#include <vector>

namespace gears{

    class Application{
    public:
        static constexpr int WIDTH = 800;
        static constexpr int HEIGHT = 600;

        Application();
        ~Application();

        Application(const Application &)= delete;
        Application &operator=(const Application &)= delete;

        void run();
    private:

        void loadGameObjects();

        EngineWindow engineWindow{WIDTH, HEIGHT, "Gears engine goes brrrrrrrrrrrr"};
        EngineDevice engineDevice{engineWindow};
        EngineRenderer engineRenderer{engineWindow, engineDevice};
        std::vector<EngineGameObject> gameObjects;

    };
}