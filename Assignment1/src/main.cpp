

#include <iostream>
#include <ctime>

#include <heartbeat/Receiver.hpp>

#include <boost/process.hpp>
#include <boost/interprocess/ipc/message_queue.hpp>

int main(int argc, char const *argv[])
{
    std::cout << "Assignment 1" << std::endl;

    std::string mqn("myQueue");
    boost::interprocess::message_queue::remove(mqn.c_str());

    namespace bp = boost::process;
    bp::child primarySensorProcess("./sensorProcess --size 10 --scaleFactor 2.0 --mq myQueue --id primarySensor");
    sleep(1);
    bp::child secondarySensorProcess("./sensorProcess --size 10 --scaleFactor 2.0 --mq myQueue --id secondarySensor");

    Heartbeat::Receiver hr(mqn, std::chrono::milliseconds(50), std::chrono::milliseconds(3000));

    hr.start();
    hr.addSenderId("primarySensor");
    hr.addSenderId("secondarySensor");
    
    sleep(60);

    std::chrono::system_clock::time_point current_time = std::chrono::system_clock::now();
    std::time_t tt = std::chrono::system_clock::to_time_t(current_time);
    std::cout << "------------------------------------" << std::endl;
    std::cout << "current time: " << std::ctime(&tt);
    std::cout << "------------------------------------" << std::endl;
    for(auto dead : hr.deadIds())
    {
        std::cout << "        DEAD: " << dead << std::endl;
        std::chrono::system_clock::time_point tp(hr.getLastBeat(dead));
        std::time_t tt = std::chrono::system_clock::to_time_t(tp);
        std::cout << "  last comms: " << std::ctime(&tt);
        std::cout << "------------------------------------" << std::endl;
    }

    hr.end();
    primarySensorProcess.terminate();
    secondarySensorProcess.terminate();

    boost::interprocess::message_queue::remove(mqn.c_str());
    return 0;
}