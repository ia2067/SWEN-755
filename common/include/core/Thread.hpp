/**
 * @file Thread.hpp
 * @author Ian Adler (ia2067@rit.edu)
 * @brief Small Wrapper for std::thread
 * @date 2022-09-28
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef CORE_THREAD_HPP
#define CORE_THREAD_HPP

#include <memory> //shared_ptr
#include <thread>
#include <mutex>

namespace Core
{
    /**
     * @brief Small Wrapper for std::thread
     * 
     */
    class Thread
    {
    public:
        Thread();
        ~Thread();

    public:
        /**
         * @brief wrapper to create a std::thread that points to _run
         * 
         * @return true the thread started
         * @return false the thread did not start
         */
        bool start();
        /**
         * @brief Accessor to tell us if a thread is started
         * 
         * @return true it is
         * @return false it aint
         */
        bool started();
        /**
         * @brief method to stop the underlying thread, will block until thread stops
         * 
         * @return true Thread stopped nicely
         * @return false thread did not stop nicely
         */
        bool end();

    protected:
        bool _getShutdown();

    private:
        void _runRunner();
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