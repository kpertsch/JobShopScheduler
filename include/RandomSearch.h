#pragma once

#include "SearchAlgorithm.h"

namespace jss
{

class RandomSearch : public SearchAlgorithm
{

public:
    RandomSearch(const std::string& file_name, unsigned seed);

protected:
    std::shared_ptr<Schedule> findSolutionSerial(double time_limit) const override;
};
}
