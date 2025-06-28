#include "Objfile.h"


#include <iostream>
#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../utils/Utils.h"



Objfile::Objfile(std::string filename) {
	std::string file = filename + ".obj";
	std::ifstream inFile (file);

	if (inFile.is_open()) {

		std::string line, type;
		int n = 0;

		while (!inFile.eof()) {
			std::istringstream iss(line);
			iss >> type; 
			if (type == "v") {
				this->handleVertexCoordinates(iss);
			} else if (type == "vt") {
				this->handleVertexTextureCoordinates(iss);
			} else if (type == "vn") {
				this->handleVertexNormal(iss);
			} else if (type == "f") { 
				this->handleFace(iss);
			} else {
				std::cout << "ignored " << type << " line of .OBJ file" << std::endl;
			}
		}

		inFile.close();
		this->normalizeMesh(2);
		this->loaded = true;
	} else {
		this->loaded = false;
	}
}


Objfile::~Objfile() {}


void Objfile::handleVertexCoordinates(std::istringstream& iss) {
	float v1, v2, v3;
	iss >> v1 >> v2 >> v3;
	std::vector<float> vec = { v1, v2, v3 };
	this->vertexCoordinates.push_back(vec);
}


void Objfile::handleVertexTextureCoordinates(std::istringstream& iss) {
	float v1, v2;
	iss >> v1 >> v2;
	std::vector<float> vec = { v1, v2 };
	this->textureCoordinates.push_back(vec);
}


void Objfile::handleVertexNormal(std::istringstream& iss) {
	float v1, v2, v3;
	iss >> v1 >> v2 >> v3;
	std::vector<float> vec = { v1, v2, v3 };
	this->vertexNormals.push_back(vec);
}


void Objfile::handleFace(std::istringstream& iss) {
	std::vector<int> faceCoordinatesIdx;
	std::vector<int> faceTexturesIdx;
	std::vector<int> faceNormalIdx;

	std::string val;
	std::vector<std::string> values;
	while (iss >> val) values.push_back(val);

	for (int k = 1; k < values.size(); ++k) {
		std::stringstream ss(values[k]);
		std::vector<std::string> tokens;
		std::string token;

		while (std::getline(ss, token, '/')) {
			tokens.push_back(token);
		}

		faceCoordinatesIdx.push_back(std::stoi(tokens[0]) - 1);

		if (tokens.size() > 1) {
			if (!tokens[1].empty()) {
				faceTexturesIdx.push_back(std::stoi(tokens[1]) - 1);
			}
			if (tokens.size() > 2) {
				if (!tokens[2].empty()) {
					faceNormalIdx.push_back(std::stoi(tokens[2]) - 1);
				}
			}
		}
	}

	this->faceCoordinatesId.push_back(faceCoordinatesIdx);
	this->faceTexturesId.push_back(faceTexturesIdx);
	this->faceNormalId.push_back(faceNormalIdx);
	this->faceValences.push_back(values.size() - 1);
}


bool Objfile::loadedSuccesfully() const {
	return this->loaded;
}


void Objfile::normalizeMesh(float desiredScale) {
	float scale = calculateBoundingBoxScale(this->vertexCoordinates, desiredScale);
	glm::mat4x4 transform(1.0f);
	glm::highp_mat4x4 scaled = glm::scale(transform, glm::vec3(scale, scale, scale));
	transform = glm::scale(transform, glm::vec3(scale, scale, scale));

	for (int i = 0; i < this->vertexCoordinates.size(); ++i) {
		glm::vec4 vec(this->vertexCoordinates[i][0], this->vertexCoordinates[i][1], this->vertexCoordinates[i][2], 1.0f);
		glm::vec4 res = transform * vec;
		this->vertexCoordinates[i][0] = res.x;
		this->vertexCoordinates[i][1] = res.y;
		this->vertexCoordinates[i][2] = res.z;
	}
}
