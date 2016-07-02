#include "SearchAlgorithm.h"

namespace jss
{

class StochasticHillClimber : public SearchAlgorithm
{
public:
    using SearchAlgorithm::SearchAlgorithm;
    virtual std::shared_ptr<Schedule> findSolution(double time_limit) const override;

private:
    double T = 20.0; /// Temperature parameter
};
}
