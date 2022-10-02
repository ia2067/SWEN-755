#include <sync/Message.hpp>

namespace Sync
{
    Message::Message()
    : _id(""),
      _msgVals(0)
    { }

    Message::Message(std::string id,
                     std::list<int> newVals)
    : _id(id),
      _msgVals(newVals)
    { }

    std::string Message::getId()
    {
        return _id;
    }

    std::list<int> Message::getVals()
    {
        return _msgVals;
    }
} // namespace Sync