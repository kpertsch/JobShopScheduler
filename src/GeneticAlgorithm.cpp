#include "GeneticAlgorithm.h"

#include <algorithm>
#include <cassert>
#include <iostream>

namespace jss
{

using sched_ptr = std::shared_ptr<Schedule>;

enum class CounterType : unsigned
{
    Mutation,
    Recombination
};

GeneticAlgorithm::GeneticAlgorithm(const std::string& file_name, unsigned seed)
    : SearchAlgorithm(file_name, seed, 2)
{
}

std::string GeneticAlgorithm::extraCounterName(unsigned counter_idx) const
{
    switch (counter_idx)
    {
    case static_cast<unsigned>(CounterType::Mutation):
        return "Mutations performed";
    case static_cast<unsigned>(CounterType::Recombination):
        return "Recombinations performed";
    default:
        return "";
    }
}

std::shared_ptr<Schedule> GeneticAlgorithm::findSolutionSerial(double time_limit) const
{
    sched_ptr best_solution = generateRandomSolution();
    bool finished = false;
    while (not finished)
    {
        unsigned last_improving_generation = 0;
        unsigned generation = 0;
        // initialise population with random candidate solutions
        std::vector<sched_ptr> population;
        population.reserve(populationCount);
        for (unsigned i = 0; i < populationCount; ++i)
        {
            population.push_back(generateRandomSolution());
        }
        std::sort(population.begin(), population.end(), [](const sched_ptr& sched1, const sched_ptr& sched2) { return *sched1 < *sched2; });
        sched_ptr best_evolution_solution = population[0];

        while ((generation - last_improving_generation) < 50 and not(finished = isTimeLimitReached(time_limit)))
        {
            std::vector<sched_ptr> offsprings;
            generateOffsprings(population, offsprings);

            // mutate the resulting offspring
            for (sched_ptr offspring : offsprings)
            {
                mutate(offspring);
            }

            // add offspring to the population
            population.insert(population.end(), offsprings.begin(), offsprings.end());

            selectSurvivors(population);
            ++generation;

            sched_ptr best_generation_solution = population[0];
            if (best_generation_solution->execTime() < best_evolution_solution->execTime())
            {
                last_improving_generation = generation;
                best_evolution_solution = best_generation_solution;
                if (best_evolution_solution->execTime() < best_solution->execTime())
                {
                    best_solution = best_evolution_solution;
                }
            }
        }
    }

    return best_solution;
}

void GeneticAlgorithm::mutate(std::shared_ptr<Schedule> individual) const
{
    if (std::generate_canonical<double, std::numeric_limits<double>::digits>(currentRandomEngine()) < mutation_probability)
    {
        increaseCounter(static_cast<unsigned>(CounterType::Mutation));
        unsigned beginIdx, endIdx, newBeginIdx;
        std::uniform_int_distribution<unsigned> randomBeginIdx{ 0, operationCount() - 1 };
        beginIdx = randomBeginIdx(currentRandomEngine());
        std::uniform_int_distribution<unsigned> randomEndIdx{ beginIdx + 1, operationCount() };
        endIdx = randomEndIdx(currentRandomEngine());
        std::uniform_int_distribution<unsigned> randomNewBeginIdx{ 0, operationCount() - endIdx + beginIdx };
        newBeginIdx = randomNewBeginIdx(currentRandomEngine());
        individual->inverseRangeMove(beginIdx, endIdx, newBeginIdx);
    }
}

std::shared_ptr<Schedule> GeneticAlgorithm::recombine(std::shared_ptr<Schedule> parent1, std::shared_ptr<Schedule> parent2) const
{
    std::vector<bool> decisions;
    std::bernoulli_distribution randBool;
    decisions.reserve(operationCount());
    for (unsigned i = 0; i < operationCount(); ++i)
    {
        decisions.push_back(randBool(currentRandomEngine()));
    }
    increaseCounter(static_cast<unsigned>(CounterType::Recombination));
    return Schedule::precedencePreservingCrossover(decisions, *parent1, *parent2);
}

void GeneticAlgorithm::generateOffsprings(
    const std::vector<std::shared_ptr<Schedule> >& population, std::vector<std::shared_ptr<Schedule> >& offsprings) const
{
    offsprings.reserve(offspringCount);
    // parent selection and recombination
    std::vector<sched_ptr> potential_parents;
    potential_parents.reserve(populationCount);

    // roulette wheel selection
    assert(std::is_sorted(population.begin(), population.end(), [](const sched_ptr& sched1, const sched_ptr& sched2) { return *sched1 < *sched2; }));
    // worst individual is still at the end
    unsigned max_time = population.back()->execTime() + 5;

    int fitness_sum = 0;
    for (sched_ptr indivdual : population)
    {
        fitness_sum += indivdual->relativeFitnessToWorst(max_time);
    }
    std::uniform_int_distribution<int> randomFitnessPos{ 0, fitness_sum };
    for (unsigned i = 0; i < population.size(); ++i)
    {
        int fitnessPos = randomFitnessPos(currentRandomEngine());
        for (sched_ptr indivdual : population)
        {
            fitnessPos -= indivdual->relativeFitnessToWorst(max_time);
            if (fitnessPos <= 0)
            {
                potential_parents.push_back(indivdual);
            }
        }
    }

    std::uniform_int_distribution<unsigned> randomParent(0, population.size() - 1);
    for (unsigned i = 0; i < offspringCount; ++i)
    {
        sched_ptr parent1 = potential_parents[randomParent(currentRandomEngine())];
        sched_ptr parent2 = potential_parents[randomParent(currentRandomEngine())];
        if (parent1 == parent2)
        {
            // same parents don't need to be recombined
            offsprings.push_back(parent1);
        }
        else
        {
            offsprings.push_back(recombine(parent1, parent2));
        }
    }
}

void GeneticAlgorithm::selectSurvivors(std::vector<std::shared_ptr<Schedule> >& population) const
{
    // select as many best individuals as the target population count for the next generation
    std::sort(population.begin(), population.end(), [](const sched_ptr& sched1, const sched_ptr& sched2) { return *sched1 < *sched2; });
    population.resize(populationCount);
}
}
