#include <HelloWorldPrinter.hpp>

#include <iostream>



HelloWorldPrinter::HelloWorldPrinter(std::string messageQueueName)
: _thread(NULL),
  _counter(10)
{ 
    _pMessageQueue = std::make_shared<boost::interprocess::message_queue>(
                        boost::interprocess::open_or_create,
                        messageQueueName.c_str(),
                        100,
                        100);
}

HelloWorldPrinter::~HelloWorldPrinter()
{ }

void HelloWorldPrinter::print()
{
    while(_counter > 0)
    {
        std::cout << _counter << std::endl;
        _counter--;
        boost::this_thread::sleep_for(boost::chrono::milliseconds(250));
    }
    std::cout << "Hello World!" << std::endl;
}

void HelloWorldPrinter::start()
{  
    bool waiting = true;

    std::size_t buf_size(100), rcvd_size;
    unsigned int pri;
    char* buf = new char[buf_size];

    while(waiting)
    {
        if(_pMessageQueue->try_receive((void*) buf, buf_size, rcvd_size, pri))
        {
            std::string rcved_msg(buf);

            // std::cout << "Got message: " << rcved_msg << std::endl;   

            if(0 == rcved_msg.compare("START"))
            {
                waiting = false;
                break;
            }

            boost::this_thread::sleep_for(boost::chrono::milliseconds(250));
        }
    }
    // Lambda pointing to print function
    _thread = std::make_shared<boost::thread>( [this] { this->print(); } );
}

void HelloWorldPrinter::end()
{
    _thread->join();
}
