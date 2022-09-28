#ifndef COMMON_THREAD_HPP
#define COMMON_THREAD_HPP

#include <boost/thread.hpp>
#include <mutex>

namespace Common
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
        std::shared_ptr<boost::thread> _pThread;
        bool _shutdown;
    };    
} // namespace Common


#endif