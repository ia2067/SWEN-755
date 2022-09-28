#ifndef HEARTBEAT_MESSAGE_HPP
#define HEARTBEAT_MESSAGE_HPP

#include <chrono>
#include <boost/serialization/binary_object.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

namespace Heartbeat
{
    class Message
    {
        private: //serialization
            friend class boost::serialization::access;
            template<class Archive>
            void serialize(Archive & ar, const unsigned int version)
            {
                ar & _id;
                ar & boost::serialization::make_binary_object(&_beatTime, sizeof(_beatTime));
            }
        public:
            Message();
            Message(std::string id);
            Message(std::string id,
                            std::chrono::system_clock::time_point beatTime);
            virtual ~Message() = default;

        public:
            std::string getId();
            std::chrono::system_clock::time_point getBeatTime();

        private:
            std::string _id;
            std::chrono::system_clock::time_point _beatTime;
    };    
} // namespace Heartbeat



#endif