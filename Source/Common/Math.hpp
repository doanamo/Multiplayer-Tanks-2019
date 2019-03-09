#pragma once

template<typename Type>
Type Lerp(const Type& from, const Type& to, float alpha)
{
    return (1.0f - alpha) * from + alpha * to;
}
