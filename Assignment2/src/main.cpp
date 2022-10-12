

#include <iostream>
#include <csignal>

#include <boost/process.hpp>
#include <boost/interprocess/ipc/message_queue.hpp>

#include <FaultMonitor.hpp>


int main(int argc, char const *argv[])
{
    std::cout << "Assignment 2" << std::endl;

    std::string mqn("myQueue");
    boost::interprocess::message_queue::remove(mqn.c_str());

    std::string syncQueue("syncQueue");
    boost::interprocess::message_queue::remove(syncQueue.c_str());

    std::string primarySensorQueueName("primarySensorQ");
    std::string secondarySensorQueueName("secondarySensorQ");
    boost::interprocess::message_queue::remove(primarySensorQueueName.c_str());
    boost::interprocess::message_queue::remove(secondarySensorQueueName.c_str());

    namespace bp = boost::process;
    bp::child primarySensorProcess("./syncedSensorProcess --size 10 --scaleFactor 2.0 --hbMq myQueue --fhMq primarySensorQ --id primarySensor --syncTx syncQueue --syncRx null");
    std::this_thread::sleep_for(std::chrono::seconds(1));
    bp::child secondarySensorProcess("./syncedSensorProcess --size 10 --scaleFactor 2.0 --hbMq myQueue --fhMq secondarySensorQ --id secondarySensor --syncTx null --syncRx syncQueue");

    Assignment2::FaultMonitor fm(mqn, primarySensorQueueName, secondarySensorQueueName);

    std::cout << "Starting Fault Monitor" << std::endl;
    fm.start();

    while(fm.getState() != Assignment2::FaultMonitor::State_e::SHUTDOWN)
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    std::cout << "Ending Fault Monitor" << std::endl;
    fm.end();

    primarySensorProcess.terminate();
    secondarySensorProcess.terminate();

    boost::interprocess::message_queue::remove(mqn.c_str());
    return 0;
}