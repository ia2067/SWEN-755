#ifndef ASSIGNMENT3_CIRCLEDETECTORCOMMAND_HPP
#define ASSIGNMENT3_CIRCLEDETECTORCOMMAND_HPP

#include <Command.hpp>

namespace Assignment3
{
class CircleDetectorCommand : public Command
{
public:
    CircleDetectorCommand();
    virtual ~CircleDetectorCommand() = default;

public: // Command
    double _operation() override;
    int getId() {return 1;};

    

};

} // namespace Assignment3



#endif