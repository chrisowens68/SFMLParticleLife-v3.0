#pragma once
#include "SFML/Graphics.hpp"


class Vec2 
{

private:
    

public:
    Vec2();
    ~Vec2();
    Vec2(float x, float y);

    float m_X = 0.0f;
    float m_Y = 0.0f;

    Vec2 operator+(Vec2& v);
    Vec2 operator-(Vec2& v);
    float operator*(const Vec2& rhs) const;
    Vec2 operator*(const float val);
    bool operator==(Vec2& v);
    bool operator!=(Vec2& v);
    Vec2& operator+=(const Vec2& rhs);    
    Vec2& operator-=(const Vec2& rhs);
    Vec2& operator/(const float rhs);
    Vec2& operator/=(const float rhs);



    Vec2 scale(float s);
    //Vec2& scale(float s);

    float dist(Vec2 v);
    float length();
    Vec2 normalize();
    float dot(const Vec2& rhs);
    sf::Vector2f ConverToVector2f();
    Vec2 ConvertToVec2(const sf::Vector2f& rhs);
    float cross(const Vec2& rhs);


};

