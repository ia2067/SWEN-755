#include <ThreadPool.hpp>
#include <ostream>
namespace Assignment3
{

ThreadPool::ThreadPool(int num_threads)
: _numThreads(num_threads),
  _state(INIT)
{ }


void ThreadPool::_run()
{
    do
    {
        std::chrono::milliseconds nextSleepTime;
        switch(getState())
        {
        case INIT:
            nextSleepTime = _state_init();
            break;
        case READY:
            nextSleepTime = _state_ready();
            break;
        default:
            break;
        }
        std::this_thread::sleep_for(nextSleepTime);
    } while (!_getShutdown());

    _setState(SHUTDOWN);
}

std::chrono::milliseconds ThreadPool::_state_init()
{
    // Create threads for pool. NOTE: Empty pool prior to creating proper number of threads.
    _threads.clear();
    for (int i=0; i<_numThreads; i++)
    {
        _threads.push_back(std::make_shared<WorkerThread>());
    }

    _setState(READY);
    return std::chrono::milliseconds(100);
}

std::chrono::milliseconds ThreadPool::_state_ready()
{
    // TODO: Assign Commands to threads.
    return std::chrono::milliseconds(100);
}

void ThreadPool::addCommand(std::shared_ptr<Command> cmd, Priority_e pri)
{
    std::lock_guard<std::mutex> lock(_mutex);
    switch (pri)
    {
        case LOW:
            _lowPriorityQueue.push(cmd);
            break;
        case HIGH:
            _highPriorityQueue.push(cmd);
            break;
        default:
            break;
    }
}

ThreadPool::State_e ThreadPool::getState()
{
    std::lock_guard<std::mutex> lock(_mutex);
    return _state;
}

void ThreadPool::_setState(State_e state)
{
    std::lock_guard<std::mutex> lock(_mutex);
    _state = state;
}

std::ostream& operator<<(std::ostream& os, const Assignment3::ThreadPool::State_e& state)
{
    switch (state)
    {
    case Assignment3::ThreadPool::INIT:
        os << "INIT";
        break;
    case Assignment3::ThreadPool::READY:
        os << "READY";
        break;
    case Assignment3::ThreadPool::SHUTDOWN:
        os << "SHUTDOWN";
        break;
    default:
        os << "UNKNOWN";
        break;
    }

    return os;
}


} // namespace Assignment3