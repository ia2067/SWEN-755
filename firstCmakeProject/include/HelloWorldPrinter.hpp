#include <thread>
#include <memory>

class HelloWorldPrinter
{
public: // CTOR and DTOR
    HelloWorldPrinter();
    ~HelloWorldPrinter();

public: // Methods
    void print();
    void start();
    void end();

private:
    std::shared_ptr<std::thread> _thread;
    int _counter;
};
