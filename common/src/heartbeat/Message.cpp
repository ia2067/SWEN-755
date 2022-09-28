#include <heartbeat/Message.hpp>

namespace Heartbeat
{
//--------------------------------------------------------------------------
Message::Message()
: _id(""),
  _beatTime(std::chrono::milliseconds(0))
{ }
Message::Message(std::string id)
: Message(id, std::chrono::system_clock::now())
{ }
Message::Message(std::string id,
                    std::chrono::system_clock::time_point beatTime)
: _id(id),
  _beatTime(beatTime)
{ }
std::string Message::getId()
{
    return _id;
}
std::chrono::system_clock::time_point Message::getBeatTime()
{
    return _beatTime;
}
} // namespace Common
