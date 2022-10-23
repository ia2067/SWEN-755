#ifndef ASSIGNMENT3_PRIMENUMBERCOUNTCOMMAND_HPP
#define ASSIGNMENT3_PRIMENUMBERCOUNTCOMMAND_HPP

#include <Command.hpp>

namespace Assignment3
{
class PrimeNumberCountCommand : public Command
{
public:
    PrimeNumberCountCommand(long);
    virtual ~PrimeNumberCountCommand() = default;

public: // Command
    int _operation() override;

private:
    long _maxNumber;
};

} // namespace Assignment3



#endif