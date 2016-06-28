#include "SearchAlgorithm.h"

namespace jss
{

class IteratedHillClimber : public SearchAlgorithm
{
public:
    using SearchAlgorithm::SearchAlgorithm;
    virtual std::shared_ptr<Schedule> findSolutionInTime(double time_limit) const override;
    virtual std::shared_ptr<Schedule> findSolutionInSteps(unsigned step_limit) const override;

private:
    template <class IsFinished>
    std::shared_ptr<Schedule> findSolution(const IsFinished& isFinished) const;
};
}