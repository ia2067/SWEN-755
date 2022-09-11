#include <iostream>
#include <common/HeartbeatSender.hpp>

int main(int argc, char const *argv[])
{
    std::cout << "Assignment 1" << std::endl;

    Common::HeartbeatSender hs("poopy", 1500);
    return 0;
}
