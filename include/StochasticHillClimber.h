#include "SearchAlgorithm.h"

namespace jss
{

class StochasticHillClimber : public SearchAlgorithm
{
public:
    using SearchAlgorithm::SearchAlgorithm;
    virtual std::shared_ptr<Schedule> findSolutionInTime(double time_limit) const override;
    virtual std::shared_ptr<Schedule> findSolutionInSteps(unsigned step_limit) const override;

private:
    template <class IsFinished>
    std::shared_ptr<Schedule> findSolution(const IsFinished& isFinished) const;

    double T = 20.0; /// Temperature parameter
};
}
