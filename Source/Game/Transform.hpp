#pragma once

#include "Precompiled.hpp"

class Transform : public Serializable
{
public:
    Transform();
    ~Transform();

    void resetInterpolation();
    void move(const sf::Vector2f& offset);
    void rotate(float rotation);
    void scale(float scaling);

    void setPosition(const sf::Vector2f& position, bool ignoreInterpolation = false);
    void setDirection(const sf::Vector2f& direction, bool ignoreInterpolation = false);
    void setRotation(float rotation, bool ignoreInterpolation = false);
    void setScale(float scale, bool ignoreInterpolation = false);

    sf::Vector2f getPosition(float timeAlpha = 1.0f) const;
    sf::Vector2f getDirection(float timeAlpha = 1.0f) const;
    float getRotation(float timeAlpha = 1.0f) const;
    float getScale(float timeAlpha = 1.0f) const;

protected:
    bool onSerialize(MemoryBuffer& buffer) override;
    bool onDeserialize(MemoryBuffer& buffer) override;

public:
    sf::Vector2f m_currentPosition;
    sf::Vector2f m_previousPosition;
    float m_currentRotation;
    float m_previousRotation;
    float m_currentScale;
    float m_previousScale;
};
