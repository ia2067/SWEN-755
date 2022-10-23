#include <sync/Message.hpp>

namespace Sync
{
    Message::Message()
    : _msgVals(0)
    { }

    Message::Message(std::list<int> newVals)
    : _msgVals(newVals)
    { }

    std::list<int> Message::getVals()
    {
        return _msgVals;
    }
} // namespace Sync