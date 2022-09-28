#include <heartbeat/HeartbeatMessage.hpp>

namespace Common
{
//--------------------------------------------------------------------------
HeartbeatMessage::HeartbeatMessage()
: _id(""),
  _beatTime(std::chrono::milliseconds(0))
{ }
HeartbeatMessage::HeartbeatMessage(std::string id)
: HeartbeatMessage(id, std::chrono::system_clock::now())
{ }
HeartbeatMessage::HeartbeatMessage(std::string id,
                    std::chrono::system_clock::time_point beatTime)
: _id(id),
  _beatTime(beatTime)
{ }
std::string HeartbeatMessage::getId()
{
    return _id;
}
std::chrono::system_clock::time_point HeartbeatMessage::getBeatTime()
{
    return _beatTime;
}
} // namespace Common
