#include <iostream>
#include "average.h"
#include "mode_average.h"

int main() {
    std::cout << "ordinary::average<int>(3, 4)         = "
              << ordinary::average<int>(3, 4) << '\n';

    std::cout << "ordinary::average<double>(3.0, 4.0)  = "
              << ordinary::average<double>(3.0, 4.0) << '\n';

    std::cout << "modified::mode_average<int>(100,100)      = "
              << modified::mode_average<int>(100, 100) << '\n';

    std::cout << "modified::mode_average<double>(100.0,100.0) = "
              << modified::mode_average<double>(100.0, 100.0) << '\n';

    return 0;
}