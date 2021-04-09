
#include <ext/matrix_transform.hpp>
#include "Transform.hpp"
Transform::Transform() : Transform(glm::vec3(),  glm::vec3())
{
}

Transform::Transform(glm::vec3 _position, glm::vec3 _scale)
{
	position = _position;
	scale = _scale;
}


Transform::Transform(const Transform& _transform) : Transform(_transform.position, _transform.scale)
{
	modelMatrix = _transform.modelMatrix;
}

void Transform::SetPosition(glm::vec3 _position)
{
	position = _position;
	modelMatrix = glm::mat4(1);
	modelMatrix = glm::translate(modelMatrix, _position);
}

void Transform::Translate(glm::vec3 _position)
{
	position += _position;
	modelMatrix = glm::translate(modelMatrix, _position);
}

void Transform::SetScale(glm::vec3 _scale)
{
	scale = _scale;
}

/// <summary>
/// Using GLM Rotation matrix
/// </summary>
/// <param name="_angle">deg</param>
/// <param name="_nomalRotation">normal of rotation </param>
void Transform::SetRotation(float _angle, glm::vec3 _normalRotation)
{
	if (_normalRotation == glm::vec3(0)) return;
	modelMatrix = glm::rotate<float>(glm::mat4(1.0f), glm::radians(_angle), _normalRotation);
}