#ifndef ASSIGNMENT3_PIESTIMATORCOMMAND_HPP
#define ASSIGNMENT3_PIESTIMATORCOMMAND_HPP

#include <Command.hpp>

namespace Assignment3
{
    class PiEstimatorCommand : public Command
    {
    public:
        PiEstimatorCommand(double, double);
        virtual ~PiEstimatorCommand() = default;
        
    public: // Command
        double _operation() override;

    private: 
        double _pointsInCircle;
        double _totalPoints;
        int getId() { return 4; };
    };
}
#endif