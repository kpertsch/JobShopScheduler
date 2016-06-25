#include "IteratedHillClimber.h"

using namespace jss;

template <class IsFinished>
std::shared_ptr<Schedule> IteratedHillClimber::findSolution(const IsFinished& isFinished) const
{
    std::shared_ptr<Schedule> bestSchedule = std::make_shared<Schedule>(generateRandomSolution(), machine_count(), job_count());
    while (not isFinished())
    {
        std::shared_ptr<SerializedSchedule> currSolution = generateRandomSolution();
        std::shared_ptr<Schedule> currSchedule = std::make_shared<Schedule>(currSolution, machine_count(), job_count());
        while (true)
        {
            auto solutionNeighbours = generateNeighbours(*currSolution);
            bool foundBetter = false;
            for (std::shared_ptr<SerializedSchedule> neighbour : *solutionNeighbours)
            {
                std::shared_ptr<Schedule> neighbourSchedule = std::make_shared<Schedule>(neighbour, machine_count(), job_count());
                if (neighbourSchedule->exec_time() < currSchedule->exec_time())
                {
                    foundBetter = true;
                    currSchedule = neighbourSchedule;
                    currSolution = neighbour;
                }
            }
            if (not foundBetter)
                break;
        }
        if (currSchedule->exec_time() < bestSchedule->exec_time())
        {
            bestSchedule = currSchedule;
        }
    }
    return bestSchedule;
}

std::shared_ptr<Schedule> IteratedHillClimber::findSolutionInTime(double time_limit) const
{
    startTimer();
    return findSolution([&]() { return isTimeLimitReached(time_limit); });
}

std::shared_ptr<Schedule> IteratedHillClimber::findSolutionInSteps(unsigned step_limit) const
{
    unsigned steps = 0;
    return findSolution([&]() { return ++steps >= step_limit; });
}
