//
//  Window.h
//  Cours1
//
//  Created by Maxime Lahaye on 17/01/15.
//  Copyright (c) 2015 Maxime Lahaye. All rights reserved.
//

#ifndef __Cours1__Window__
#define __Cours1__Window__

#include "Point.h"
#include <vector>

class Window
{
private:
    std::string name;
    std::vector<Point> points;
    
public:
    
    /* Contructor and Desctructor */
    Window();
    Window(const std::vector<Point> &points, std::string name);
    Window(const Window &win);
    ~Window(void);
    
    /* Getter and Setter */
    std::string get_name() const;
    std::vector<Point> get_points() const;
    
    void set_name(std::string name);
    void set_points(std::vector<Point> points);
    
    void add_point(Point &p);
    
    
};

std::ostream& operator<<(std::ostream& out, const Window &w);

#endif /* defined(__Cours1__Window__) */
