#include "Vec2.h"
#include <iostream>
#include <math.h>


Vec2::Vec2()
{
    //std::cout << "Constructor called...." << std::endl;
}

Vec2::Vec2(float x, float y) : m_X(x), m_Y(y)
{
    //std::cout << "2nd Constructor called...." << std::endl;
}

Vec2::~Vec2()
{
    //std::cout << "Destructor called...." << std::endl;
}

Vec2 Vec2::operator+(Vec2& v)
{
    Vec2 res(v.m_X + m_X, v.m_Y + m_Y);
    return res;
}

Vec2 Vec2::operator-(Vec2& rhs)
{
    Vec2 res(m_X - rhs.m_X, m_Y - rhs.m_Y);
    return res;
}

float Vec2::operator*(const Vec2& rhs) const
{   

     return m_X * rhs.m_X + m_Y * rhs.m_Y;
}

Vec2 Vec2::operator*(float s)
{
    Vec2 res(s*m_X, s*m_Y);
    return res;
}

bool Vec2::operator==(Vec2& v)
{
    if (v.m_X == m_X && v.m_Y == m_Y)
        return true;
    else    
    return false;
}

bool Vec2::operator!=(Vec2& v)
{
    if (v.m_X != m_X || v.m_Y != m_Y)
        return true;
    else
        return false;
}

Vec2& Vec2::operator+=(const Vec2& rhs)
{
    m_X += rhs.m_X;
    m_Y += rhs.m_Y;
    return (*this);
}

Vec2& Vec2::operator-=(const Vec2& rhs)
{
    m_X -= rhs.m_X;
    m_Y -= rhs.m_Y;
    return (*this);
}

/*
Vec2 Vec2::operator[](Vec2 arr[], int index)
{
    return arr[index];
}
*/

Vec2 Vec2::operator/(const float rhs) const
{
    Vec2 res(m_X / rhs, m_Y / rhs);

    return res;
}


Vec2& Vec2::operator/=(const float rhs)
{

    m_X /= rhs;
    m_Y /= rhs;

    return (*this);
}


Vec2 Vec2::scale(float s)
{
    m_X *= s;
    m_Y *= s;

    // Vec2 res(s*m_X, s*m_Y);

    Vec2 res(m_X, m_Y);

    return res;
}


/*
* Alternative implementation which allows for chaining
* Cannot overload functions by return type alone so wont work with the basic version
* of the method.
Vec2& Vec2::scale(float s)
{
    m_X *= s;
    m_Y += s;
    return *this;
}
*/

float Vec2::dist(Vec2 v)
{
    float dx = m_X - v.m_X;
    float dy = m_Y - v.m_Y;

    return sqrtf(dx*dx + dy*dy);
}

float Vec2::length()
{
    
    return sqrtf(m_X * m_X + m_Y * m_Y);
}

Vec2 Vec2::normalize()
{
    
    float length, x, y;

    length = sqrtf(m_X * m_X + m_Y * m_Y);

    x = m_X / length;
    y = m_Y / length;

    Vec2 res(x, y);

    return res;

}

float Vec2::dot(const Vec2& rhs)
{
    return m_X * rhs.m_X + m_Y * rhs.m_Y;
}

sf::Vector2f Vec2::ConverToVector2f()
{
    return sf::Vector2f(m_X, m_Y);
}

Vec2 Vec2::ConvertToVec2(const sf::Vector2f& v)
{
    return Vec2(v.x, v.y);
}

float Vec2::cross(const Vec2& rhs)
{
        return (this->m_X * rhs.m_Y - this->m_Y * rhs.m_X);
}
