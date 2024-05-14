#include "firstApp.hpp" 

#include "macro.hpp"

#include <iostream>
#include <cstdlib>
#include <stdexcept>


int main(int argc, char *argv[]){
	
	engine::FirstApp app{};

	try{
		app.run();
	} catch(const std::exception &e){
		std::cerr << e.what() << '\n';
		return EXIT_FAILURE;
	}
	
	
	ENGINE_DEBUG_WAIT;
	
	return EXIT_SUCCESS;
}
