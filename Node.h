#pragma once
#include <memory>

class Node
{

public:
	typedef std::shared_ptr<Node> NodePtr;

	Node(void);
	Node(float yMax, float xMin, float mRev);
	Node(float yMax, float xMin, float mRev, NodePtr next);
	~Node(void);

	float getYMax() const;
	float getXMin() const;
	float getMRev() const;
	NodePtr getNext() const;

	void setYMax(float yMax);
	void setXMin(float xMin);
	void setMRev(float mRev);
	void setNext(NodePtr next);

private:
	float yMax;
	float xMin;
	float mRev;
	NodePtr next;

	

};

