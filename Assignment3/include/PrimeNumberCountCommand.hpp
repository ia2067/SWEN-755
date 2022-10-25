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
    double _operation() override;

private:
    long _maxNumber;
    int getId() {return 3;};
};

} // namespace Assignment3



#endif