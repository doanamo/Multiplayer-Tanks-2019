#pragma once

const float Pi = 3.1415926535897932384626433f;

inline float dotProduct(const sf::Vector2f& a, const sf::Vector2f b)
{
    return a.x * b.x + a.y * b.y;
}

inline float crossProduct(const sf::Vector2f& a, const sf::Vector2f b)
{
    return a.x * b.y - a.y * b.x;
}

inline float toRadians(const float& degrees)
{
    return (degrees * Pi) / 180.0f;
}

inline float toDegrees(const float& radians)
{
    return (radians * 180.0f) / Pi;
}

template<typename Type>
Type clamp(const Type& value, const Type& min, const Type& max)
{
    return std::max(min, std::min(max, value));
}

template<typename Type>
Type lerp(const Type& from, const Type& to, float alpha)
{
    return (1.0f - alpha) * from + alpha * to;
}
