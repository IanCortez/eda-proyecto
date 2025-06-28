#ifndef SUBDIVIDER_H
#define SUBDIVIDER_H


#include "../mesh/Mesh.h"


class Subdivider {
public:
	virtual ~Subdivider();
	virtual Mesh subdivide(Mesh& mesh) const = 0;
};


#endif
