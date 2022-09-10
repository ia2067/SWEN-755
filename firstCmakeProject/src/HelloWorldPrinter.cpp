#include <HelloWorldPrinter.hpp>

#include <iostream>

HelloWorldPrinter::HelloWorldPrinter()
: _thread(NULL), _counter(10)
{ }

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
    // Lambda pointing to print function
    _thread = std::make_shared<boost::thread>( [this] { this->print(); } );
}

void HelloWorldPrinter::end()
{
    _thread->join();
}
