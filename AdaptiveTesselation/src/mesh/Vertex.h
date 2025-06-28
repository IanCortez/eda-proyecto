#ifndef VERTEX_H
#define VERTEX_H


#include <vector>


class HalfEdge;


class Vertex {

public:
	std::vector<float> coordinates;
	HalfEdge* out;
	int valence;
	int index;

	Vertex();
	Vertex(std::vector<float> coords, HalfEdge* out, int valence, int index);

	HalfEdge* nextBoundaryHalfEdge() const;
	HalfEdge* prevBoundaryHaflEdge() const;
	bool isBoundaryVertex() const;
	void recalculateValence();
};



#endif