#include <FibonacciCommand.hpp>

namespace Assignment3
{
FibonacciCommand::FibonacciCommand(int numIterations)
: _numIterations(numIterations)
{ }

int FibonacciCommand::_operation()
{
    int lastSum = 0;
    int sum = 1;

    for(int i = 0; i < _numIterations; i++)
    {
        int temp = sum + lastSum;
        lastSum = sum;
        sum = temp;
    }
    return sum;
}
} // namespace Assignment3
