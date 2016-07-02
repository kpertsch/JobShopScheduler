#include "StochasticHillClimber.h"

using namespace jss;

template <class IsFinished>
std::shared_ptr<Schedule> StochasticHillClimber::findSolution(const IsFinished& isFinished) const
{
    std::shared_ptr<Schedule> best_sched = generateRandomSolution();
    std::shared_ptr<Schedule> curr_sched = best_sched;
    while (not isFinished())
    {
        auto comp_sched = generateNeighbourSolution(*curr_sched);

        auto accept_new_prob = 1.0 / (1.0 + exp((static_cast<int>(comp_sched->exec_time()) - static_cast<int>(curr_sched->exec_time())) / T));

        if (std::generate_canonical<double, std::numeric_limits<double>::digits>(m_random_engine) <= accept_new_prob)
        {
            curr_sched = comp_sched;
            if (curr_sched->exec_time() < best_sched->exec_time())
            {
                best_sched = curr_sched;
            }
        }
    }
    return best_sched;
}

std::shared_ptr<Schedule> StochasticHillClimber::findSolutionInTime(double time_limit) const
{
    startTimer();
    return findSolution([&]() { return isTimeLimitReached(time_limit); });
}

std::shared_ptr<Schedule> StochasticHillClimber::findSolutionInSteps(unsigned step_limit) const
{
    unsigned steps = 0;
    return findSolution([&]() { return ++steps >= step_limit; });
}
