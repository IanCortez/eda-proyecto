#include "Mesh.h"

#include "Face.h"
#include "HalfEdge.h"
#include "Vertex.h"

#include <glm/glm.hpp>



Mesh::~Mesh() {
	this->vertices.clear();
	this->halfEdges.clear();
	this->faces.clear();
}


void Mesh::extractAttributes() {
	this->recalculateNormals();

	this->vertexCoords.clear();
	for (Vertex* vertex: this->vertices) {
		this->vertexCoords.push_back(vertex->coordinates);
	}

	this->polyIndices.clear();
	for (Face* face : this->faces) {
		HalfEdge* hf = face->side;
		for (int i = 0; i < face->valence; ++i) {
			this->polyIndices.push_back(hf->origin->index);
			hf = hf->next;
		}
		this->polyIndices.push_back(INT_MAX);
	}

	this->quadIndices.clear();
	for (Face* face : this->faces) {
		HalfEdge* hf = face->side;
		if (face->valence == 4) {
			for (int i = 0; i < face->valence; ++i) {
				this->quadIndices.push_back(hf->origin->index);
				hf = hf->next;
			}
		}
	}

	this->updateRegularQuadIndices();
}


void Mesh::recalculateNormals() {
	for (Face* face: this->faces) {
		face->computeNormal();
	}

	this->vertexNormals.clear();
	this->vertexNormals.reserve(this->numVerts());
	for (int i = 0; i < this->numVerts(); ++i) {
		this->vertexNormals[i] = std::vector<float>(3, 0.0f);
	}

	for (HalfEdge* hf: this->halfEdges) {
		std::vector<float> prev = hf->prev->origin->coordinates;
		std::vector<float> curr = hf->origin->coordinates;
		std::vector<float> next = hf->next->origin->coordinates;

		glm::vec3 vecPrev(prev[0], prev[1], prev[2]);
		glm::vec3 vecCurr(curr[0], curr[1], curr[2]);
		glm::vec3 vecNext(next[0], next[1], next[2]);

		glm::vec3 e1 = vecPrev - vecCurr;
		glm::vec3 e2 = vecNext - vecCurr;

		float edgeLength = e1.length() * e2.length();
		float edgeDot = glm::dot(e1, e2) / edgeLength;
		float angle = std::sqrt(1 - edgeDot * edgeDot);

		glm::vec3 faceNormal(hf->face->normal[0], hf->face->normal[1], hf->face->normal[2]);
		glm::vec3 norm = (angle * faceNormal) / edgeLength;

		this->vertexNormals[hf->origin->index][0] += norm.x;
		this->vertexNormals[hf->origin->index][1] += norm.y;
		this->vertexNormals[hf->origin->index][2] += norm.z;
	}

	for (std::vector<float>& normal : this->vertexNormals) {
		glm::vec3 vec(normal[0], normal[1], normal[2]);
		glm::vec3 norm = vec / ((float)vec.length());

		normal[0] = norm.x;
		normal[1] = norm.y;
		normal[2] = norm.z;
	}
}


void Mesh::updateRegularQuadIndices() {
	this->regularQuadIndices.clear();

	for (Face* face: this->faces) {
        HalfEdge* hf = face->side;

        // Checks if it is a quad with valency 4
        if (face->valence == 4 && !hf->isBoundaryEdge()) {
             std::vector<unsigned int> subsetQuadIndices;
             bool saveIndices = true;
             for (int m = 0; m < face->valence; m++) {
                if (!isEdgeRegularCandidate(hf)){
                    saveIndices = false;
                    break;
                }
                // Append the index of vertex
                subsetQuadIndices.push_back(hf->origin->index);

                // Extracting patch boundary vertex index
                HalfEdge* intermEdge = hf->twin->next->next;

                if (!isEdgeRegularCandidate(intermEdge)){
                    saveIndices = false;
                    break;
                }
                // Append the index of vertex
                subsetQuadIndices.push_back(intermEdge->origin->index);

                intermEdge = intermEdge->next;
                if (!isEdgeRegularCandidate(intermEdge)){
                    saveIndices = false;
                    break;
                }
                // Append the index of vertex
				subsetQuadIndices.push_back(intermEdge->origin->index);

                intermEdge = intermEdge->twin->next->next;
                if (!isEdgeRegularCandidate(intermEdge)){
                    saveIndices = false;
                    break;
                }
                // Append the index of vertex
				subsetQuadIndices.push_back(intermEdge->origin->index);

                // Update the currentEdge
                hf = hf->next;
             }
             if (saveIndices){
				std::vector<unsigned int> newSubsetQuadIndices = orderQuadIndices(subsetQuadIndices);
				for (unsigned int t : newSubsetQuadIndices) {
					this->regularQuadIndices.push_back(t);
				}
             }
        }
    }
    this->regularQuadIndices.shrink_to_fit();
}


bool Mesh::isEdgeRegularCandidate(HalfEdge* intermEdge) {
	return !(intermEdge->origin->valence != 4 || intermEdge->isBoundaryEdge());
}

std::vector<unsigned int> Mesh::orderQuadIndices(std::vector<unsigned int> oldQuadIndices) {
	std::vector<unsigned int> newQuadIndices;
	newQuadIndices.reserve(oldQuadIndices.size());

	newQuadIndices.push_back(oldQuadIndices[11]);
    newQuadIndices.push_back(oldQuadIndices[10]);
    newQuadIndices.push_back(oldQuadIndices[9]);
    newQuadIndices.push_back(oldQuadIndices[7]);
    newQuadIndices.push_back(oldQuadIndices[13]);
    newQuadIndices.push_back(oldQuadIndices[12]);
    newQuadIndices.push_back(oldQuadIndices[8]);
    newQuadIndices.push_back(oldQuadIndices[6]);
    newQuadIndices.push_back(oldQuadIndices[14]);
    newQuadIndices.push_back(oldQuadIndices[0]);
    newQuadIndices.push_back(oldQuadIndices[4]);
    newQuadIndices.push_back(oldQuadIndices[5]);
    newQuadIndices.push_back(oldQuadIndices[15]);
    newQuadIndices.push_back(oldQuadIndices[1]);
    newQuadIndices.push_back(oldQuadIndices[2]);
    newQuadIndices.push_back(oldQuadIndices[3]);

	return newQuadIndices;
}


int Mesh::numVerts() { return this->vertices.size(); }
int Mesh::numHalfEdges() { return this->halfEdges.size(); }
int Mesh::numFaces() { return this->faces.size(); }
int Mesh::numEdges() { return this->edgeCount; }
