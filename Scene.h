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
	
	/*Material *PBRmaterial;
	Material *enviDiffuseMaterial;
	Material *prefilteredMaterial;
	Material *brdfLUTMaterial;*/

	// Entity mesh
	Mesh * skyBoxMesh;
	Mesh * groundMesh;

	Mesh* houseMesh;
	Material* houseMaterial;

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

	Entity* house;

	PointLight pointLight0;
	PointLight pointLight1;
	PointLight pointLight2;
	PointLight pointLight3;

	DirectionalLight sunLight;
	

	// use for shadow map
	std::vector<Entity *> entitiesOpaque;
	Material* shadowMapMat;


	// Actually another scene, there's a cube to draw the skybox on and to be captured to generate irradiance
	//Entity * cubeForCaptureEnviDiffuse;
	//Entity * cubeForCapturePrefiltered;
	//Entity * brdfLUT;

	//// use for PBR
	//XMFLOAT3 albedo;
	//float metallic[5];
	//float roughness[5];
	//float ao;

	// Initialization helper methods - feel free to customize, combine, etc.
	void CreateBasicGeometry(ID3D11Device *);
	void CreateMaterial(ID3D11Device *, ID3D11DeviceContext *);
	void CreateLights();
	void CreateEntities();

	void init(ID3D11Device *, ID3D11DeviceContext *);
};

