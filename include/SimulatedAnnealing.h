#pragma once

#include "SearchAlgorithm.h"

namespace jss
{

class SimulatedAnnealing : public SearchAlgorithm
{
public:
    static const std::string longName;
    static const std::string shortName;

    SimulatedAnnealing(const std::string& file_name, unsigned seed);

protected:
    virtual std::shared_ptr<Schedule> findSolutionSerial(double time_limit) const override;

private:
    double initialTemperature = 100.0;
    double endTemperature = 1.0;
    double coolDownRate = 0.97;
    unsigned temperatureRounds = 1000;
};
}
