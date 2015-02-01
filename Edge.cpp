//#include "stdafx.h"
#include "Edge.h"

typedef std::shared_ptr<Edge> EdgePtr;

Edge::Edge(void)
{
}


Edge::Edge(float yMax, float xMin, float mRev)
{
	this->yMax = yMax;
	this->xMin = xMin;
	this->mRev = mRev;
}

Edge::Edge(float yMax, float xMin, float mRev, EdgePtr next)
{
	Edge(yMax, xMin, mRev);
	this->next = next;
}

Edge::~Edge(void)
{
	this->next.reset();
}

float Edge::getYMax() const
{
	return this->yMax;
}

float Edge::getXMin() const
{
	return this->xMin;
}

float Edge::getMRev() const
{
	return this->mRev;
}

EdgePtr Edge::getNext() const
{
	return this->next;
}

void Edge::setYMax(float yMax)
{
	this->yMax = yMax;
}

void Edge::setXMin(float xMin)
{
	this->xMin = xMin;
}

void Edge::setMRev(float mRev)
{
	this->mRev = mRev;
}

void Edge::setNext(EdgePtr next)
{
	this->next = next;
}