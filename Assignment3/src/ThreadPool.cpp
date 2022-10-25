#include <ThreadPool.hpp>
#include <ostream>
#include <iostream>

namespace Assignment3
{

ThreadPool::ThreadPool(int num_threads, std::shared_ptr<Scheduler> scheduler)
: _numThreads(num_threads), _scheduler(scheduler),
  _state(INIT)
{ }


void ThreadPool::_run()
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
            nextSleepTime = _state_ready();
            break;
        default:
            break;
        }
        std::this_thread::sleep_for(nextSleepTime);
    } while (!_getShutdown());

    // Stop worker threads.
    int i = 0;
    for (std::shared_ptr<WorkerThread> t : _threads)
    {
        t->end();
    }
    _setState(SHUTDOWN);
}

std::chrono::milliseconds ThreadPool::_state_init()
{
    // Create threads for pool. NOTE: Empty pool prior to creating proper number of threads.
    _threads.clear();
    for (int i=0; i<_numThreads; i++)
    {
        std::shared_ptr<WorkerThread> t = std::make_shared<WorkerThread>();
        _threads.push_back(t);
        t->start();
    }

    _setState(READY);
    return std::chrono::milliseconds(1);
}

std::chrono::milliseconds ThreadPool::_state_ready()
{
    for (std::shared_ptr<WorkerThread> thread : _threads)
    {
        std::shared_ptr<Command> next = _scheduler->getNext();

        // Find finished threads to get result. Will move it to ready.
        if (thread->isDone())
        {
            std::lock_guard<std::mutex> lock(_mutex);
            _finishedCommands.push_back(thread->getFinishedCommand());
        }

        // Any thread that is ready should get the next command.
        if (thread->isReady() && next)
        {
            thread->addCommand(next);
        }
    }

    return std::chrono::milliseconds(1);
}

void ThreadPool::addCommand(std::shared_ptr<Command> cmd, Priority::Priority_e pri)
{
    std::lock_guard<std::mutex> lock(_mutex);
    _scheduler->addCmd(cmd, pri);
}

std::vector<std::shared_ptr<Command>> ThreadPool::getFinishedCommands()
{
    std::lock_guard<std::mutex> lock(_mutex);
    
    // Create a copy of the _finishedCommands to be returned.
    std::vector<std::shared_ptr<Command>> res;
    res.assign(_finishedCommands.begin(), _finishedCommands.end());
    
    // Clear finished commands so that only new commands are in it.
    _finishedCommands.clear();

    return res;
}

ThreadPool::State_e ThreadPool::getState()
{
    return _state;
}

void ThreadPool::_setState(State_e state)
{
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