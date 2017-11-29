#include "template.h"

Camera::Camera(){
	this->pos = float3(0, 0, 0);
	this->dir = float3(0, 0, 1);
	this->d = 1;

    this->aspectRatio = 4/3;
	this->calculateScreen();
}
	
Camera::Camera(float3 pos, float3 dir, float aspectRatio){
	this->pos = pos;
	this->dir = dir;
	this->d = 1;

    this->aspectRatio = aspectRatio;
	this->calculateScreen();
}

void Camera::calculateScreen(){
    screenCenter = pos + dir*d;
    
	p0 = screenCenter + float3(-1, -0.75, 0);
	p1 = screenCenter + float3(1, -0.75, 0);
	p2 = screenCenter + float3(-1, 0.75, 0);
}

Ray Camera::generateRay(float2 p){
    
    float3 rayDir = p0 + (p1-p0)*p.x + (p2-p0)*p.y;
    rayDir.normalize();
    
    return Ray(pos,rayDir);
}
