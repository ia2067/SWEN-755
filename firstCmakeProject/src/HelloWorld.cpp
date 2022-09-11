// Project Headers
#include <HelloWorld.hpp>
#include <HelloWorldPrinter.hpp>

// boost headers
#include <boost/program_options.hpp>

// std headers
#include <iostream>

int main(int argc, char const *argv[])
{
    std::string mq;
    namespace po = boost::program_options;
    po::options_description desc("Hello World Helper");
    desc.add_options()
        ("help", "produce help message")
        ("mq", po::value<std::string>(&mq), "set messaging queue name")
    ;

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if(vm.count("help") || !vm.count("mq")) {
        std::cout << desc << std::endl;
        return -1;
    }

    HelloWorldPrinter hwp(mq);

    std::cout << "Starting Thread..." << std::endl;
    hwp.start();
    hwp.end();
    std::cout << "Thread has joined!" << std::endl;

    return 0;
}
