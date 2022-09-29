#include <sync/Message.hpp>

namespace Sync
{
    Message::Message()
    : _id("")
    {
        for (int i = 0; i < 16; i++)
        {
            _msgVals[i] = 0;
        }
    }

    Message::Message(std::string id)
    : Message(id, nullptr)
    { }

    // Message::Message(std::string id,
    //                  float* vals[16])
    Message::Message(std::string id,
                     float vals[16])
    : _id(id)
    {
        for (int i = 0; i < 16; i++)
        {
            _msgVals[i] = vals[i];
        }
    }

    std::string Message::getId()
    {
        return _id;
    }

    //float* Message::getVals()
    // float[16] Message::getVals()
    // {
    //     return _msgVals;
    // }
} // namespace Sync