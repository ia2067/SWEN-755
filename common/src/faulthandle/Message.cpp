#include <faulthandle/Message.hpp>

namespace FaultHandle
{

    Message::Message()
    : _msgType(BAD_MESSAGE_TYPE),
      _sensorData({}) // Empty initializer list
    { }

    Message::Message(MessageType_e msgType, std::list<int> sensorData)
    : _msgType(msgType),
      _sensorData(sensorData)
    { }

    Message::Message(Message& other)
    : Message::Message(other.getMessageType(), other.getSensorData())
    { }

    Message Message::operator=(Message& other)
    {
      Message ret(other.getMessageType(), other.getSensorData());
      return ret;
    }

    MessageType_e Message::getMessageType()
    {
        std::lock_guard<std::mutex> lock(_mutex);
        return _msgType;
    }

    std::list<int> Message::getSensorData()
    {
        std::lock_guard<std::mutex> lock(_mutex);
        return _sensorData;
    }

} // namespace FaultHandle