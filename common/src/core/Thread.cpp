#include <core/Thread.hpp>
#include <iostream>

namespace Core
{
    Thread::Thread()
    : _pThread(nullptr),
      _shutdown(false)
    { }
    Thread::~Thread()
    {
        //Ensure thread is there
        if (_pThread)
        {
            _pThread->join();
        }
    }

    bool Thread::start()
    {
        _setShutdown(false);

        _pThread = std::make_shared<std::thread>(
            [this] { 
                        _runRunner(); 
                   });

        // probs a better way to check the thread started ok
        return bool(_pThread);
    }
    bool Thread::started()
    {
        return bool(_pThread);
    }
    bool Thread::end()
    {
        _setShutdown(true);

        try
        {
            if(_pThread)
                _pThread->join();
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
            return false;
        }
        
        _pThread = nullptr;
        return true;
    }
    void Thread::_runRunner()
    {
        try
        {
            this->_run();
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        } 
    }

    bool Thread::_getShutdown()
    {
        std::lock_guard<std::mutex> lock(_mutex);
        return _shutdown;
    }
    void Thread::_setShutdown(bool shutdown)
    {
        std::lock_guard<std::mutex> lock(_mutex);
        _shutdown = shutdown;
    }
} // namespace Core
