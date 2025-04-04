#pragma once
#include <bits/stdc++.h>

// Transform T to string type
template <class T>
inline std::string toStr(const T& t)
{
    std::string str;
    std::stringstream stream;
    stream << t;
    stream >> str;
    return str;
}

// Transform string to T type
template <class T>
inline T fromStr(const std::string& str)
{
    T t;
    std::stringstream stream;
    stream << str;
    stream >> t;
    return t;
}

// Transform double to string, and the precision we can set
inline std::string dou2str(double number, int precision) 
{
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(precision) << number;
    return oss.str();
}

template <class T>
inline std::string r_jf(T _var, int _width) 
{
    std::ostringstream oss;
    oss << std::right << std::setw(_width) << _var;
    return oss.str();
}

template <class T>
inline std::string l_jf(T _var, int _width) 
{
    std::ostringstream oss;
    oss << std::left << std::setw(_width) << _var;
    return oss.str();
}