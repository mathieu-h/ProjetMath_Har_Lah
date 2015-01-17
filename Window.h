//
//  Window.h
//  Cours1
//
//  Created by Maxime Lahaye on 17/01/15.
//  Copyright (c) 2015 Maxime Lahaye. All rights reserved.
//

#ifndef __Cours1__Window__
#define __Cours1__Window__

#include "Segment.h"
#include <list>

class Window
{
private:
    std::string name;
    std::list<Segment> segments;
    
public:
    
    /* Contructor and Desctructor */
    Window();
    Window(const std::list<Segment> &segments, std::string name);
    Window(const Window &win);
    ~Window(void);
    
    /* Getter and Setter */
    std::string get_name() const;
    std::list<Segment> get_segments() const;
    
    void set_name(std::string name);
    void set_segments(std::list<Segment> segments);

    
    
};

#endif /* defined(__Cours1__Window__) */
