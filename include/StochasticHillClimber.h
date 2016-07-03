#include "SearchAlgorithm.h"

namespace jss
{

class StochasticHillClimber : public SearchAlgorithm
{
public:
    using SearchAlgorithm::SearchAlgorithm;

protected:
    virtual std::shared_ptr<Schedule> findSolutionSerial(double time_limit) const override;

private:
    double T = 20.0; /// Temperature parameter
};
}
