#ifndef SYNC_MESSAGE_HPP
#define SYNC_MESSAGE_HPP

#include <boost/serialization/binary_object.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

namespace Sync
{
    /**
     * @brief Serializable Sync Message
     */
    class Message
    {
        private: ////serialization
            friend class boost::serialization::access;
            //float * vals[16];
            float vals[16];
            template<class Archive>
            void serialize(Archive & ar, const unsigned int version)
            {
                ar & _id;
                for(int i = 0; i < sizeof(vals); i++)
                    ar & vals[i];
            }
        public:
            Message();
            Message(std::string id);
            Message(std::string id,
                    //float* newVals[16]);
                    float newVals[16]);
            virtual ~Message() = default;
        
        public: 
            std::string getId();
            //float* getVals();
            //float[16] getVals();

        private:
            std::string _id;
            float _msgVals[16];
    };
} // namespace Sync

#endif