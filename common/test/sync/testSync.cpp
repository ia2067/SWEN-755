#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE testSync
#include <boost/test/unit_test.hpp>

#include <sync/Sender.hpp>
#include <sync/Receiver.hpp>

namespace utf = boost::unit_test;

BOOST_AUTO_TEST_CASE(sender)
{
    const std::string mqn("testMQ1");
    
    BOOST_WARN(boost::interprocess::message_queue::remove(mqn.c_str()));

    auto pSyncSend = std::make_shared<Sync::Sender>(mqn, std::chrono::milliseconds(2000));
    BOOST_CHECK_MESSAGE(pSyncSend->start(), "Failure to start sync sender(1)");

    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    BOOST_CHECK_MESSAGE(pSyncSend->started(), "Failure to start syn Sender(2)");

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    BOOST_CHECK(pSyncSend->getState() == Sync::Sender::JOINING_MQ);
}

BOOST_AUTO_TEST_CASE(receiver)
{
    const std::string mqn("testMQ2");
    BOOST_WARN(boost::interprocess::message_queue::remove(mqn.c_str()));
    auto pSyncRcvr = std::make_shared<Sync::Receiver>(mqn);

    BOOST_CHECK_MESSAGE(pSyncRcvr->start(), "Failure to start Sync Receiver(1)");

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    BOOST_CHECK_MESSAGE(pSyncRcvr->started(), "Failure to start Sync Receiver(2)");

    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    BOOST_CHECK_MESSAGE(pSyncRcvr->getState() != Sync::Receiver::CREATING_MQ, "Failure to create Message Queue");
}

