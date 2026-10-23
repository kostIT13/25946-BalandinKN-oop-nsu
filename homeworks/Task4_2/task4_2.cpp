#include "task4_2.h"
#include <iostream>
#include <algorithm>
#include <numeric>
#include <map>
#include <functional>
#include <vector>

bool equalsVal(int x, int val) {
    return x == val;
}

void solveTask4_2(const std::vector<int>& vecLarge, const std::vector<int>& vecSmall) {
    std::cout << "             ЗАДАНИЕ 4.II\n";

    // 1. Бинарная операция (умножение) через std::accumulate
    std::cout << "--- Пункт 1: Бинарная операция (умножение) ---\n";
    long long productLarge = std::accumulate(vecLarge.begin(), vecLarge.end(), 1LL,
                                             std::multiplies<long long>());
    long long productSmall = std::accumulate(vecSmall.begin(), vecSmall.end(), 1LL,
                                             std::multiplies<long long>());
    std::cout << "Произведение 1-го вектора: " << productLarge << std::endl;
    std::cout << "Произведение 2-го вектора: " << productSmall << std::endl;

    // 2. Определение дубликатов
    std::cout << "\n--- Пункт 2: Поиск дубликатов ---\n";
    std::map<int, int> freqLarge;
    std::map<int, int> freqSmall;

    for (int i = 0; i < (int)vecLarge.size(); ++i) {
        freqLarge[vecLarge[i]]++;
    }
    for (int i = 0; i < (int)vecSmall.size(); ++i) {
        freqSmall[vecSmall[i]]++;
    }

    std::vector<int> dupSmall;
    for (std::map<int, int>::iterator it = freqSmall.begin(); it != freqSmall.end(); ++it) {
        if (it->second >= 2) {
            dupSmall.push_back(it->first);
        }
    }

    std::vector<int> dupLarge;
    for (std::map<int, int>::iterator it = freqLarge.begin(); it != freqLarge.end(); ++it) {
        if (it->second > 3) {
            dupLarge.push_back(it->first);
        }
    }

    std::cout << "Числа во 2-м векторе (>=2 раз): ";
    for (int i = 0; i < (int)dupSmall.size(); ++i) {
        std::cout << dupSmall[i] << " ";
    }
    std::cout << std::endl;

    std::cout << "Числа в 1-м векторе (>3 раз): ";
    for (int i = 0; i < (int)dupLarge.size(); ++i) {
        std::cout << dupLarge[i] << " ";
    }
    std::cout << std::endl;

    // 3. Сравнение дубликатов
    std::cout << "\n--- Пункт 3: Сравнение дубликатов ---\n";

    // Способ A: через цикл for
    std::cout << "Способ A (цикл for):\n";
    for (int i = 0; i < (int)dupLarge.size(); ++i) {
        int val = dupLarge[i];
        int countInSmall = 0;
        for (int j = 0; j < (int)vecSmall.size(); ++j) {
            if (vecSmall[j] == val) {
                countInSmall++;
            }
        }
        std::cout << "  Число " << val
                  << ": в 1-м векторе " << freqLarge[val]
                  << " раз, во 2-м векторе " << countInSmall << " раз.\n";
    }

    // Способ B: через <algorithm> + lambda
    std::cout << "Способ B (<algorithm> + lambda):\n";
    for (int i = 0; i < (int)dupLarge.size(); ++i) {
        int val = dupLarge[i];
        // int countInSmall = (int)std::count_if(vecSmall.begin(), vecSmall.end(),
        //                                       [val](int x) { return x == val; });

        int countInSmall = (int)std::count_if(vecSmall.begin(), vecSmall.end(), std::bind(equalsVal, std::placeholders::_1, val));
        std::cout << "  Число " << val
                  << ": в 1-м векторе " << freqLarge[val]
                  << " раз, во 2-м векторе " << countInSmall << " раз.\n";
    }
}