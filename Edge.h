#pragma once
#include <memory>
#include <stdio.h>
#include <iostream>
class Edge
{

public:

	Edge(void);
	Edge(float yMax, float yMin, float xMin, float mRev);
	~Edge(void);

	float getYMax() const;
    float getYMin() const;
	float getXMin() const;
	float getMRev() const;
    
    bool isEmpty() const;

	void setYMax(float yMax);
    void setYMin(float xMin);
	void setXMin(float xMin);
	void setMRev(float mRev);

private:
	float yMax;
    float yMin;
	float xMin;
	float mRev;

};

std::ostream& operator<<(std::ostream& out, const Edge &e);
