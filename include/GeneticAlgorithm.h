#pragma once

#include "SearchAlgorithm.h"

namespace jss
{

class GeneticAlgorithm : public SearchAlgorithm
{

public:
    GeneticAlgorithm(const std::string& file_name, unsigned seed);

protected:
    virtual std::string extraCounterName(unsigned counter_idx) const override;
    std::shared_ptr<Schedule> findSolutionSerial(double time_limit) const override;

private:
    // variation operators
    void mutate(std::shared_ptr<Schedule> individual) const;
    std::shared_ptr<Schedule> recombine(std::shared_ptr<Schedule> parent1, std::shared_ptr<Schedule> parent2) const;

    // selectors
    void generateOffsprings(const std::vector<std::shared_ptr<Schedule> >& population, std::vector<std::shared_ptr<Schedule> >& offsprings) const;
    void selectSurvivors(std::vector<std::shared_ptr<Schedule> >& population) const;

    const unsigned populationCount = 45;
    const unsigned offspringCount = 25;
    const double mutation_probability = 0.13;
};
}
