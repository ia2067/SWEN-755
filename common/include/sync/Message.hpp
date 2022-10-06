#ifndef SYNC_MESSAGE_HPP
#define SYNC_MESSAGE_HPP

#include <boost/serialization/binary_object.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/list.hpp>

namespace Sync
{
    /**
     * @brief Serializable Sync Message
     */
    class Message
    {
        private: //serialization
            friend class boost::serialization::access;
            template<class Archive>
            void serialize(Archive & ar, const unsigned int version)
            {
                ar & _msgVals;
            }
        public:
            Message();
            Message(std::list<int> newVals);
            virtual ~Message() = default;

        private:
            //Message() {};
        
        public: 
            std::list<int> getVals();

        private:
            std::list<int> _msgVals;
    };
} // namespace Sync

#endif