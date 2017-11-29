#include "template.h"

Camera::Camera(){
	this->pos = float3(0, 0, 0);
	this->dir = float3(0, 0, 1);
	this->d = 1;
}
	
Camera::Camera(float3 pos, float3 dir){
		this->pos = pos;
		this->dir = dir;
		this->d = 1;
}

void Camera::calculateScreen(){
    screenCenter = pos + dir*d;
    
    p0.x = screenCenter.x - 1;
    p0.y = screenCenter.y - 1;
    
    p1.x = screenCenter.x + 1;
    p1.y = screenCenter.y - 1;
    
    p2.x = screenCenter.x - 1;
    p2.y = screenCenter.y + 1;
}

Ray Camera::generateRay(float2 p){
    
    float3 rayDir = p0 + (p1-p0)*p.x + (p2-p0)*p.y;
    rayDir.normalize();
    
    return Ray(pos,rayDir);
}
