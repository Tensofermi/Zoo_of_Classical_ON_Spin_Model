#pragma once
#include <bits/stdc++.h>

#include "Text_Formalization.hpp"
#include "Vector_operation.hpp"
#include "Union_Find.hpp"

//===========================
//========= MISC ============
//===========================

inline int pbc_mod(int _a, int _L)
{
    if (_a >= 0)
    {
        return _a % _L;
    }
    else if (_a >= -_L)
    {
        _a += _L;
        return _a;
    }
    else
    {
        _a += _L;
        return pbc_mod(_a, _L);
    }
}


// 将角度规范化到 [-π, π) 范围
inline double normalizeAngle(double angle) {
    // 如果角度大于或等于 π，则减去 2π
    while (angle >= M_PI) angle -= 2 * M_PI;
    // 如果角度小于 -π，则加上 2π
    while (angle < -M_PI) angle += 2 * M_PI;
    return angle;
}

// 将两个角度相加并规范化到 [-π, π) 范围
inline double addAngle(double A, double B) {
    return normalizeAngle(A + B);
}

// 规范化单个角度到 [-π, π) 范围
inline double modAngle(double A) {
    return normalizeAngle(A);
}

//===========================
//========= MISC ============
//===========================