#ifndef ASSIGNMENT3_FIBONACCICOMMAND_HPP
#define ASSIGNMENT3_FIBONACCICOMMAND_HPP

#include <Command.hpp>

namespace Assignment3
{
class FibonacciCommand : public Command
{
public:
    FibonacciCommand(int);
    virtual ~FibonacciCommand() = default;

public: // Command
    double _operation() override;

private:
    int _numIterations;

}; 
} // namespace Assignment3


#endif