#include "tools.hpp"
// ====== Function code ======
void printVector(const std::vector<int> data)
{
    for (int n : data)
    {
        std::cout << n << " ";
    }
    std::cout << std::endl;
}
