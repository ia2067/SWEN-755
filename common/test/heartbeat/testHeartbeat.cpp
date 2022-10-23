#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE testHeartbeat

#include <atomic>

#include <boost/test/unit_test.hpp>
#include <boost/bind/bind.hpp>

#include <heartbeat/Sender.hpp>
#include <heartbeat/Receiver.hpp>

namespace utf = boost::unit_test;


BOOST_AUTO_TEST_CASE(sender)
{
    const std::string mqn("testMQ1");
    const std::string sender_id("testHBSender1");
    BOOST_WARN(boost::interprocess::message_queue::remove(mqn.c_str()));

    auto pHBS = std::make_shared<Heartbeat::Sender>(sender_id, mqn);
    BOOST_CHECK_MESSAGE(pHBS->start(), "Failure to start Heartbeat sender(1)");

    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    BOOST_CHECK_MESSAGE(pHBS->started(), "Failure to start Heartbeat sender(2)");

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    BOOST_CHECK(pHBS->getState() == Heartbeat::Sender::JOINING_MQ);
}

BOOST_AUTO_TEST_CASE(receiver)
{
    const std::string mqn("testMQ2");
    const std::string sender_id("testHBSender2");
    BOOST_WARN(boost::interprocess::message_queue::remove(mqn.c_str()));
    auto pHBR = std::make_shared<Heartbeat::Receiver>(mqn);

    pHBR->addSenderId(sender_id);
    BOOST_CHECK_MESSAGE(pHBR->start(), "Failure to start Heartbeat Receiver(1)");

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    BOOST_CHECK_MESSAGE(pHBR->started(), "Failure to start Heartbeat Receiver(2)");

    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    BOOST_CHECK_MESSAGE(pHBR->getState() != Heartbeat::Receiver::CREATING_MQ, "Failure to create Message Queue");
}


BOOST_AUTO_TEST_CASE(full)
{
    const std::string mqn("testMQ3");
    const std::string sender_id("testHBSender3");
    BOOST_WARN(boost::interprocess::message_queue::remove(mqn.c_str()));

    auto pHBS = std::make_shared<Heartbeat::Sender>(sender_id, 
                                                    mqn, 
                                                    std::chrono::milliseconds(250));

    pHBS->start();

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    auto pHBR = std::make_shared<Heartbeat::Receiver>(mqn, 
                                                      std::chrono::milliseconds(250), 
                                                      std::chrono::milliseconds(500));

    pHBR->addSenderId(sender_id);
    BOOST_CHECK_MESSAGE(pHBR->start(), "Failure to start Heartbeat Receiver(1)");

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    BOOST_CHECK_MESSAGE(pHBR->started(), "Failure to start Heartbeat Receiver(2)");

    while(pHBR->getState() == Heartbeat::Receiver::CREATING_MQ)
    {
       std::this_thread::sleep_for(std::chrono::milliseconds(250));
    }

    while(pHBS->getState() == Heartbeat::Sender::JOINING_MQ)
    {
       std::this_thread::sleep_for(std::chrono::milliseconds(250));
    }

    BOOST_CHECK_MESSAGE(pHBS->getState() == Heartbeat::Sender::BEATING, "Failure of Heartbeat Sender to start beating");

    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    BOOST_CHECK_MESSAGE(pHBR->deadIds().count(sender_id) == 0, "Failure to detect alive");

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    std::atomic<bool> didDie(false);
    
    pHBR->sigNewDead.connect([&](std::string id)
    {
        if(id.compare(sender_id) == 0)
            didDie.store(true);
    });

    BOOST_CHECK_MESSAGE(pHBS->end(), "Failed to end Heartbeat sender");

    std::this_thread::sleep_for(std::chrono::milliseconds(250));

    BOOST_CHECK_MESSAGE(pHBR->deadIds().count(sender_id) == 0, "Prematurely dead");

    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    BOOST_CHECK_MESSAGE(didDie, "Failure to get signal of dead");

    BOOST_CHECK_MESSAGE(pHBR->deadIds().count(sender_id) != 0, "failed to detect dead");
}