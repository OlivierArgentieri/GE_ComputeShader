#pragma once
#include <glm.hpp>

class Transform
{
private:
	glm::mat4 modelMatrix = glm::mat4(0);

public:
	glm::vec3 position;
	glm::vec3 scale;

	Transform();
	Transform(glm::vec3 _position, glm::vec3 _scale);
	Transform(const Transform& _transform);

	void SetPosition(glm::vec3 _position);
	void Translate(glm::vec3 _position);
	void SetScale(glm::vec3 _scale);
	void SetRotation(float _angle, glm::vec3 _normalRotation);

	glm::mat4 GetModelMatrix()const { return modelMatrix; }

};
