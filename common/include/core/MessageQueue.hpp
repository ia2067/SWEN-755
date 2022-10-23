#ifndef CORE_MESSAGEQUEUE_HPP
#define CORE_MESSAGEQUEUE_HPP

#include <memory.h>
#include <mutex>
#include <iostream>

#include <boost/interprocess/ipc/message_queue.hpp>
#include <boost/serialization/binary_object.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

namespace bip = boost::interprocess;

namespace Core
{

template<class T>
class MessageQueue
{
public:
    MessageQueue(std::string mqn, bool isCreator=false)
    : _mqn(mqn),
      _isCreator(isCreator),
      _pMQ(nullptr)
      { }
    ~MessageQueue()
    {
        disconnect();
    }

public: // Functional
    bool connect()
    {
        try
        {
            if(_isCreator)
            {
                size_t max_num_msgs = 100;
                size_t bufSize  = 2 * sizeof(T);
                std::lock_guard<std::mutex> lock(_mutex);
                _pMQ = std::make_shared<bip::message_queue>(bip::create_only,
                                                            _mqn.c_str(),
                                                            max_num_msgs,
                                                            bufSize);
            }
            else
            {
                std::lock_guard<std::mutex> lock(_mutex);
                _pMQ = std::make_shared<bip::message_queue>(bip::open_only,
                                                            _mqn.c_str());
            }

        }
        catch(const std::exception& e)
        {
            // std::cout << "error " << (_isCreator ? "creating" : "joining") << " queue: " << e.what() << std::endl;
            return false;
        }
        // std::cout << (_isCreator ? "created" : "joined") << std::endl;
        return bool(_pMQ);

    }
    bool isConnected()
    {
        std::lock_guard<std::mutex> lock(_mutex);
        return bool(_pMQ);
    }
    bool disconnect()
    {
        std::lock_guard<std::mutex> lock(_mutex);
        _pMQ.reset();
        return true;
    }
    bool sendMessage(T& tMsg)
    {
        std::lock_guard<std::mutex> lock(_mutex);
        std::ostringstream oss;
        boost::archive::text_oarchive oa(oss);

        oa << tMsg;

        std::string serialized_msg(oss.str());
        char msg[serialized_msg.length()];
        serialized_msg.copy(msg, serialized_msg.length());
        auto timeout = _makeTimeout(std::chrono::milliseconds(1000));
        // std::cout << "buh buh" << std::endl;
        
        if(!_pMQ->timed_send(msg, serialized_msg.length(), 1, timeout))
        {
            return false;
        }
        return true;
}
    bool recvMessage(T& msg, bool& areMoreMsgs)
    {
        std::lock_guard<std::mutex> lock(_mutex);
        size_t bufSize = 2 * sizeof(T); 
        char buf[bufSize];
        size_t rcvdSize;
        uint pri;

        areMoreMsgs = true;
        if(_pMQ->try_receive(buf, bufSize, rcvdSize, pri))
        {
            std::istringstream iss(std::string(buf,bufSize));
            boost::archive::text_iarchive ia(iss);

            ia >> msg;
        }
        else
        {
            areMoreMsgs = false;
        }

        return true;
    }

    bool timedRecvMessage(T& msg, std::chrono::milliseconds timeout)
    {
        std::lock_guard<std::mutex> lock(_mutex);
        size_t bufSize = 2 * sizeof(T); 
        char buf[bufSize];
        size_t rcvdSize;
        uint pri;

        auto timeout_date = _makeTimeout(timeout);
        if(_pMQ->timed_receive(buf, bufSize, rcvdSize, pri,timeout_date))
        {
            std::istringstream iss(std::string(buf,bufSize));
            boost::archive::text_iarchive ia(iss);

            ia >> msg;
        }
        else
        {
            return false;
        }

        return true;
    }

public: //gets/
    std::string getMessageQueueName()
    {
        std::lock_guard<std::mutex> lock(_mutex);
        return _mqn;
    }

private:
    boost::posix_time::ptime _makeTimeout(std::chrono::milliseconds timeout)
    {
        using clock = std::chrono::system_clock;
        clock::time_point nowp = clock::now();
        clock::time_point end = nowp + timeout;
        auto intermediate = std::chrono::system_clock::to_time_t(end);
        return boost::posix_time::from_time_t(intermediate);
    }

private:
    std::mutex _mutex;
    std::string _mqn;
    bool _isCreator;
    std::shared_ptr<boost::interprocess::message_queue> _pMQ;
};

} // namespace Core


#endif //CORE_MESSAGEQUEUE_HPP