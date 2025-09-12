#include <iostream>
#include "App.h"

int main(int argc, char* argv[]) {
    try {
        App app;      
        app.run();    
    }
    catch (const std::exception& e) {
        std::cerr << "Unexpected error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}