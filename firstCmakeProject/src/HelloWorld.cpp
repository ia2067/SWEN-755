// Project Headers
#include <HelloWorld.hpp>
#include <HelloWorldPrinter.hpp>

// std headers
#include <iostream>

int main(int argc, char const *argv[])
{
    HelloWorldPrinter hwp;

    std::cout << "Starting Thread..." << std::endl;
    hwp.start();
    hwp.end();
    std::cout << "Thread has joined!" << std::endl;

    return 0;
}
