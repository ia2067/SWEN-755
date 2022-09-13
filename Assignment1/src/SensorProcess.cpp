// boost headers
#include <boost/program_options.hpp>
#include <boost/process.hpp>

// std headers
#include <iostream>

#include <Sensor.hpp>

int main(int argc, char const *argv[])
{
    std::string mq, id;
    int size;
    float scaleFactor;

    namespace po = boost::program_options;
    po::options_description desc("Sensor Process");
    desc.add_options()
        ("help", "produce help message")
        ("size", po::value<int>(&size), "set sample size")
        ("scaleFactor", po::value<float>(&scaleFactor), "set scale factor")
        ("mq", po::value<std::string>(&mq), "set messaging queue name")
        ("id", po::value<std::string>(&id), "set sensor id");

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if(vm.count("help") || !vm.count("mq") || !vm.count("id")) {
        std::cout << desc << std::endl;
        return -1;
    }

    Sensor sensor(size, scaleFactor, mq, id);
    sensor.start();

    sleep(20);

    sensor.end();
    return 0;
}