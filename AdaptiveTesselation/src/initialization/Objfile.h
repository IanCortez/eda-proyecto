#ifndef OBJFILE_H
#define OBJFILE_H


#include <string>
#include <vector>
#include <sstream>


class Objfile {

private:
	std::vector<std::vector<float>> vertexCoordinates; // N * 3
	std::vector<std::vector<float>> textureCoordinates; // N * 2
	std::vector<std::vector<float>> vertexNormals; // N * 3
	std::vector<int> faceValences;
	std::vector<std::vector<int>> faceCoordinatesId;
	std::vector<std::vector<int>> faceTexturesId;
	std::vector<std::vector<int>> faceNormalId;
	bool loaded;

	void handleVertexCoordinates(std::istringstream& iss);
	void handleVertexTextureCoordinates(std::istringstream& iss);
	void handleVertexNormal(std::istringstream& iss);
	void handleFace(std::istringstream& iss);


public:
	Objfile(std::string filename);
	~Objfile();

	bool loadedSuccesfully() const;
	void normalizeMesh(float desiredScale);
};


#endif
