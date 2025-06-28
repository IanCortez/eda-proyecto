#include "Utils.h"

#include <glm/glm.hpp>


float calculateBoundingBoxScale(const std::vector<std::vector<float>>& coordinates, const float desiredScale) {
	glm::vec3 minCoord(coordinates[0][0], coordinates[0][1], coordinates[0][2]);
    glm::vec3 maxCoord(coordinates[0][0], coordinates[0][1], coordinates[0][2]);
    for (std::size_t i = 1; i < coordinates.size(); ++i) {
        glm::vec3 coord(coordinates[i][0], coordinates[i][1], coordinates[i][2]);

		minCoord.x = std::min(coord.x, minCoord.x);
        minCoord.y = std::min(coord.y, minCoord.y);
        minCoord.z = std::min(coord.z, minCoord.z);

        maxCoord.x = std::max(coord.x, maxCoord.x);
        maxCoord.y = std::max(coord.y, maxCoord.y);
        maxCoord.z = std::max(coord.z, maxCoord.z);
    }
    glm::vec3 dims = maxCoord - minCoord;
	return desiredScale / std::min(dims.x, dims.y);
}
