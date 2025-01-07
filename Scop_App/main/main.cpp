#include "../App/Scop.hpp"

#include <iostream>
   
// obj file 
// https://people.math.sc.edu/Burkardt/data/obj/obj.html

int main(int argc, char** argv) {
    if (argc < 2 || argc > 3)
    {
        std::cerr << "Usage: " << argv[0] << " <obj file> [bmp file]" << std::endl;
        return EXIT_FAILURE;
    }
    
    std::string objFilename{ "models/al.obj" };
    std::string bmpFilename{ "./Scop_App/textures/640-480-sample.bmp" };

    objFilename = argv[1];
    if (argc == 3)
    {
        bmpFilename = argv[2];
        bmpFilename = "./Scop_App/textures/" + bmpFilename;
        std::cout << "Texture file: " << bmpFilename << std::endl;
    }
    else
    {
        std::cout << "Using default texture file: " << bmpFilename << std::endl;
    }

    scop::Scop * scop;
    try
    {
        scop = new scop::Scop();
        if (scop->prepare(objFilename, bmpFilename) == false)
        {
            std::cerr << "Failed to prepare Scop" << std::endl;
            delete scop;
            return EXIT_FAILURE;
        }
        std::cout << "objFile : " << objFilename << "bmpFile : " << bmpFilename << std::endl;

        scop->run();
    }
    catch (const std::bad_alloc& e)
    {
        delete scop;
        std::cerr << "Memory allocation error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    catch (const std::exception& e) {
        delete scop;
        std::cerr << e.what() << '\n';
        return EXIT_FAILURE;
    }

    delete scop;

    return EXIT_SUCCESS;
}