#include "firstApp.hpp"

namespace engine{

    void FirstApp::run(){
        while(!engineWindow.shouldClose()){
            glfwPollEvents();
        }

        engineWindow.~EngineWindow();           // da tenere per ora 
    }
    
}