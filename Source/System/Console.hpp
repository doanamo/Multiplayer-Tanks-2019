#pragma once

#include "Precompiled.hpp"

class Console
{
public:
    Console();
    ~Console();

    bool initialize();
    void toggle();
    void display();

private:
    bool m_display;
};
