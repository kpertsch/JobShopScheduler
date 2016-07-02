#include "IteratedHillClimber.h"

using namespace jss;

template <class IsFinished>
std::shared_ptr<Schedule> IteratedHillClimber::findSolution(const IsFinished& isFinished) const
{
    bool finished = false;
    // initialise with something useful
    std::shared_ptr<Schedule> best_sched = generateRandomSolution();
    while (not finished)
    {
        std::shared_ptr<Schedule> curr_sched = generateRandomSolution();
        while (not finished)
        {
            bool found_better = false;
            auto neighbours = generateNeighbours(*curr_sched);
            for (std::shared_ptr<Schedule> neighbour_sched : *neighbours)
            {

                if (neighbour_sched->exec_time() < curr_sched->exec_time())
                {
                    curr_sched = neighbour_sched;
                    found_better = true;
                }
            }
            if (not found_better or (finished = isFinished()))
                break;
        }
        if (curr_sched->exec_time() < best_sched->exec_time())
        {
            best_sched = curr_sched;
        }
    }
    return best_sched;
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
