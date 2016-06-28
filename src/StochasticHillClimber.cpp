#include "StochasticHillClimber.h"

using namespace jss;

template <class IsFinished>
std::shared_ptr<Schedule> StochasticHillClimber::findSolution(const IsFinished& isFinished) const
{
    auto curr_solution = generateRandomSolution();
    std::shared_ptr<Schedule> bestSchedule = std::make_shared<Schedule>(curr_solution, machine_count(), job_count());
    while (not isFinished())
    {
        auto comp_solution = generateNeighbourSolution(*curr_solution);
        auto comp_schedule = std::make_shared<Schedule>(comp_solution, machine_count(), job_count());

        auto accept_new_prob = 1 / (1 + exp((bestSchedule->exec_time() - comp_schedule->exec_time())) / T);

        if(((double) rand() / (RAND_MAX)) <= accept_new_prob)
        {
            curr_solution = comp_solution;
            bestSchedule = comp_schedule;
        }
    }
    return bestSchedule;
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
