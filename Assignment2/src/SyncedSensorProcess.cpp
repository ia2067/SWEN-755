// boost headers
#include <boost/program_options.hpp>
#include <boost/process.hpp>

// std headers
#include <iostream>

#include <SyncedSensor.hpp>

int main(int argc, char const *argv[])
{
    std::string hbMq, fhMq, id, syncRx, syncTx;
    int size;
    float scaleFactor;

    namespace po = boost::program_options;
    po::options_description desc("Sensor Process");
    desc.add_options()
        ("help", "produce help message")
        ("size", po::value<int>(&size), "set sample size")
        ("scaleFactor", po::value<float>(&scaleFactor), "set scale factor")
        ("hbMq", po::value<std::string>(&hbMq), "set heartbeat messaging queue name")
        ("fhMq", po::value<std::string>(&fhMq), "set heartbeat messaging queue name")
        ("id", po::value<std::string>(&id), "set sensor id")
        ("syncTx", po::value<std::string>(&syncTx), "set syncTx queue name (null if a sync receiver)")
        ("syncRx", po::value<std::string>(&syncRx), "set syncRx queue name (null if a sync sender)");

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if(vm.count("help") || !vm.count("hbMq") || !vm.count("fhMq") || !vm.count("id") || 
       !vm.count("syncTx") || !vm.count("syncRx")) 
    {
        std::cout << desc << std::endl;
        return -1;
    }

    if(syncTx != "null")
    {
        std::shared_ptr<Sync::Sender> sender = std::make_shared<Sync::Sender>(syncTx, std::chrono::milliseconds(2000));
        Assignment2::Sensor primarySensor(size, scaleFactor, hbMq, fhMq, id, 
                                          sender);
        

        primarySensor.start();
        while(true) {}

        primarySensor.end();
    }
    else if (syncRx != "null")
    {
        std::shared_ptr<Sync::Receiver> receiver = std::make_shared<Sync::Receiver>(syncRx);
        Assignment2::Sensor secondarySensor(size, scaleFactor, hbMq, fhMq, id,
                                            receiver);
        secondarySensor.start();
        while(true) {}

        secondarySensor.end();
    }
    else
    {
        std::cout << "FATAL: You tried to create a sensor without a receiver or a sender" << std::endl;
        return 1;
    }

    // Assignment2::Sensor sensor(size, scaleFactor, mq, id);
    // sensor.start();

    //while(true) {}

    // sensor.end();
    return 0;
}