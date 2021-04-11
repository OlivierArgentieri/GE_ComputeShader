#pragma once
#include <glew.h>
#include <vector>
#include <glm/glm.hpp>

#include "Transform.hpp"

class GraphicObject {

private:
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;

	GLuint vertexbuffer = 0;
	GLuint uvbuffer = 0;

public:
	std::vector<glm::vec3> GetVertices() const { return vertices; }
	std::vector<glm::vec2> GetUvs() const { return uvs; }
	std::vector<glm::vec3> GetNormals() const { return normals; }

	GLuint GetVertexBuffer() const { return vertexbuffer; }
	GLuint GetUvBuffer() const { return uvbuffer; }

	Transform transform;

	void LoadFromFile(const char* _path);
	bool IsValid();
public:
	// graphic command
	
	/** manage buffers */
	void ComputeBuffers();

	/** Draw buffers */
	void Draw();
};
