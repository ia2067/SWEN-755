#include <iostream>

#include <HelloApplication.hpp>



int main(int argc, char *argv[])
{
    std::cout << "Assignment 4" << std::endl;

    return Wt::WRun(argc, argv, [](const Wt::WEnvironment& env) {
        return std::make_unique<Assignment4::HelloApplication>(env);
    });
}
