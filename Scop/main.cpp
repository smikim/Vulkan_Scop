#include "Scop.h"

#include <iostream>

#include "test.h"

void mathTest()
{
   //multi_matrix();
   //compareMat4AndGLM();
   //testLookAt();
   //testLookAt2();
   //
   //testLookAt3();
   //comparePerspectiveFunctions();
   testRotationFunctions();
   //testRotationFunctions2();
   //testScaleFunctions();
   //testTranslateFunctions();
}
   


int main() {

    mathTest();
    
    scop::Scop * scop;
    try
    {
        scop = new scop::Scop();
        scop->prepare();
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