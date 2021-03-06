#pragma once

#include "Material.h"
#include "Light.h"
#include "Mesh.h"
#include "Entity.h"

class Scene
{
	Mesh* sphereMesh;
	Mesh* cubeMesh;

    Material* skyBoxMaterial;
	Material* snowManMaterial;
	Material* snowManEyesMaterial;
	Material* groundMaterial;
	Material* carMaterial;

	Material *enviDiffuseMaterial;
	Material *prefilteredMaterial;
	Material *brdfLUTMaterial;

	// Entity mesh
	Mesh * groundMesh;
public:
	Scene();
	~Scene();
	Entity* snowMan0Body;
	Entity* snowMan0Head;
	Entity* snowMan0Eye0;
	Entity* snowMan0Eye1;

	Entity* snowMan1Body;
	Entity* snowMan1Head;
	Entity* snowMan1Eye0;
	Entity* snowMan1Eye1;

	Entity* ground;
	Entity* skyBox;
	Entity* car;

	PointLight pointLight0;
	PointLight pointLight1;
	PointLight pointLight2;
	PointLight pointLight3;

	// use for shadow map
	std::vector<Entity *> entitiesOpaque;
	Material* shadowMapMat;


	// Actually another scene, there's a cube to draw the skybox on and to be captured to generate irradiance
	Entity * cubeForCaptureEnviDiffuse;
	Entity * cubeForCapturePrefiltered;
	Entity * brdfLUT;

	// Initialization helper methods - feel free to customize, combine, etc.
	void CreateBasicGeometry(ID3D11Device *);
	void CreateMaterial(ID3D11Device *, ID3D11DeviceContext *);
	void CreateLights();
	void CreateEntities();

	void init(ID3D11Device *, ID3D11DeviceContext *);
};

