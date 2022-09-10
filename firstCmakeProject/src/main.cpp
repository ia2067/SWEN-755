#include <iostream>

#include <boost/process.hpp>

int main(int argc, char const *argv[])
{
    namespace bp = boost::process;

    std::cout << "Creating child process..." << std::endl;
    bp::child c("./HelloWorld_helper");

    std::cout << "... waiting on child process to end" << std::endl;
    while(c.running())
    {
        ; //do nothing
    }
    std::cout << "Child process done!" << std::endl;
    c.wait();
    return 0;
}
