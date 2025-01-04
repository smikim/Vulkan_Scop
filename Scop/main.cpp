#include "Scop.h"

#include <iostream>
   
// obj file 
// https://people.math.sc.edu/Burkardt/data/obj/obj.html

int main(int argc, char *argv) {

    /*if (argc < 2 || argc > 3)
    {
        std::cerr << "Usage: " << argv[0] << " <obj file> [bmp file]" << std::endl;
        return EXIT_FAILURE;
    }*/
    
    std::string objFilename{ "models/al.obj" };
    std::string bmpFilename{ "textures/640-480-sample.bmp" };

    //std::cout << argv[0] << ">> " << argv[1] << " >> " << argv[2] << std::endl;

    //if (argc == 3)
    //{
    //    bmpFilename = argv[2];
    //    std::cout << "Texture file: " << bmpFilename << std::endl;
    //}
    //else
    //{
    //    // 기본 텍스춰 파일 지정 
    //    std::cout << "Using default texture file: " << bmpFilename << std::endl;
    //}

    //std::string filename = "models/42.obj";
    //std::string filename = "models/teapot.obj";
    //std::string filename = "models/teapot2.obj";
    //std::string filename = "models/humanoid_quad.obj";
    //std::string filename = "models/humanoid_tri.obj";
    //std::string filename = "models/monkey.obj";
    //std::string filename = "models/al.obj";
    //std::string filename = "models/power_lines.obj";
    //std::string filename = "models/skyscraper.obj";
    //std::string filename = "models/sandal.obj";
    //std::string filename = "models/lamp.obj";
    //std::string filename = "models/shuttle.obj";
  
    

    //std::string bmpFilename{ "textures/lena.bmp" };
    //std::string filename{ "textures/sample.bmp" };

    //std::string filename{ "textures/640-480-sample.bmp" };
    //std::string filename{ "textures/blackbuck.bmp" };
    //std::string filename{ "textures/bmp_24.bmp" };

    //std::string filename{ "textures/dots.bmp" };


    scop::Scop * scop;
    try
    {
        scop = new scop::Scop();
        // pass obj file
        // filename 2 개로 넘겨 준다?
        scop->prepare(objFilename, bmpFilename);
        std::cout << objFilename << bmpFilename << std::endl;
        scop->run();
    }
    catch (const std::bad_alloc& e)
    {
        std::cerr << "Memory allocation error: " << e.what() << std::endl;
        return false;
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
        return EXIT_FAILURE;
    }

    delete scop;

    return EXIT_SUCCESS;
}