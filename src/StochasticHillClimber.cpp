#include "StochasticHillClimber.h"

using namespace jss;

std::shared_ptr<Schedule> StochasticHillClimber::findSolution(double time_limit) const
{
    startTimer();

    std::shared_ptr<Schedule> best_sched = generateRandomSolution();
    std::shared_ptr<Schedule> curr_sched = best_sched;
    while (not isTimeLimitReached(time_limit))
    {
        auto comp_sched = generateNeighbourSolution(*curr_sched);
        if (comp_sched->exec_time() < best_sched->exec_time())
        {
            best_sched = comp_sched;
        }

        auto accept_new_prob = 1.0 / (1.0 + exp((static_cast<int>(comp_sched->exec_time()) - static_cast<int>(curr_sched->exec_time())) / T));

        if (std::generate_canonical<double, std::numeric_limits<double>::digits>(current_random_engine()) <= accept_new_prob)
        {
            curr_sched = comp_sched;
        }
    }
    return best_sched;
}
