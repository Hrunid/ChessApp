#include <iostream>
#include "App.hpp"

int main() {
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