#include "SearchAlgorithm.h"

namespace jss
{

class StochasticHillClimber : public SearchAlgorithm
{
public:
    static const std::string longName;
    static const std::string shortName;

    StochasticHillClimber(const std::string& file_name, unsigned seed);

protected:
    virtual std::shared_ptr<Schedule> findSolutionSerial(double time_limit) const override;

private:
    double T = 20.0; /// Temperature parameter
};
}
