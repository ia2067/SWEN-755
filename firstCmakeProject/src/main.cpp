#include <iostream>

#include <boost/process.hpp>
#include <boost/thread.hpp>
#include <boost/interprocess/ipc/message_queue.hpp>

int main(int argc, char const *argv[])
{
    namespace bp = boost::process;

    std::cout << "Creating child process..." << std::endl;
    bp::child c("./HelloWorld_helper --mq poopy");

    auto pMQ = std::make_shared<boost::interprocess::message_queue>(
                        boost::interprocess::open_or_create,
                        "poopy",
                        100,
                        100);

    sleep(1);

    while(c.running())
    {

        std::string str;
        std::cout << "Enter message for other process: ";
        std::cin >> str;

        size_t str_len = str.length();
        if(str_len > 99) {
            std::cout << "Message is too long" << std::endl;
            continue;
        }
        
        char msg[100];
        str.copy(msg, str_len);
        msg[str_len] = '\0';
        if(!pMQ->try_send(msg, str_len + 1, 1))
        {
            std::cout << "Message sending failed..." << std::endl;
        }
        sleep(5);
    }
    std::cout << "... waiting on child process to end" << std::endl;
    c.wait();
    return 0;
}
