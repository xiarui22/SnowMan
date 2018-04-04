#pragma once
#include "DXCore.h"
#include "SimpleShader.h"
#include <DirectXMath.h>
#include <vector>
#include "Mesh.h"
#include "Material.h"


class Entity
{
	Mesh* mesh;
	Material* material;

	DirectX::XMFLOAT4X4 worldMatrix;

	DirectX::XMFLOAT3 translation;
	DirectX::XMFLOAT3 rotate;
	DirectX::XMFLOAT3 scale;

public:
	Entity();
	Entity(Mesh*, Material*, DirectX::XMFLOAT3, DirectX::XMFLOAT3, DirectX::XMFLOAT3);
	~Entity();

	void Update();

	Entity* parent;
	std::vector<Entity*> children;
	void SetParent(Entity* e);


	void setTranslation(float, float, float);
	void setScale(float, float, float);
	void setRotate(float, float, float);
	DirectX::XMFLOAT3 getTranslation();
	DirectX::XMFLOAT3 getScale();
	DirectX::XMFLOAT3 getRotate();

	void setWorld();
	DirectX::XMFLOAT4X4 getWorld();

	Mesh* GetMesh();
	Material* GetMaterial();

};

