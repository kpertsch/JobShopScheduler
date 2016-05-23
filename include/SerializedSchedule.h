#include "Operation.h"

#include <vector>

namespace jss
{

class SerializedSchedule
{
public:
    bool addOperation(const Operation& op);
    void swapOperations(unsigned index1, unsigned index2);

    std::vector<Operation>::iterator begin()
    {
        return m_operations.begin();
    }

    std::vector<Operation>::iterator end()
    {
        return m_operations.end();
    }

private:
    std::vector<Operation> m_operations;
};
}