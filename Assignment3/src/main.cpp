#include <iostream>

#include <ThreadPool.hpp>

#include <FibonacciCommand.hpp>
#include <PrimeNumberCountCommand.hpp>
#include <CircleDetectorCommand.hpp>
#include <PiEstimatorCommand.hpp>

#include <PurePriorityScheduler.hpp>

#include <cmath>

int main(int argc, char const *argv[])
{
    std::cout << "Assignment 3" << std::endl;

    Assignment3::ThreadPool tp(10, std::make_shared<Assignment3::PurePriorityScheduler>());
    tp.start();

    std::cout << "Waiting for threadpool to start";
    while(tp.getState() != Assignment3::ThreadPool::INIT)
    {
        std::cout << "." << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
    std::cout << " Started!" << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    int i = 0;
    // while(i++ < 5)
    // {
    //     std::cout << "Current State: " << tp.getState() << std::endl;
    //     std::this_thread::sleep_for(std::chrono::milliseconds(500));
    // }

    // std::shared_ptr<Assignment3::FibonacciCommand> pFibCmd = std::make_shared<Assignment3::FibonacciCommand>(10);
    // std::shared_ptr<Assignment3::PrimeNumberCountCommand> pPrimeNumCount = std::make_shared<Assignment3::PrimeNumberCountCommand>(10);
    // tp.addCommand(pFibCmd, Assignment3::Priority::HIGH);
    // tp.addCommand(pPrimeNumCount, Assignment3::Priority::LOW);

    // std::this_thread::sleep_for(std::chrono::milliseconds(100));
    // for (std::shared_ptr<Assignment3::Command> cmd : tp.getFinishedCommands())
    // {
    //     std::cout << cmd->getResult() << std::endl;
    // }
    
    // std::shared_ptr<Assignment3::FibonacciCommand> pFibCmd2 = std::make_shared<Assignment3::FibonacciCommand>(10);
    // std::shared_ptr<Assignment3::PrimeNumberCountCommand> pPrimeNumCount2 = std::make_shared<Assignment3::PrimeNumberCountCommand>(10);
    // tp.addCommand(pFibCmd2, Assignment3::Priority::HIGH);
    // tp.addCommand(pPrimeNumCount2, Assignment3::Priority::LOW);

    // std::this_thread::sleep_for(std::chrono::milliseconds(100));
    // for (std::shared_ptr<Assignment3::Command> cmd : tp.getFinishedCommands())
    // {
    //     std::cout << cmd->getResult() << std::endl;
    // }

    while (i++ < 100000)
    {
        std::shared_ptr<Assignment3::CircleDetectorCommand> pCircDetectCmd = std::make_shared<Assignment3::CircleDetectorCommand>();

        tp.addCommand(pCircDetectCmd, Assignment3::Priority::LOW);
    }

    int j = 0;
    while (j++ < 10)
    {
        std::shared_ptr<Assignment3::FibonacciCommand> pFibCmd2 = std::make_shared<Assignment3::FibonacciCommand>(2*j);
        std::shared_ptr<Assignment3::PrimeNumberCountCommand> pPrimeNumCount2 = std::make_shared<Assignment3::PrimeNumberCountCommand>(2*j);
        tp.addCommand(pFibCmd2, Assignment3::Priority::HIGH);
        tp.addCommand(pPrimeNumCount2, Assignment3::Priority::HIGH);
    }

    //std::cout << tp.getNumberUnfinishedCmd() << std::endl;
    while (tp.getNumberUnfinishedCmd() > 0)
    { 
        std::this_thread::sleep_for(std::chrono::milliseconds(100)); 
    }

    double inCirclePoints = 0;
    double totalPoints = 0;
    for (auto cmd : tp.getFinishedCommands())
    {
        switch(cmd->getId())
        {
        case 1:
            inCirclePoints += cmd->getResult();
            totalPoints++;
            break;
        case 2:
            std::cout << "Fibonacci: " << cmd->getResult() << std::endl;
            break;
        case 3:
            std::cout << "Primes: " << cmd->getResult() << std::endl;
            break;
        default:
            break;
        }
    }
    std::shared_ptr<Assignment3::PiEstimatorCommand> pPiEstCmd = std::make_shared<Assignment3::PiEstimatorCommand>(inCirclePoints, totalPoints);
    tp.addCommand(pPiEstCmd, Assignment3::Priority::LOW);

    while (tp.getNumberUnfinishedCmd() > 0)
    { std::this_thread::sleep_for(std::chrono::milliseconds(100)); }

    std::cout << "Points in Circle: " << inCirclePoints << " Total Points: " << totalPoints << std::endl;
    std::cout << "Pi Estimation: " << pPiEstCmd->getResult() << std::endl;

    tp.end();
    std::cout << "END MAIN." << std::endl;
    return 0;
}
