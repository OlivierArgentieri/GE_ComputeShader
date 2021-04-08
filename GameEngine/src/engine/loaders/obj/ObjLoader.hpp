#pragma once

#include <vector>
#include <ext/vector_float2.hpp>
#include <ext/vector_float3.hpp>


class ObjLoader
{
public:
	static bool Load(const char* path, std::vector<glm::vec3>& out_vertices, std::vector<glm::vec2>& out_uvs, std::vector<glm::vec3>& out_normals);
};