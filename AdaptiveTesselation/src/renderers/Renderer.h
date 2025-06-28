#ifndef RENDERER_H
#define RENDERER_H


#include "../utils/ShaderTypes.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>



class Renderer {
protected:


public:
	Renderer();

	virtual ~Renderer();

	virtual void initShaders() = 0;
	virtual void initBuffers() = 0;

	void init();
};


#endif
