#ifndef HALFEDGE_H
#define HALFEDGE_H



class Face;
class Vertex;



class HalfEdge {

public:
	Vertex* origin;
	HalfEdge* next;
	HalfEdge* prev;
	HalfEdge* twin;
	Face* face;
	int index;
	int edgeIndex;


	HalfEdge();
	HalfEdge(int index);
	HalfEdge(Vertex* origin, HalfEdge* next, HalfEdge* prev, HalfEdge* twin, Face* face, int index, int edgeIndex);

	int nextIndex() const;
	int prevIndex() const;
	int twinIndex() const;
	int faceIndex() const;

	bool isBoundaryEdge() const;
};


#endif