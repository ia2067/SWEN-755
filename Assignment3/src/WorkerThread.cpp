#include <WorkerThread.hpp>
#include <iostream>

namespace Assignment3
{

// Initialize Next Thread ID for all threads.
int WorkerThread::NEXT_THREAD_ID = 0;

WorkerThread::WorkerThread()
{ 
    // Grab the next thread id as the current thread id and increment it to show it's taken.
    _id = NEXT_THREAD_ID++;
}


void WorkerThread::_run()
{
    do
    {
        std::chrono::milliseconds nextSleepTime = std::chrono::milliseconds(1);
        switch(getState())
        {
        case INIT:
            nextSleepTime = _state_init();
            break;
        case READY:
            // No action to take while waiting for command to come in.
            continue;
        case EXECUTE:
            nextSleepTime = _state_execute();
            break;
        case DONE:
            // No action to take while waiting for result to be querried.
            continue;
        default:
            break;
        }
        std::this_thread::sleep_for(nextSleepTime);
    } while (!_getShutdown());

    _setState(SHUTDOWN);
}

std::chrono::milliseconds WorkerThread::_state_init()
{
    // TODO: Any initialization?
    _setState(READY);
    return std::chrono::milliseconds(1);
}

std::chrono::milliseconds WorkerThread::_state_execute()
{
    try
    {
        std::lock_guard<std::mutex> lock(_mutex);
        _cmd->execute();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
    
    
    _setState(DONE);
    return std::chrono::milliseconds(1);
}

void WorkerThread::addCommand(std::shared_ptr<Command> cmd)
{
    // If Worker is not ready to process commands then they should be rejected.
    if (getState() == READY)
    {
        _setState(EXECUTE);

        std::lock_guard<std::mutex> lock(_mutex);
        _cmd = cmd;
    }
    // TODO: Else exception?
}

std::shared_ptr<Command> WorkerThread::getFinishedCommand()
{
    // Result should only be available if comand has finished. When result is fetched return to waiting for command.
    if (getState() == DONE)
    {
        _setState(READY);
        return _cmd;
    }
    //TODO: Exception?
    return nullptr;
}

bool WorkerThread::isReady()
{
    if (getState() == READY)
    {
        return true;
    }
    else
    {
        return false;
    }
}


bool WorkerThread::isDone()
{
    if (getState() == DONE)
    {
        return true;
    }
    else
    {
        return false;
    }
}

WorkerThread::State_e WorkerThread::getState()
{
    std::lock_guard<std::mutex> lock(_mutex);
    return _state;
}
int WorkerThread::getId()
{
    return _id;
}
void WorkerThread::_setState(State_e state)
{
    std::lock_guard<std::mutex> lock(_mutex);
    _state = state;
}
} // namespace Assignment3