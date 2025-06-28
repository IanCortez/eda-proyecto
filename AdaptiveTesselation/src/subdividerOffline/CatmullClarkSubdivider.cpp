#include "CatmullClarkSubdivider.h"


CatmullClarkSubdivider::CatmullClarkSubdivider() {}


Mesh CatmullClarkSubdivider::subdivide(Mesh& mesh) const {
	Mesh newMesh;
	this->reserveSizes(mesh, newMesh);
	this->refineGeometry(mesh, newMesh);
	this->refineTopology(mesh, newMesh);
	return newMesh;
}


void CatmullClarkSubdivider::reserveSizes(Mesh& mesh, Mesh& newMesh) const {
	int newEdgesN = 2 * mesh.numEdges() + mesh.numHalfEdges();
	int newFacesN = mesh.numHalfEdges();
	int newHalfEdgesN = mesh.numHalfEdges() * 4;
	int newVerticesN = mesh.numVerts() + mesh.numFaces() + mesh.numEdges();

	newMesh.vertices.resize(newVerticesN);
	newMesh.halfEdges.resize(newHalfEdgesN);
	newMesh.faces.resize(newFacesN);
	newMesh.edgeCount = newEdgesN;
}


void CatmullClarkSubdivider::refineGeometry(Mesh& mesh, Mesh& newMesh) const {
    std::vector<Vertex*> &vertices = mesh.vertices;
	std::vector<Vertex*> &newVertices = newMesh.vertices;
    std::vector<Face*> &faces = mesh.faces;

    // Face Points
    for (int f = 0; f < mesh.numFaces(); f++) {
		std::vector<float> coords = this->facePoint(*faces[f]);
        int i = mesh.numVerts() + faces[f]->index;
        // Face points always inherit the valence of the face
        Vertex* facePoint = new Vertex(coords, nullptr, faces[f]->valence, i);
        newVertices[i] = facePoint;
    }

    // Edge Points
    std::vector<HalfEdge*> &halfEdges = mesh.halfEdges;
    for (int h = 0; h < mesh.numHalfEdges(); h++) {
        HalfEdge* currentEdge = halfEdges[h];
        // Only create a new vertex per set of halfEdges (i.e. once per undirected
        // edge)
        if (h > currentEdge->twinIndex()) {
            int v = mesh.numVerts() + mesh.numFaces() + currentEdge->edgeIndex;
            int valence;
            std::vector<float> coords;
            if (currentEdge->isBoundaryEdge()) {
                coords = this->boundaryEdgePoint(*currentEdge);
                valence = 3;
            } else {
                coords = this->edgePoint(*currentEdge);
                valence = 4;
            }
            newVertices[v] = new Vertex(coords, nullptr, valence, v);
        }
    }

    // Vertex Points
    for (int v = 0; v < mesh.numVerts(); v++) {
		std::vector<float> coords;
        if (vertices[v]->isBoundaryVertex()) {
            coords = boundaryVertexPoint(*vertices[v]);
        } else {
            coords = vertexPoint(*vertices[v]);
        }
        newVertices[v] = new Vertex(coords, nullptr, vertices[v]->valence, v);
    }
}


void CatmullClarkSubdivider::refineTopology(Mesh& mesh, Mesh& newMesh) const {
	for (int f = 0; f < newMesh.numFaces(); ++f) {
        newMesh.faces[f]->index = f;
        newMesh.faces[f]->valence = 4;
    }

    // Split halfedges
    for (int h = 0; h < mesh.numHalfEdges(); ++h) {
        HalfEdge* edge = mesh.halfEdges[h];

        int h1 = 4 * h;
        int h2 = 4 * h + 1;
        int h3 = 4 * h + 2;
        int h4 = 4 * h + 3;

        int twinIdx1 = edge->twinIndex() < 0 ? -1 : 4 * edge->twin->next->index + 3;
        int twinIdx2 = 4 * edge->next->index + 2;
        int twinIdx3 = 4 * edge->prev->index + 1;
        int twinIdx4 = 4 * edge->prev->twinIndex();

        int vertIdx1 = edge->origin->index;
        int vertIdx2 = mesh.numVerts() + mesh.numFaces() + edge->edgeIndex;
        int vertIdx3 = mesh.numVerts() + edge->faceIndex();
        int vertIdx4 = mesh.numVerts() + mesh.numFaces() + edge->prev->edgeIndex;

        int edgeIdx1 = 2 * edge->edgeIndex + (h > edge->twinIndex() ? 0 : 1);
        int edgeIdx2 = 2 * mesh.numEdges() + h;
        int edgeIdx3 = 2 * mesh.numEdges() + edge->prev->index;
        int edgeIdx4 = 2 * edge->prev->edgeIndex + (edge->prevIndex() > edge->prev->twinIndex() ? 1 : 0);

        this->setHalfEdgeData(newMesh, h1, edgeIdx1, vertIdx1, twinIdx1);
        this->setHalfEdgeData(newMesh, h2, edgeIdx2, vertIdx2, twinIdx2);
        this->setHalfEdgeData(newMesh, h3, edgeIdx3, vertIdx3, twinIdx3);
        this->setHalfEdgeData(newMesh, h4, edgeIdx4, vertIdx4, twinIdx4);
    }
}


