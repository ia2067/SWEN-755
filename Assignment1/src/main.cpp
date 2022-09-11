#include <iostream>
#include <common/HeartbeatSender.hpp>

int main(int argc, char const *argv[])
{
    std::cout << "Assignment 1" << std::endl;
    
    boost::interprocess::message_queue::remove("poopy");

    Common::HeartbeatSender hs("poopy", boost::chrono::milliseconds(1500));

    hs.start();

    std::string input;
    std::cin >> input;
    
    hs.end();
    return 0;
}
