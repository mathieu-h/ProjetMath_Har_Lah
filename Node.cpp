#include "stdafx.h"
#include "Node.h"

typedef std::shared_ptr<Node> NodePtr;

Node::Node(void)
{
}


Node::Node(float yMax, float xMin, float mRev)
{
	this->yMax = yMax;
	this->xMin = xMin;
	this->mRev = mRev;
}

Node::Node(float yMax, float xMin, float mRev, NodePtr next)
{
	Node(yMax, xMin, mRev);
	this->next = next;
}

Node::~Node(void)
{
	this->next.reset();
}

float Node::getYMax() const
{
	return this->yMax;
}

float Node::getXMin() const
{
	return this->xMin;
}

float Node::getMRev() const
{
	return this->mRev;
}

NodePtr Node::getNext() const
{
	return this->next;
}

void Node::setYMax(float yMax)
{
	this->yMax = yMax;
}

void Node::setXMin(float xMin)
{
	this->xMin = xMin;
}

void Node::setMRev(float mRev)
{
	this->mRev = mRev;
}

void Node::setNext(NodePtr next)
{
	this->next = next;
}