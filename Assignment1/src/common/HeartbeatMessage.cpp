#include <common/HeartbeatMessage.hpp>

namespace bc = boost::chrono;
namespace Common
{
//--------------------------------------------------------------------------
HeartbeatMessage::HeartbeatMessage()
: _id(""),
  _beatTime(bc::milliseconds(0))
{ }
HeartbeatMessage::HeartbeatMessage(std::string id)
: HeartbeatMessage(id, bc::system_clock::now())
{ }
HeartbeatMessage::HeartbeatMessage(std::string id,
                    bc::system_clock::time_point beatTime)
: _id(id),
  _beatTime(beatTime)
{ }
std::string HeartbeatMessage::getId()
{
    return _id;
}
bc::system_clock::time_point HeartbeatMessage::getBeatTime()
{
    return _beatTime;
}
} // namespace Common
