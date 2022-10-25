#ifndef ASSIGNMENT3_SCHEDULER_HPP
#define ASSIGNMENT3_SCHEDULER_HPP

#include <Command.hpp>
#include <queue>
#include <core/Thread.hpp>

namespace Assignment3
{

namespace Priority
{
// Command priority flag.
enum Priority_e {
    LOW,
    HIGH
};
}// namespace Assignment3::Priority

class Scheduler
{
public:
    Scheduler() {}
    virtual ~Scheduler() = default;

protected: // To be scheduled task lists.
    std::queue<std::shared_ptr<Command>> _lowPriorityQueue;
    std::queue<std::shared_ptr<Command>> _highPriorityQueue;
    
protected: // Helper functions to get next Low/High Priority items.
    std::shared_ptr<Command> getLowPriority() {
        std::shared_ptr<Command> res =_lowPriorityQueue.front();
        _lowPriorityQueue.pop();
        return res;
    }
    std::shared_ptr<Command> getHighPriority() {
        std::shared_ptr<Command> res =_highPriorityQueue.front();
        _highPriorityQueue.pop();
        return res;
    }

public:
    /**
     * @brief Add a LOW/HIGH Priority command to the appropriate queue.
     * 
     * @param cmd Command to be added.
     * @param pri Priority for the command to be added.
     */
    void addCmd(std::shared_ptr<Command> cmd, Priority::Priority_e pri)
    {
        switch (pri)
        {
            case Priority::LOW:
                _lowPriorityQueue.push(cmd);
                break;
            case Priority::HIGH:
                _highPriorityQueue.push(cmd);
                break;
            default:
                // TODO: Exception?
                break;
        }
    }

public:
    /**
     * @brief Get the next command to be executed from queues based on scheduling algorithm.
     * @note TO BE OVERRIDE TO PROVIDE ACTUAL SCHEDULING APPROACH.
     * 
     * @return Command to be executed next.
     */
    virtual std::shared_ptr<Command> getNext() = 0;

    int getNumberLowCommands() {return _lowPriorityQueue.size();}
    int getNumberHighCommands() {return _highPriorityQueue.size();}
    int getNumberCommands() {return getNumberLowCommands() + getNumberHighCommands();}

};
} // namespace Assignment3

#endif