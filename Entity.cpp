#include "Entity.h"

using namespace DirectX;

Entity::Entity()
{
}

Entity::Entity(Mesh *pp, Material *mm, XMFLOAT3 pos, XMFLOAT3 rot, XMFLOAT3 sca)
{
	mesh = pp;
	material = mm;
	XMStoreFloat4x4(&worldMatrix, XMMatrixIdentity());
	translation = pos;
	scale = rot;
	rotate = sca;
	parent = NULL;
	setWorld();
}

void Entity::SetParent(Entity * e)
{
	if (parent != nullptr) {
		for (unsigned i = 0; i < parent->children.size(); i++) {
			if (parent->children[i] == this) {
				parent->children.erase(parent->children.begin() + i);
				break;
			}
		}
	}

	parent = e;
	e->children.push_back(this);
}

void Entity::setTranslation(float x, float y, float z) {
	translation = XMFLOAT3(x, y, z);
}

void Entity::setScale(float x, float y, float z) {
	scale = XMFLOAT3(x, y, z);
}
void Entity::setRotate(float x, float y, float z) {
	rotate = XMFLOAT3(x, y, z);
}
XMFLOAT3 Entity::getTranslation() {
	return translation;
}
XMFLOAT3 Entity::getScale() {
	return scale;
}
XMFLOAT3 Entity::getRotate() {
	return rotate;
}

void Entity::setWorld() {
	XMFLOAT3 tm = translation;
	XMFLOAT3 sm = scale;
	XMFLOAT3 rm = rotate;
	XMMATRIX translationMat = XMMatrixTranslation(tm.x, tm.y, tm.z);
	XMMATRIX scaleMat = XMMatrixScaling(sm.x, sm.y, sm.z);
	XMMATRIX rotationMat = XMMatrixRotationZ(rm.z);
	rotationMat = XMMatrixMultiply(rotationMat, XMMatrixRotationX(rm.x));
	rotationMat = XMMatrixMultiply(rotationMat, XMMatrixRotationY(rm.y));

	XMVECTOR rot = XMLoadFloat3(&rm);
	XMMATRIX rotation0 = XMMatrixRotationRollPitchYawFromVector(rot);

	XMStoreFloat4x4(&worldMatrix, XMMatrixTranspose(scaleMat * rotation0 * translationMat));


	if (parent != nullptr) {
		XMMATRIX local = scaleMat * rotation0 * translationMat;
		local = XMMatrixMultiply(local, XMMatrixTranspose(XMLoadFloat4x4(&parent->getWorld())));
		XMStoreFloat4x4(&worldMatrix, XMMatrixTranspose(local));
	}

	if (children.size()>0) {
		for (Entity* e : children) {
			e->setWorld();
		}
	}
}


XMFLOAT4X4 Entity::getWorld(  ) {
	
	return worldMatrix;
}

Mesh* Entity::GetMesh()
{
	return mesh;
}

Material* Entity::GetMaterial()
{
	return material;
}

Entity::~Entity()
{
}

void Entity::Update()
{
	setWorld();
}
