#include "stdafx.h"
#include "Edge.h"

typedef Edge* EdgePtr;

Edge::Edge(void)
{
    this->yMin = 0;
    this->yMax = 0;
    this->xMin = 0;
    this->mRev = 0;
}


Edge::Edge(float yMax, float yMin, float xMin, float mRev)
{
	this->yMax = yMax;
    this->yMin = yMin;
	this->xMin = xMin;
	this->mRev = mRev;
}

Edge::~Edge(void)
{
}

std::ostream& operator<<(std::ostream& out, const Edge &e)
{
    if(e.isEmpty())
        out << "empty";
    else
    {
        out << "[ yMax : " << e.getYMax() << " yMin : " << e.getYMin() << " xMin : " << e.getXMin() << " 1/slope : " << 1.0f / e.getMRev() << "] , ";

    }
    return out;
}

bool Edge::isEmpty() const
{
    return (this->yMax == 0.0f && this->yMin == 0.0f && this->xMin == 0.0f && this->mRev == 0.0f);
}

float Edge::getYMax() const
{
	return this->yMax;
}

float Edge::getYMin() const
{
    return this->yMin;
}

float Edge::getXMin() const
{
	return this->xMin;
}

float Edge::getMRev() const
{
	return this->mRev;
}

void Edge::setYMax(float yMax)
{
	this->yMax = yMax;
}

void Edge::setYMin(float yMin)
{
    this->yMin = yMin;
}

void Edge::setXMin(float xMin)
{
	this->xMin = xMin;
}

void Edge::setMRev(float mRev)
{
	this->mRev = mRev;
}
