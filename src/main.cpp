#include <iostream>
#include <cstdlib>
#include <stdexcept>

#include "apps/first_app.hpp"

int main() {
    
    bor::FirstApp app{};

    try{
        app.run();
    } catch(std::exception& e) {
        std::cerr << e.what() << '\n';
        return EXIT_SUCCESS;
    }
    
    return EXIT_FAILURE;
}