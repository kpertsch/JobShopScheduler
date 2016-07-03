#include "SearchAlgorithm.h"

namespace jss
{

class IteratedHillClimber : public SearchAlgorithm
{
public:
    using SearchAlgorithm::SearchAlgorithm;

protected:
    virtual std::shared_ptr<Schedule> findSolutionSerial(double time_limit) const override;
};
}
