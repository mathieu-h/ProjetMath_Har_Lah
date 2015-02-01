#pragma once
#include <memory>

class Edge
{

public:
	typedef std::shared_ptr<Edge> EdgePtr;

	Edge(void);
	Edge(float yMax, float xMin, float mRev);
	Edge(float yMax, float xMin, float mRev, EdgePtr next);
	~Edge(void);

	float getYMax() const;
	float getXMin() const;
	float getMRev() const;
	EdgePtr getNext() const;

	void setYMax(float yMax);
	void setXMin(float xMin);
	void setMRev(float mRev);
	void setNext(EdgePtr next);

private:
	float yMax;
	float xMin;
	float mRev;
	EdgePtr next;

	

};

