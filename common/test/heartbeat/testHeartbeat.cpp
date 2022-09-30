#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE testHeartbeat
#include <boost/test/unit_test.hpp>

#include <heartbeat/Sender.hpp>
#include <heartbeat/Receiver.hpp>

namespace utf = boost::unit_test;

BOOST_AUTO_TEST_CASE(full)
{
    const std::string mqn("testMQ");
    const std::string sender_id("testHBSender");
    BOOST_WARN(boost::interprocess::message_queue::remove(mqn.c_str()));

    auto pHBS = std::make_shared<Heartbeat::Sender>(sender_id, mqn);
    BOOST_CHECK_MESSAGE(pHBS->start(), "Failure to start Heartbeat sender(1)");

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    BOOST_CHECK_MESSAGE(pHBS->started(), "Failure to start Heartbeat sender(2)");

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    BOOST_CHECK(pHBS->getState() == Heartbeat::Sender::JOINING_MQ);

    auto pHBR = std::make_shared<Heartbeat::Receiver>(mqn);

    pHBR->addSenderId(sender_id);
    BOOST_CHECK_MESSAGE(pHBR->start(), "Failure to start Heartbeat Receiver(1)");

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    BOOST_CHECK_MESSAGE(pHBR->started(), "Failure to start Heartbeat Receiver(2)");

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    BOOST_CHECK_MESSAGE(pHBR->getState() != Heartbeat::Receiver::CREATING_MQ, "Failure of Heartbeat receiver to create MQ");
    

    while(pHBS->getState() == Heartbeat::Sender::JOINING_MQ)
    {
       std::this_thread::sleep_for(std::chrono::milliseconds(250));
    }

    BOOST_CHECK_MESSAGE(pHBS->getState() == Heartbeat::Sender::BEATING, "Failure of Heartbeat Sender to start beating");

    std::this_thread::sleep_for(std::chrono::milliseconds(3000));

    BOOST_CHECK_MESSAGE(pHBR->deadIds().count(sender_id) == 0, "Failure to detect alive");

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    BOOST_CHECK_MESSAGE(pHBS->end(), "Failed to end Heartbeat sender");

    std::this_thread::sleep_for(std::chrono::milliseconds(2500));

    BOOST_CHECK_MESSAGE(pHBR->deadIds().count(sender_id) == 0, "Prematurely dead");

    std::this_thread::sleep_for(std::chrono::milliseconds(2500));

    BOOST_CHECK_MESSAGE(pHBR->deadIds().count(sender_id) != 0, "failed to detect dead");

}