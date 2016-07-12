#include "StochasticHillClimber.h"

using namespace jss;

StochasticHillClimber::StochasticHillClimber(const std::string& file_name, unsigned seed)
    : SearchAlgorithm(file_name, seed, 0)
{
}

std::string StochasticHillClimber::extraCounterName(unsigned counter_idx) const
{
    (void)counter_idx;
    return "";
}

std::shared_ptr<Schedule> StochasticHillClimber::findSolutionSerial(double time_limit) const
{
    std::shared_ptr<Schedule> best_sched = generateRandomSolution();
    std::shared_ptr<Schedule> curr_sched = best_sched;
    while (not isTimeLimitReached(time_limit))
    {
        auto comp_sched = generateNeighbourSolution(*curr_sched);
        if (comp_sched->execTime() < best_sched->execTime())
        {
            best_sched = comp_sched;
        }

        auto accept_new_prob = 1.0 / (1.0 + exp((static_cast<int>(comp_sched->execTime()) - static_cast<int>(curr_sched->execTime())) / T));

        if (std::generate_canonical<double, std::numeric_limits<double>::digits>(currentRandomEngine()) < accept_new_prob)
        {
            curr_sched = comp_sched;
        }
    }
    return best_sched;
}
