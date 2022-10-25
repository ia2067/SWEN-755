#include <CircleDetectorCommand.hpp>
#include <cmath>

namespace Assignment3
{

CircleDetectorCommand::CircleDetectorCommand()
{
}

int CircleDetectorCommand::_operation() 
{
    // Simulate (x,y) coordinate.
    double x = (rand() / RAND_MAX) - 1;
    double y = (rand() / RAND_MAX) - 1;

    // Determine if distance from origin is < 1 (i.e. in unit circle) and return 1.
    if (sqrt(x*x + y*y) <= 1)
    {
        return 1;
    }
    else 
    {
        return 0;
    }
}

} // namespace Assignment3

