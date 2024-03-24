//
// Created by DELL on 2023/4/21.
//

#include "utils.h"

int vectorToInt(std::vector<char> &v)
{
    int num = 0;
    for (char c : v)
    {
        assert(isdigit(c));
        num = num * 10 + (c - '0');
    }
    return num;
}