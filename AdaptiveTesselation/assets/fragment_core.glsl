#version 330 core
out vec4 FragColor;

in vec3 ourColor;

void main() {
	// FragColor = vec4(1.0f, 0.2f, 0.6f, 1.0f);
	FragColor = vec3(ourColor, 1.0);
}