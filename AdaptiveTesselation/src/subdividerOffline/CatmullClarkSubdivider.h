#ifndef CATMULL_CLARK_SUBDIVIDER_H
#define CATMULL_CLARK_SUBDIVIDER_H


#include <vector>

#include "../mesh/Vertex.h"
#include "../mesh/Mesh.h"
#include "../mesh/HalfEdge.h"
#include "../mesh/Face.h"
#include "Subdivider.h"

#include <glm/glm.hpp>



class CatmullClarkSubdivider: public Subdivider {

private:
	void reserveSizes(Mesh& mesh, Mesh& newMesh) const;
	void refineGeometry(Mesh& mesh, Mesh& newMesh) const;
	void refineTopology(Mesh& mesh, Mesh& newMesh) const;

	void setHalfEdgeData(Mesh& newMesh, int h, int edgeIndex, int vertexIndex, int twinIndex) const;

	std::vector<float> facePoint(const Face& face) const;
	std::vector<float> edgePoint(const HalfEdge& edge) const;
	std::vector<float> boundaryEdgePoint(const HalfEdge& edge) const;
	std::vector<float> vertexPoint(const Vertex& vertex) const;
	std::vector<float> boundaryVertexPoint(const Vertex& vertex) const;

public:
	CatmullClarkSubdivider();
	Mesh subdivide(Mesh& mesh) const override;
};


#endif
