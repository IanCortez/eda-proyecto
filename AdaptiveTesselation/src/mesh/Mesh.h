#ifndef MESH_H
#define MESH_H


#include <vector>


class Vertex;
class Face;
class HalfEdge;


class Mesh {

private:
	std::vector<std::vector<float>> vertexCoords;
	std::vector<std::vector<float>> vertexNormals;
	std::vector<unsigned int> polyIndices;

	// for quad tessellation
	std::vector<unsigned int> quadIndices;

	// for regular quad tessellation
	std::vector<unsigned int> regularQuadIndices;

	std::vector<Vertex*> vertices;
	std::vector<Face*> faces;
	std::vector<HalfEdge*> halfEdges;

	int edgeCount;

	// These classes require access to the private fields to prevent a bunch of
	// function calls.
	friend class MeshInitializer;
	friend class Subdivider;
	friend class CatmullClarkSubdivider;
	friend class LimitPositionSubdivider;


public:
	~Mesh();

	void extractAttributes();
	void recalculateNormals();
	void updateRegularQuadIndices();
	bool isEdgeRegularCandidate(HalfEdge* intermEdge);

	std::vector<unsigned int> orderQuadIndices(std::vector<unsigned int> oldQuadIndices);

	int numVerts();
	int numHalfEdges();
	int numFaces();
	int numEdges();
};


#endif