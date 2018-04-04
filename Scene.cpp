#include "Scene.h"

using namespace std;
Scene::Scene()
{
}


Scene::~Scene()
{
	delete snowMan0Body;
	delete snowMan0Head;
	delete snowMan0Eye0;
	delete snowMan0Eye1;

	delete cubeMesh;
	delete sphereMesh;

	delete skyBoxMaterial;
	delete snowManMaterial;
	delete snowManEyesMaterial;
	delete groundMaterial;

	// Entity mesh
	
	delete skyBoxMesh;
	delete groundMesh;
}


// --------------------------------------------------------
// Creates the geometry we're going to draw - a single triangle for now
// --------------------------------------------------------
void Scene::CreateBasicGeometry(ID3D11Device * device)
{
	// Create the VERTEX BUFFER description -----------------------------------
	// - The description is created on the stack because we only need
	//    it to create the buffer.  The description is then useless.
	sphereMesh = new Mesh("Assets/Models/sphere.obj", device);
	//mesh = new Mesh("Assets/Models/building.obj", device);
	cubeMesh = new Mesh("Assets/Models/cube.obj", device);

	// For 2d Plane
	Vertex vertices[4];
	vertices[0].Position = XMFLOAT3(-0.5, -0.5, 0);
	vertices[0].UV = XMFLOAT2(0, 0);
	vertices[1].Position = XMFLOAT3(0.5, -0.5, 0);
	vertices[1].UV = XMFLOAT2(1, 0);
	vertices[2].Position = XMFLOAT3(-0.5, 0.5, 0);
	vertices[2].UV = XMFLOAT2(0, 1);
	vertices[3].Position = XMFLOAT3(0.5, 0.5, 0);
	vertices[3].UV = XMFLOAT2(1, 1);

	unsigned int indices[] = { 3, 1, 2 ,0, 2 ,1};
	
	groundMesh = new Mesh(vertices, 4, indices, 6, device);

	skyBoxMesh = new Mesh(cubeMesh);

}

void Scene::CreateMaterial(ID3D11Device * device, ID3D11DeviceContext * context)
{
	snowManMaterial = new Material(device, context, kMaterialNormal, L"Assets/Textures/snowTexture.jpg", nullptr, nullptr, nullptr, nullptr, nullptr);
	snowManMaterial->LoadVertexShaders(device, context, L"VertexShader");
	snowManMaterial->LoadPixelShaders(device, context, L"PixelShader");

	snowManEyesMaterial = new Material(device, context, kMaterialNormal, L"Assets/Textures/parameter0.png", nullptr, nullptr, nullptr, nullptr, nullptr);
	snowManEyesMaterial->LoadVertexShaders(device, context, L"VertexShader");
	snowManEyesMaterial->LoadPixelShaders(device, context, L"PixelShader");

	groundMaterial = new Material(device, context, kMaterialNormal, L"Assets/Textures/snowTexture.jpg", nullptr, nullptr, nullptr, nullptr, nullptr);
	groundMaterial->LoadVertexShaders(device, context, L"VertexShader");
	groundMaterial->LoadPixelShaders(device, context, L"PixelShader");


	skyBoxMaterial = new Material(device, context, kMaterialCubemap, L"Assets/Textures/Texture1.dds", nullptr, nullptr, nullptr, nullptr, nullptr);
    skyBoxMaterial->LoadVertexShaders(device, context, L"SkyVS");
	skyBoxMaterial->LoadPixelShaders(device, context, L"SkyPS");

	//PBRmaterial = new Material(device, context, kMaterialPBR, nullptr, L"Assets/Textures/Building1_CFT_D.png", L"Assets/Textures/Building1_CFT_MT.png",
	//	L"Assets/Textures/Building1_CFT_R.png", L"Assets/Textures/Building_CFT_AO.png", L"Assets/Textures/Building1_CFT_N.png");
	//PBRmaterial->LoadVertexShaders(device, context, L"PBRVertexShader");
	//PBRmaterial->LoadPixelShaders(device, context, L"PBRPixelShader");

	//enviDiffuseMaterial = new Material(device, context, kMaterialCubemap, L"Assets/Textures/Texture1.dds", nullptr, nullptr, nullptr, nullptr, nullptr);
	//enviDiffuseMaterial->LoadVertexShaders(device, context, L"EnvironmentDiffuseVS");
	//enviDiffuseMaterial->LoadPixelShaders(device, context, L"EnvironmentDiffusePS");

	//prefilteredMaterial = new Material(device, context, kMaterialCubemap, L"Assets/Textures/Texture1.dds", nullptr, nullptr, nullptr, nullptr, nullptr);
	//prefilteredMaterial->LoadVertexShaders(device, context, L"EnvironmentSpecularPrefilteredVS");
	//prefilteredMaterial->LoadPixelShaders(device, context, L"EnvironmentSpecularPrefilteredPS");

	//brdfLUTMaterial = new Material();
	//brdfLUTMaterial->LoadVertexShaders(device, context, L"BRDFLookupTextureVS");
	//brdfLUTMaterial->LoadPixelShaders(device, context, L"BRDFLookupTexturePS");

	shadowMapMat = new Material();
	shadowMapMat->LoadVertexShaders(device, context, L"ShadowVS");
	
	
}

void Scene::CreateLights()
{
	pointLight0.pointLightColor = XMFLOAT4(1, 1, 1, 1);
	pointLight0.pointLightPosition = XMFLOAT3(5, 0, -4);

	pointLight1.pointLightColor = XMFLOAT4(1, 1, 1, 1);
	pointLight1.pointLightPosition = XMFLOAT3(0, 5, -4);

	pointLight2.pointLightColor = XMFLOAT4(1, 1, 1, 1);
	pointLight2.pointLightPosition = XMFLOAT3(5, 5, -4);

	pointLight3.pointLightColor = XMFLOAT4(1, 1, 1, 1);
	pointLight3.pointLightPosition = XMFLOAT3(0, 0, -4);

}

void Scene::CreateEntities()
{
	snowMan0Body = new Entity(sphereMesh, snowManMaterial, XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, 0));
	snowMan0Head = new Entity(sphereMesh, snowManMaterial, XMFLOAT3(0, 0.5, 0), XMFLOAT3(0, 0, 0), XMFLOAT3(0.8, 0.8, 0.8));
	snowMan0Eye0 = new Entity(sphereMesh, snowManMaterial, XMFLOAT3(-0.2, 0.1, -0.2), XMFLOAT3(0, 0, 0), XMFLOAT3(0.1, 0.1, 0.1));
	snowMan0Eye1 = new Entity(sphereMesh, snowManMaterial, XMFLOAT3(0.2, 0.1, -0.2), XMFLOAT3(0, 0, 0), XMFLOAT3(0.1, 0.1, 0.1));
	snowMan0Head->SetParent(snowMan0Body);
	snowMan0Eye0->SetParent(snowMan0Head);
	snowMan0Eye1->SetParent(snowMan0Head);

	skyBox = new Entity(skyBoxMesh, skyBoxMaterial, XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, 0));

	ground = new Entity(groundMesh, groundMaterial, XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, 0));

	// For shadow map
	entitiesOpaque.push_back(snowMan0Body);
	entitiesOpaque.push_back(snowMan0Head);
	entitiesOpaque.push_back(snowMan0Eye0);
	entitiesOpaque.push_back(snowMan0Eye1);
	entitiesOpaque.push_back(ground);

}

void Scene::init(ID3D11Device * device, ID3D11DeviceContext * context)
{
	CreateBasicGeometry(device);
	CreateMaterial(device, context);
	CreateEntities();
	CreateLights();
	
}