void CatmullClarkSubdivider::setHalfEdgeData(Mesh& newMesh, int h, int edgeIndex, int vertexIndex, int twinIndex) const {
	HalfEdge* halfEdge = newMesh.halfEdges[h];

	halfEdge->edgeIndex = edgeIndex;
	halfEdge->index = h;
	halfEdge->origin = newMesh.vertices[vertexIndex];
	halfEdge->face = newMesh.faces[halfEdge->faceIndex()];
	halfEdge->next = newMesh.halfEdges[halfEdge->nextIndex()];
	halfEdge->prev = newMesh.halfEdges[halfEdge->prevIndex()];
	halfEdge->twin = twinIndex < 0 ? nullptr : newMesh.halfEdges[twinIndex];

	halfEdge->origin->out = halfEdge;
	halfEdge->origin->index = vertexIndex;
	halfEdge->face->side = halfEdge;
}


std::vector<float> CatmullClarkSubdivider::facePoint(const Face& face) const {
	std::vector<float> edgeVec(3, 0.0f);
	HalfEdge* edge = face.side;

	for (int i = 0; i < face.valence; ++i) {
		edgeVec[0] += edge->origin->coordinates[0];
		edgeVec[1] += edge->origin->coordinates[1];
		edgeVec[2] += edge->origin->coordinates[2];
		edge = edge->next;
	}
	glm::vec3 tempVec(edgeVec[0], edgeVec[1], edgeVec[1]);
	tempVec = (1.0f / face.valence) * tempVec;
	edgeVec[0] = tempVec.x;
	edgeVec[1] = tempVec.y;
	edgeVec[2] = tempVec.z;

	return edgeVec;
}


std::vector<float> CatmullClarkSubdivider::edgePoint(const HalfEdge& edge) const {
	std::vector<float> temp = this->boundaryEdgePoint(edge);
	std::vector<float> f1 = this->facePoint(*edge.face);
	std::vector<float> f2 = this->facePoint(*edge.twin->face);

	temp[0] += (f1[0] + f2[0]) / 2.0f;
	temp[1] += (f1[1] + f2[1]) / 2.0f;
	temp[2] += (f1[2] + f2[2]) / 2.0f;

	temp[0] /= 2.0f;
	temp[1] /= 2.0f;
	temp[2] /= 2.0f;
	return temp;
}


std::vector<float> CatmullClarkSubdivider::boundaryEdgePoint(const HalfEdge& edge) const {
	std::vector<float> res(3, 0.0f);
	res[0] = (edge.origin->coordinates[0] + edge.next->origin->coordinates[0]) / 2.0f;
	res[1] = (edge.origin->coordinates[1] + edge.next->origin->coordinates[1]) / 2.0f;
	res[2] = (edge.origin->coordinates[2] + edge.next->origin->coordinates[2]) / 2.0f;
	return res;
}


std::vector<float> CatmullClarkSubdivider::vertexPoint(const Vertex& vertex) const {
	HalfEdge* hf = vertex.out;
	std::vector<float> R(3, 0.0f);
	std::vector<float> Q(3, 0.0f);

	for (int i = 0; i < vertex.valence; ++i) {
		R[0] += (hf->origin->coordinates[0] + hf->next->origin->coordinates[0]) / 2.0f;
		R[1] += (hf->origin->coordinates[1] + hf->next->origin->coordinates[1]) / 2.0f;
		R[2] += (hf->origin->coordinates[2] + hf->next->origin->coordinates[2]) / 2.0f;

		std::vector<float> tt = this->facePoint(*hf->face);
		Q[0] += tt[0];
		Q[1] += tt[1];
		Q[2] += tt[2];

		hf = hf->prev->twin;
	}

	float n = float(vertex.valence);
	Q[0] /= n;
	Q[1] /= n;
	Q[2] /= n;

	R[0] /= n;
	R[1] /= n;
	R[2] /= n;

	std::vector<float> res(3, 0.0f);
	res[0] = (Q[0] + 2 * R[0] + (vertex.coordinates[0] * (n - 3.0f)) ) / n;
	res[1] = (Q[1] + 2 * R[1] + (vertex.coordinates[1] * (n - 3.0f)) ) / n;
	res[2] = (Q[2] + 2 * R[2] + (vertex.coordinates[2] * (n - 3.0f)) ) / n;
	return res;
}


std::vector<float> CatmullClarkSubdivider::boundaryVertexPoint(const Vertex& vertex) const {
	std::vector<float> res = vertex.coordinates;
	res[0] *= 2;
	res[1] *= 2;
	res[2] *= 2;

	std::vector<float> b1 = this->boundaryEdgePoint(*vertex.nextBoundaryHalfEdge());
	std::vector<float> b2 = this->boundaryEdgePoint(*vertex.prevBoundaryHaflEdge());

	res[0] += b1[0] + b2[0];
	res[1] += b1[1] + b2[1];
	res[2] += b1[2] + b2[2];

	res[0] /= 4.0f;
	res[1] /= 4.0f;
	res[2] /= 4.0f;

	return res;
}
