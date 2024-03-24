//
// Created by DELL on 2023/4/21.
//

#ifndef PL0_COMPILER23_UTILS_H
#define PL0_COMPILER23_UTILS_H

#ifdef DEBUG
#include <iostream>
void dbg() { std::cout << "\n"; }
template<typename T, typename... A>
void dbg(T a, A... x) { std::cout << a << ' '; dbg(x...); }
#define logv(x...) std::cout << #x << " -> "; dbg(x);
#define logs(x...) dbg(x);
#else
#define logs(...)
#define logv(...)
#endif

#include <vector>
#include <cassert>
#include <cctype>

int vectorToInt(std::vector<char> &v);

#endif //PL0_COMPILER23_UTILS_H
