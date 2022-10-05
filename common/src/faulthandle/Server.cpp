#include <faulthandle/Server.hpp>
#include <faulthandle/Message.hpp>

namespace FaultHandle
{
    Server::Server(std::string mqn)
    : _pMQ(std::make_shared<Core::MessageQueue<Message>>(mqn, true))
    { }

    std::string Server::getMessageQueueName()
    {
        return _pMQ->getMessageQueueName();
    }
    Server::State_e Server::getState()
    {
        std::lock_guard<std::mutex> lock(_mutex);
        return _state;
    }
    void Server::_setState(State_e state)
    {
        std::lock_guard<std::mutex> lock(_mutex);
        _state = state;
    }
    void Server::_run()
    {
        do
        {
            std::chrono::milliseconds nextSleepTime;
            switch (getState())
            {
            case INIT:
                nextSleepTime = _init();
                break;
            case CREATING_MQ:
                nextSleepTime = _createMQ();
                break;
            case LISTENING:
                nextSleepTime = _listen();
                break;
            default:
                break;
            }
            std::this_thread::sleep_for(nextSleepTime);
        } while (!_getShutdown());

        _setState(SHUTDOWN);  
    }
    std::chrono::milliseconds Server::_init()
    {
        _setState(CREATING_MQ);

        return std::chrono::milliseconds(0);
    }
    std::chrono::milliseconds Server::_createMQ()
    {
        if(!_pMQ->connect())
            return std::chrono::milliseconds(250);
            
            
        _setState(LISTENING);
        return std::chrono::milliseconds(100);
    }
    std::chrono::milliseconds Server::_listen()
    {
        Message msg;
        if(_pMQ->timedRecvMessage(msg, std::chrono::milliseconds(250)))
        {
            switch(msg.getMessageType())
            {
                case CMD_GETDATA:
                    _handleGetData();
                    break;
                case CMD_WAKEUP:
                    _handleWakeUp();
                    break;
                
                default:
                    break;
            }
        }
        return std::chrono::milliseconds(100);
    }

    void Server::_handleGetData()
    {
        std::list<int> data = sigGetData().get();
        Message rsp(RSP_GETDATA,
                    data);

        _pMQ->sendMessage(rsp);

    }
    void Server::_handleWakeUp()
    {
        sigWakeUp();
        Message rsp(RSP_WAKEUP,
                    {});
        _pMQ->sendMessage(rsp);
    }
} // namespace FaultHandle
