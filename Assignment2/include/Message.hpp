#ifndef ASSIGNMENT2_MESSAGES_HPP
#define ASSIGNMENT2_MESSAGES_HPP

#include <mutex>

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/list.hpp>

namespace Assignment2
{
    enum MessageType_e
    {
        CMD_GETDATA,
        CMD_WAKEUP,


        RSP_GETDATA,
        RSP_WAKEUP,
        BAD_MESSAGE_TYPE
    };

    class Message
    {
        private: // serialization
            friend class boost::serialization::access;
            template<class Archive>
            void serialize(Archive & ar, const unsigned int version)
            {
                ar & _msgType;
                ar & _sensorData;
            }
        public:
            Message();
            Message(MessageType_e msgType,
                    std::list<int> sensorData);
        public:
            MessageType_e getMessageType();
            std::list<int> getSensorData();

        private:
            std::mutex _mutex;
            MessageType_e _msgType;
            std::list<int> _sensorData;
    }

} //namespace Assignment2

#endif //ASSIGNMENT2_MESSAGES_HPP
