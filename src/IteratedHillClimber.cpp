#include "IteratedHillClimber.h"

using namespace jss;

std::shared_ptr<Schedule> IteratedHillClimber::findSolutionSerial(double time_limit) const
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
            if (not found_better or (finished = isTimeLimitReached(time_limit)))
                break;
        }
        if (curr_sched->exec_time() < best_sched->exec_time())
        {
            best_sched = curr_sched;
        }
    }
    return best_sched;
}
