//
//  Window.cpp
//  Cours1
//
//  Created by Maxime Lahaye on 17/01/15.
//  Copyright (c) 2015 Maxime Lahaye. All rights reserved.
//

#include "Window.h"

Window::Window()
{
    this->name = "Ma fenêtre";
}

Window::Window(const std::list<Segment> &segments, std::string name)
{
    this->segments = segments;
    this->name = name;
}

Window::Window(const Window &window){
    this->segments = window.segments;
    this->name = window.name;
}

Window::~Window(void)
{
    
}

std::string Window::get_name() const
{
    return this->name;
}

std::list<Segment> Window::get_segments() const
{
    return this->segments;
}

void Window::set_name(std::string name)
{
    this->name = name;
}

void Window::set_segments(std::list<Segment> segments)
{
    this->segments = segments;
}
