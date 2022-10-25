#include <PiEstimatorCommand.hpp>

namespace Assignment3
{
    PiEstimatorCommand::PiEstimatorCommand(double pointsInCircle, double totalPoints)
    : _pointsInCircle(pointsInCircle),
      _totalPoints(totalPoints)
    { }

    double PiEstimatorCommand::_operation()
    {
        return 4 * (_pointsInCircle / _totalPoints);
    }
}