#include <boost/thread.hpp>
#include <boost/interprocess/ipc/message_queue.hpp>
#include <memory> // shared_ptr

class HelloWorldPrinter
{
public: // CTOR and DTOR
    HelloWorldPrinter(std::string messageQueueName);
    ~HelloWorldPrinter();

public: // Methods
    void print();
    void start();
    void end();

private:
    std::shared_ptr<boost::thread> _thread;
    std::shared_ptr<boost::interprocess::message_queue> _pMessageQueue;
    int _counter;
};
