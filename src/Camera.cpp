#include "Camera.h"

Camera::Camera(){
	this->pos = float3(0, 0, 0);
	this->dir = float3(0, 0, 1);
	this->focalDistance = 1;
}
	
Camera::Camera(float3 pos, float3 dir){
		this->pos = pos;
		this->dir = dir;
		this->focalDistance = 1;
}
