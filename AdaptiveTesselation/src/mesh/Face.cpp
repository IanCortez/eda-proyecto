#include "Face.h"

#include <glm/glm.hpp>

#include "Vertex.h"
#include "HalfEdge.h"



Face::Face() {
	this->side = nullptr;
	this->valence = 0;
	this->index = 0;
	this->normal = std::vector<float>();
}


Face::Face(HalfEdge* side, int valence, int index) {
	this->side = side;
	this->valence = valence;
	this->index = index;
	this->normal = std::vector<float>();
}


void Face::computeNormal() {
	std::vector<float> prev = this->side->prev->origin->coordinates;
	std::vector<float> curr = this->side->origin->coordinates;
	std::vector<float> next = this->side->next->origin->coordinates;

	glm::vec3 prevVec(prev[0], prev[1], prev[2]);
	glm::vec3 currVec(curr[0], curr[1], curr[2]);
	glm::vec3 nextVec(next[0], next[1], next[2]);

	glm::vec3 e1 = prevVec - currVec;
	glm::vec3 e2 = nextVec - currVec;

	glm::vec3 norm = glm::cross(e2, e1);
	norm = (1.0f / norm.length()) * norm;

	std::vector<float> temp = { norm.x, norm.y, norm.z };
	this->normal.clear();
	this->normal = temp;
}
