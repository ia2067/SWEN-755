#ifndef CORE_THREAD_HPP
#define CORE_THREAD_HPP

#include <memory> //shared_ptr
#include <thread>
#include <mutex>

namespace Core
{
    class Thread
    {
    public:
        Thread();
        ~Thread();

    public:
        bool start();
        bool started();
        bool end();

    protected:
        bool _getShutdown();


    private: // pure virtuals
        virtual void _run() = 0;

    private:
        void _setShutdown(bool);

    private:
        std::mutex _mutex;
        std::shared_ptr<std::thread> _pThread;
        bool _shutdown;
    };    
} // namespace Core


#endif