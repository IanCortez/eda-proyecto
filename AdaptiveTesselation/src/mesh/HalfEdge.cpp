#include "HalfEdge.h"
#include "Face.h"
#include "Vertex.h"



HalfEdge::HalfEdge() {
	this->origin = nullptr;
	this->next = nullptr;
	this->prev = nullptr;
	this->twin = nullptr;
	this->face = nullptr;
	this->index = 0;
	this->edgeIndex = 0;
}


HalfEdge::HalfEdge(int index) {
	this->origin = nullptr;
	this->next = nullptr;
	this->prev = nullptr;
	this->twin = nullptr;
	this->face = nullptr;
	this->index = index;
}


HalfEdge::HalfEdge(
	Vertex* origin,
	HalfEdge* next,
	HalfEdge* prev,
	HalfEdge* twin,
	Face* face,
	int index,
	int edgeIndex
) {
	this->origin = origin;
	this->next = next;
	this->prev = prev;
	this->twin = twin;
	this->face = face;
	this->index = index;
	this->edgeIndex = edgeIndex;
}


int HalfEdge::nextIndex() const {
	if (this->next != nullptr) return this->next->index;

	if (this->index < 0) return -1;
	return this->index % 4 == 3 ? this->index - 3 : this->index + 1;
}


int HalfEdge::prevIndex() const {
	if (this->prev != nullptr) return this->prev->index;

	if (this->index < 0) return -1;
	return this->index % 4 == 0 ? this->index + 3 : this->index - 1;
}


int HalfEdge::twinIndex() const {
	if (this->twin == nullptr) return -1;
	return this->twin->index;
}


int HalfEdge::faceIndex() const {
	if (this->face != nullptr) return this->face->index;

	if (this->index < 0) return -1;
	return this->index / 4;
}


bool HalfEdge::isBoundaryEdge() const {
	return this->twin == nullptr;
}
