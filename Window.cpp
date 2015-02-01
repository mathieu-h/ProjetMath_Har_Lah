//
//  Window.cpp
//  Cours1
//
//  Created by Maxime Lahaye on 17/01/15.
//  Copyright (c) 2015 Maxime Lahaye. All rights reserved.
//
#include "stdafx.h"


#include "Window.h"

Window::Window()
{
    this->name = "Ma fenêtre";
}

Window::Window(const std::vector<Point> &points, std::string name)
{
    this->points = points;
    this->name = name;
}

Window::Window(const Window &window){
    this->points = window.points;
    this->name = window.name;
}

Window::~Window(void)
{
    
}

std::string Window::get_name() const
{
    return this->name;
}

std::vector<Point> Window::get_points() const
{
    return this->points;
}

void Window::set_name(std::string name)
{
    this->name = name;
}

void Window::set_points(std::vector<Point> points)
{
    this->points = points;
}

void Window::add_point(Point &p)
{
    this->points.push_back(p);
}

std::ostream& operator<<(std::ostream& out, const Window &w)
{
    for (std::size_t i = 0; i < w.get_points().size(); ++i) {
        out << w.get_points()[i] << " ";
    }
    
    return out;
}

