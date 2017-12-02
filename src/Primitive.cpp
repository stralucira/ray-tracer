#include "template.h"
#include "Primitive.h"

Primitive::Primitive(vec3 pos)
{
	this->Init(pos); 
};

Primitive::Primitive(glm::mat4 transformMatrix)
{
	this->Init(transformMatrix);
};

void Primitive::Init(vec3 pos)
{
	this->Init(translate(glm::mat4(1.0f), pos));

};

void Primitive::Init(glm::mat4 transformMatrix)
{
	this->transformMatrix = transformMatrix;
	this->scale = vec3();
	this->rot = quat();
	this->pos = vec3();
	this->skew = vec3();
	this->perspective = vec4();
	this->UpdatePosition();
};

vec3 Primitive::GetPosition()
{
	return this->pos;
};

vec3 Primitive::GetDirectionVector()
{
	return rot * vec3(1.0f, 0.0f, 0.0f);
};

void Primitive::UpdatePosition()
{
	decompose(transformMatrix, scale, rot, pos, skew, perspective);
	rot = conjugate(rot);
}
