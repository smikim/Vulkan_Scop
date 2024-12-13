#include "Scop.h"

#include <iostream>

int main() {
  
    scop::Scop * scop;
    try
    {
        scop = new scop::Scop();

 
        scop->run();
    }
    catch (const std::bad_alloc& e)
    {
        std::cerr << "malloc error: " << e.what() << std::endl;
        return false;
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
        return EXIT_FAILURE;
    }

    delete scop;

    return EXIT_SUCCESS;

}