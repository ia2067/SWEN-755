#ifndef ASSIGNMENT3_PUREPRIORITYSCHEDULER_HPP
#define ASSIGNMENT3_PUREPRIORITYSCHEDULER_HPP

#include <Scheduler.hpp>

namespace Assignment3
{
class PurePriorityScheduler : public Scheduler
{
public:
    PurePriorityScheduler();
    virtual ~PurePriorityScheduler() = default;

public: // Command
    std::shared_ptr<Command> getNext() override;

};

} // namespace Assignment3

#endif