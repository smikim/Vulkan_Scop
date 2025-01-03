#include "Scop.h"

#include <iostream>
   
// obj file 
// https://people.math.sc.edu/Burkardt/data/obj/obj.html

int main() {

    //mathTest();
    
    //std::string filename = "models/42.obj";
    //std::string filename = "models/teapot.obj";
    //std::string filename = "models/teapot2.obj";
    //std::string filename = "models/humanoid_quad.obj";
    //std::string filename = "models/humanoid_tri.obj";
    //std::string filename = "models/monkey.obj";
    //std::string filename = "models/al.obj";
    //std::string filename = "models/power_lines.obj";
    std::string filename = "models/skyscraper.obj";
    //std::string filename = "models/sandal.obj";
    //std::string filename = "models/lamp.obj";
    //std::string filename = "models/shuttle.obj";
  

    scop::Scop * scop;
    try
    {
        scop = new scop::Scop();
        // pass obj file
        scop->prepare(filename);
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