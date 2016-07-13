#include "SimulatedAnnealing.h"

namespace jss
{
const std::string SimulatedAnnealing::longName = "SimulatedAnnealing";
const std::string SimulatedAnnealing::shortName = "sa";

SimulatedAnnealing::SimulatedAnnealing(const std::string& file_name, unsigned seed)
    : SearchAlgorithm(file_name, seed, 2)
{
}

std::shared_ptr<Schedule> SimulatedAnnealing::findSolutionSerial(double time_limit) const
{
    std::shared_ptr<Schedule> best_sched = generateRandomSolution();
    bool finished = false;
    while (not finished)
    {
        std::shared_ptr<Schedule> curr_sched = generateRandomSolution();
        double T = initialTemperature;
        while (T >= endTemperature and not finished)
        {
            for (unsigned i = 0;
                 i < temperatureRounds and not(finished = isTimeLimitReached(time_limit)); ++i)
            {
                auto comp_sched = generateNeighbourSolution(*curr_sched);
                if (comp_sched->execTime() < best_sched->execTime())
                {
                    best_sched = comp_sched;
                }

                double accept_new_prob
                    = 1.0 / (1.0 + exp((static_cast<int>(comp_sched->execTime())
                                           - static_cast<int>(curr_sched->execTime()))
                                       / T));

                if (comp_sched->execTime() < curr_sched->execTime()
                    or std::generate_canonical<double, std::numeric_limits<double>::digits>(
                           currentRandomEngine())
                        < accept_new_prob)
                {
                    curr_sched = comp_sched;
                }
            }

            T *= coolDownRate;
        }
    }
    return best_sched;
}
}
