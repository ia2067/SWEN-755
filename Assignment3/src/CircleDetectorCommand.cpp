#include <CircleDetectorCommand.hpp>
#include <cmath>
#include <random>
#include <iostream>

namespace Assignment3
{

CircleDetectorCommand::CircleDetectorCommand()
{
}

double CircleDetectorCommand::_operation() 
{
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<double> dist(-.5, .5);
    //std::cout << dist(mt);

    // Simulate (x,y) coordinate.
    double x = dist(mt);
    double y = dist(mt);

    // Determine if distance from origin is < 1 (i.e. in unit circle) and return 1.
    if (sqrt(x*x + y*y) <= .5)
    {
        return 1;
    }
    else 
    {
        return 0;
    }
}

} // namespace Assignment3

