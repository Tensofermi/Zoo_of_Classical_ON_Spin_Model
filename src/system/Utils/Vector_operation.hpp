#pragma once
#include <bits/stdc++.h>

//--- Vector Operation
inline double dot(const std::vector<double>& vector1, const std::vector<double>& vector2) 
{
    double result = 0.0;
    for (size_t i = 0; i < vector1.size(); i++)
        result += vector1[i] * vector2[i];

    return result;
}

inline double operator*(const std::vector<double>& vector1, const std::vector<double>& vector2) 
{
    double result = 0.0;
    for (size_t i = 0; i < vector1.size(); i++)
        result += vector1[i] * vector2[i];

    return result;
}

inline std::vector<double> add(const std::vector<double>& vector1, const std::vector<double>& vector2) 
{
    std::vector<double> result(vector1.size());

    for (size_t i = 0; i < vector1.size(); i++)
        result[i] = vector1[i] + vector2[i];

    return result;
}

inline std::vector<double> operator+(const std::vector<double>& vector1, const std::vector<double>& vector2) 
{
    std::vector<double> result(vector1.size());

    for (size_t i = 0; i < vector1.size(); i++)
        result[i] = vector1[i] + vector2[i];

    return result;
}

inline std::vector<double> subtra(const std::vector<double>& vector1, const std::vector<double>& vector2) 
{
    std::vector<double> result(vector1.size());

    for (size_t i = 0; i < vector1.size(); i++)
        result[i] = vector1[i] - vector2[i];
        
    return result;
}

inline std::vector<double> operator-(const std::vector<double>& vector1, const std::vector<double>& vector2) 
{
    std::vector<double> result(vector1.size());

    for (size_t i = 0; i < vector1.size(); i++)
        result[i] = vector1[i] - vector2[i];
        
    return result;
}

inline double abs(const std::vector<double>& vector) 
{
    double sumOfSquares = 0.0;
    for (size_t i = 0; i < vector.size(); i++)
        sumOfSquares += vector[i] * vector[i];

    return std::sqrt(sumOfSquares);
}

inline std::vector<double> scalarMulti(double scalar, const std::vector<double>& vector) 
{
    std::vector<double> result(vector.size());
    for (size_t i = 0; i < vector.size(); i++)
        result[i] = vector[i] * scalar;

    return result;
}

inline std::vector<double> operator*(double scalar, const std::vector<double>& vector) 
{
    std::vector<double> result(vector.size());
    for (size_t i = 0; i < vector.size(); i++)
        result[i] = vector[i] * scalar;

    return result;
}