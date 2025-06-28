#include "Vertex.h"
#include "HalfEdge.h"



/* --- CONSTRUCTORS --- */
Vertex::Vertex() {
	this->coordinates = std::vector<float>();
	this->valence = 0;
	this->index = 0;
	this->out = nullptr;
}


Vertex::Vertex(std::vector<float> coordinates, HalfEdge* out, int valence, int index) {
	this->coordinates = coordinates;
	this->valence = valence;
	this->index = index;
	this->out = out;
}


 /* --- METHODS --- */
HalfEdge* Vertex::nextBoundaryHalfEdge() const {
	HalfEdge* h = this->out;
	while (!h->isBoundaryEdge()) {
		h = h->twin->next;
	}
	return h;
}


HalfEdge* Vertex::prevBoundaryHaflEdge() const {
	HalfEdge* h = this->out;
	while (!h->isBoundaryEdge()) {
		h = h->twin->next;
	}
	return h;
}


bool Vertex::isBoundaryVertex() const {
	HalfEdge* h = this->out;
	if (h->isBoundaryEdge()) return true;

	HalfEdge* next = h->twin->next;
	while (next != h) {
		if (next->isBoundaryEdge()) return true;
		next = next->twin->next;
	}
	return false;
}


void Vertex::recalculateValence() {
	HalfEdge* edge = this->out->prev->twin;
	int n = 1;
	while (edge != nullptr && edge != out) {
		edge = edge->prev->twin;
		++n;
	}

	edge = this->out->twin;
	while (edge != nullptr && edge->next != out) {
		edge = edge->next->twin;
		++n;
	}
	this->valence = n;
}
