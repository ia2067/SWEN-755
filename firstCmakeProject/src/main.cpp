#include <iostream>

#include <HelloWorldPrinter.hpp>

int main(int, char**) {
    HelloWorldPrinter hwp;

    std::cout << "Starting Thread..." << std::endl;
    hwp.start();
    hwp.end();
    std::cout << "Thread has joined!" << std::endl;
}
