#include "SearchAlgorithm.h"

namespace jss
{

class IteratedHillClimber : public SearchAlgorithm
{
public:
    using SearchAlgorithm::SearchAlgorithm;
    virtual std::shared_ptr<Schedule> findSolution(double time_limit) const override;
};
}
