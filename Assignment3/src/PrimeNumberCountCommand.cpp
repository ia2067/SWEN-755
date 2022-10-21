#include <PrimeNumberCountCommand.hpp>

namespace Assignment3
{

PrimeNumberCountCommand::PrimeNumberCountCommand(long maxNum)
: _maxNumber(maxNum)
{ }
int PrimeNumberCountCommand::_operation() 
{
    int count = 0;
    // crude but meh
    for(int n = 0; n < _maxNumber; n++)
    {
        bool is_prime = true;
        if(n == 0 || n == 1)
        {
            is_prime = false;
        }

        for(int i = 2; i <= n/2; i++)
        {
            if(n % i == 0)
            {
                is_prime = false;
                break;
            }
        }
        
        if(is_prime)
            count++;
    }
    return count;
}

} // namespace Assignment3

