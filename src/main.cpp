
#include "App.h"


#include <system_error>
#include <windows.h>



int main(int argc, char* argv[]) {
    try {
        App app(argc, argv);      
        app.run();    
    }
    catch (const std::system_error& e) {
        std::string msg = std::string("Blad aplikacji:\n") + e.what() + "\n\nKod bledu: " + std::to_string(e.code().value());

        MessageBoxA(nullptr, msg.c_str(), "Blad aplikacji", MB_OK | MB_ICONERROR);
        return EXIT_FAILURE;
    }
    catch (const std::exception& e) {
        MessageBoxA(nullptr, e.what(), "Unexpected error", MB_OK | MB_ICONERROR);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}