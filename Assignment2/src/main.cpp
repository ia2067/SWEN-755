

#include <iostream>
#include <ctime>

#include <heartbeat/Receiver.hpp>
#include <faulthandle/Client.hpp>

#include <boost/process.hpp>
#include <boost/interprocess/ipc/message_queue.hpp>


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

    Heartbeat::Receiver hr(mqn, std::chrono::milliseconds(100), std::chrono::milliseconds(3000));
    FaultHandle::Client primarySensorChecker(primarySensorQueueName);
    FaultHandle::Client secondarySensorChecker(secondarySensorQueueName);


    hr.start();
    // hr.addSenderId("primarySensor");
    // hr.addSenderId("secondarySensor");

    bool primaryDied = false;
    bool secondaryDied = false;
    auto fHandlePrimaryDied = [&] (std::string id){
        if (id == "primarySensor")
        {
            if(!primaryDied)
                std::cout << "PRIMARY IS DEAD!" << std::endl;
            primaryDied = true;
            
        }
        if(id == "secondarySensor")
        {
            if(!secondaryDied)
                std::cout << "SECONDARY IS DEAD!" << std::endl;
            secondaryDied = true;
        }

    };

    std::this_thread::sleep_for(std::chrono::seconds(1));
    bool primaryAwake = false;
    while(!primaryAwake)
    {
        FaultHandle::Message wakeup_primary(FaultHandle::MessageType_e::CMD_WAKEUP,{});
        if(primarySensorChecker.query(wakeup_primary))
        {
            std::cout << "PRIMARY IS AWAKE!" << std::endl;
            primaryAwake = true;
        }
            
        else
            std::cout << "PRIMARY DIDNT WAKE UP AWAKE!" << std::endl;
        
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
    
    std::this_thread::sleep_for(std::chrono::seconds(3));
    hr.sigNewDead.connect(fHandlePrimaryDied);
    std::this_thread::sleep_for(std::chrono::seconds(2));

    while(!primaryDied)
    {
        FaultHandle::Message get_data_cmd(FaultHandle::MessageType_e::CMD_GETDATA,{});
        if(primarySensorChecker.query(get_data_cmd))
        {
            std::cout << "Got Data(primary): ";
            for(auto it : get_data_cmd.getSensorData())
            {
                std::cout << it << ", " << std::flush;
            }
            std::cout << std::endl;
        }
        else
            std::cout << "DIDN'T GET DATA (PRIMARY)" << std::endl;

        std::this_thread::sleep_for(std::chrono::seconds(1));   
    }


    bool secondaryAwake = false;
    while(!secondaryAwake)
    {
        FaultHandle::Message wake_up_secondary(FaultHandle::MessageType_e::CMD_WAKEUP,{});
        if(secondarySensorChecker.query(wake_up_secondary))
        {
            std::cout << "SECONDARY IS AWAKE!" << std::endl;
            secondaryAwake = true;
        }
        else
            std::cout << "SECONDARY DIDNT WAKE UP AWAKE!" << std::endl;
    }
    
    std::this_thread::sleep_for(std::chrono::seconds(2));

    while(!secondaryDied)
    {
        FaultHandle::Message get_data_cmd(FaultHandle::MessageType_e::CMD_GETDATA,{});
        if(secondarySensorChecker.query(get_data_cmd))
        {
            std::cout << "Got Data(secondary): ";
            for(auto it : get_data_cmd.getSensorData())
            {
                std::cout << it << ", " << std::flush;
            }
            std::cout << std::endl;
        }
        else
            std::cout << "DIDN'T GET DATA (SECONDARY)" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    

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