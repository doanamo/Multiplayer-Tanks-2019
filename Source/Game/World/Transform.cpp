#include "Precompiled.hpp"
#include "Transform.hpp"

static const sf::Vector2f ForwardVector(0.0f, -1.0f);

Transform::Transform() :
    m_previousPosition(0.0f, 0.0f),
    m_currentPosition(0.0f, 0.0f),
    m_previousRotation(0.0f),
    m_currentRotation(0.0f),
    m_previousScale(0.0f),
    m_currentScale(0.0f)
{
}

Transform::~Transform()
{
}

void Transform::resetInterpolation()
{
    // Clamp rotation angle to positive 360 degrees.
    m_currentRotation = std::fmod(m_currentRotation, 360.0f);

    if(m_currentRotation < 0.0f)
    {
        m_currentRotation += 360.0f;
    }

    // Update previous transform.
    m_previousPosition = m_currentPosition;
    m_previousRotation = m_currentRotation;
    m_previousScale = m_currentScale;
}

void Transform::move(const sf::Vector2f& offset)
{
    setPosition(m_currentPosition + offset);
}

void Transform::rotate(float rotation)
{
    setRotation(m_currentRotation + rotation);
}

void Transform::scale(float scaling)
{
    setScale(m_currentScale + scaling);
}

void Transform::setPosition(const sf::Vector2f& position, bool ignoreInterpolation)
{
    m_currentPosition = position;

    if(ignoreInterpolation)
    {
        m_previousPosition = m_currentPosition;
    }
}

void Transform::setDirection(const sf::Vector2f& direction, bool ignoreInterpolation)
{
    setRotation(toDegrees(std::atan2(crossProduct(ForwardVector, direction), dotProduct(ForwardVector, direction))), ignoreInterpolation);
}

void Transform::setRotation(float rotation, bool ignoreInterpolation)
{
    m_currentRotation = rotation;

    if(ignoreInterpolation)
    {
        m_previousRotation = m_currentRotation;
    }
}

void Transform::setScale(float scale, bool ignoreInterpolation)
{
    m_currentScale = scale;

    if(ignoreInterpolation)
    {
        m_previousRotation = m_currentRotation;
    }
}

sf::Vector2f Transform::getPosition(float timeAlpha) const
{
    return lerp(m_previousPosition, m_currentPosition, timeAlpha);
}

sf::Vector2f Transform::getDirection(float timeAlpha) const
{
    float rotation = toRadians(getRotation(timeAlpha));

    sf::Vector2f output;
    output.x = ForwardVector.x * std::cos(rotation) - ForwardVector.y * std::sin(rotation);
    output.y = ForwardVector.x * std::sin(rotation) + ForwardVector.y * std::cos(rotation);
    return output;
}

float Transform::getRotation(float timeAlpha) const
{
    float angle = std::fmod(lerp(m_previousRotation, m_currentRotation, timeAlpha), 360.0f);

    if(angle < 0.0f)
    {
        angle += 360.0f;
    }

    return angle;
}

float Transform::getScale(float timeAlpha) const
{
    return lerp(m_previousScale, m_currentScale, timeAlpha);
}

bool Transform::onSerialize(MemoryStream& buffer) const
{
    if(!serialize(buffer, m_currentPosition))
        return false;

    if(!serialize(buffer, m_currentRotation))
        return false;

    if(!serialize(buffer, m_currentScale))
        return false;

    return true;
}

bool Transform::onDeserialize(MemoryStream& buffer)
{
    if(!deserialize(buffer, &m_currentPosition))
        return false;

    if(!deserialize(buffer, &m_currentRotation))
        return false;

    if(!deserialize(buffer, &m_currentScale))
        return false;

    m_previousPosition = m_currentPosition;
    m_previousRotation = m_previousRotation;
    m_previousScale = m_currentScale;

    return true;
}
