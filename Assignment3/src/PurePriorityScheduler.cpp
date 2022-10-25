#include <PurePriorityScheduler.hpp>

namespace Assignment3
{

PurePriorityScheduler::PurePriorityScheduler()
{
}

std::shared_ptr<Command>  PurePriorityScheduler::getNext() 
{
    if (_highPriorityQueue.size() > 0)
    {
        return getHighPriority();
    }
    else if (_lowPriorityQueue.size() > 0)
    {
        return getLowPriority();
    }
    else
    {
        // TODO: Exception?
        return NULL;
    }
}

} // namespace Assignment3

