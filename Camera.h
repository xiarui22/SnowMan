#pragma once
#include "DXCore.h"
#include "SimpleShader.h"
#include <DirectXMath.h>
#include "Entity.h"

class Camera
{
	
	DirectX::XMFLOAT3 camPos;
	DirectX::XMFLOAT3 camDir;
	float rotationX;
	float rotationY;
	DirectX::XMFLOAT4X4 viewMatrix;
	DirectX::XMFLOAT4X4 projectionMatrix;

public:
	Camera();
	Camera(DirectX::XMFLOAT3, DirectX::XMFLOAT3);
	~Camera();

	DirectX::XMFLOAT3 GetCamPos();

	float GetRotationx();
	float GetRotationy();

	DirectX::XMFLOAT4X4 GetView();  //look to
	DirectX::XMFLOAT4X4 GetProjection();
	void UpdateProjection(unsigned int, unsigned int);

	//void Update(XMFLOAT3, XMFLOAT3, float, float);
	void Update(float);

	void SetRotation(float, float);
};

