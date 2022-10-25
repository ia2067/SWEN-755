#include <PurePriorityScheduler.hpp>

#include <iostream>

namespace Assignment3
{

PurePriorityScheduler::PurePriorityScheduler()
{
}

std::shared_ptr<Command> PurePriorityScheduler::getNext() 
{
    if (_highPriorityQueue.size() > 0)
    {
        std::cout << "DISPATCHING HIGH PRIORITY TASK. REMAINING: " << _highPriorityQueue.size()-1 << std::endl;
        return getHighPriority();
    }
    else if (_lowPriorityQueue.size() > 0)
    {
        return getLowPriority();
    }
    else
    {
        // TODO: Exception?
        return nullptr;
    }
}

} // namespace Assignment3

