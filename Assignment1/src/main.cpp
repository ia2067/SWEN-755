#include <iostream>
#include <common/HeartbeatSender.hpp>
#include <common/HeartbeatMessage.hpp>

int main(int argc, char const *argv[])
{
    std::cout << "Assignment 1" << std::endl;

    std::string mqn("poopy");
    boost::interprocess::message_queue::remove(mqn.c_str());

    Common::HeartbeatSender hs("heartbeatSender", mqn, boost::chrono::milliseconds(3000));

    hs.start();

    size_t bufSize  = 2 * sizeof(Common::HeartbeatMessage);

    auto pMQ = std::make_shared<boost::interprocess::message_queue>(
                        boost::interprocess::open_or_create,
                        mqn.c_str(),
                        100,
                        bufSize);

    char buf[bufSize];
    size_t rcvd_size;
    unsigned int pri;
    auto timeout = boost::posix_time::second_clock::local_time() + boost::posix_time::seconds(1);
    bool gotBeat = false;
    if(pMQ)
    {

        do
        {
            // std::cout <<"Checking pulse..." << std::endl;
            gotBeat = pMQ->timed_receive(buf,bufSize, rcvd_size, pri, timeout);
        } while (!gotBeat);
    }
    else
    {
        std::cout << "no message queue????" << std::endl;
        return -1;
    }

    Common::HeartbeatMessage rcvd_hbm;

    std::istringstream iss(std::string(buf,bufSize));
    boost::archive::text_iarchive ia(iss);

    ia >> rcvd_hbm;

    std::cout << "Got beat from " << rcvd_hbm.getId() << " @ " << rcvd_hbm.getBeatTime() << std::endl;
    
    hs.end();
    return 0;
}
