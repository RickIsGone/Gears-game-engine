#include "firstApp.hpp" 

#include "macro.hpp"

#include <iostream>
#include <cstdlib>
#include <stdexcept>


int main(int argc, char *argv[]){
	
	gears::FirstApp app{};

	try{
		app.run();
	} catch(const std::exception &e){
		std::cerr << e.what() << '\n';
		
		GEARS_DEBUG_WAIT;

		return EXIT_FAILURE;
	}
	
	
	GEARS_DEBUG_WAIT;
	
	return EXIT_SUCCESS;
}
