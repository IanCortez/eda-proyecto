#ifndef FACE_H
#define FACE_H


#include <vector>


class HalfEdge;


class Face {

public:
	HalfEdge* side;
	int valence;
	int index;
	std::vector<float> normal;

	Face();
	Face(HalfEdge* side, int valence, int index);

	void computeNormal();
};


#endif