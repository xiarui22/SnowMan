#include "Scene.h"

Scene::Scene()
{
}


Scene::~Scene()
{
	/*for (int i = 0; i < 5; i++)
		for (int j = 0; j < 5; j++)
			delete spheres[i][j];*/

	delete snowMan0;

	delete quads[0];
	delete quads[1];

	delete cube;
	delete PBRmaterial;
	delete skyBoxMaterial;
	delete shadowMap;
	delete skyBoxMesh;
	delete skyBox;
	delete enviDiffuseMaterial;
	delete prefilteredMaterial;
	delete cubeForCaptureEnviDiffuse;
	delete cubeForCapturePrefiltered;
	delete plane2dMesh;
	delete brdfLUT;
	delete brdfLUTMaterial;
}


// --------------------------------------------------------
// Creates the geometry we're going to draw - a single triangle for now
// --------------------------------------------------------
void Scene::CreateBasicGeometry(ID3D11Device * device)
{
	// Create the VERTEX BUFFER description -----------------------------------
	// - The description is created on the stack because we only need
	//    it to create the buffer.  The description is then useless.
	sphere = new Mesh("Assets/Models/sphere.obj", device);
	//mesh = new Mesh("Assets/Models/building.obj", device);
	cube = new Mesh("Assets/Models/cube.obj", device);

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
	

	plane2dMesh = new Mesh(vertices, 4, indices, 6, device);
}

void Scene::CreateMaterial(ID3D11Device * device, ID3D11DeviceContext * context)
{
	skyBoxMaterial = new Material(device, context, kMaterialCubemap, L"Assets/Textures/Texture1.dds", nullptr, nullptr, nullptr, nullptr, nullptr);
    skyBoxMaterial->LoadVertexShaders(device, context, L"SkyVS");
	skyBoxMaterial->LoadPixelShaders(device, context, L"SkyPS");

	PBRmaterial = new Material(device, context, kMaterialPBR, nullptr, L"Assets/Textures/Building1_CFT_D.png", L"Assets/Textures/Building1_CFT_MT.png",
		L"Assets/Textures/Building1_CFT_R.png", L"Assets/Textures/Building_CFT_AO.png", L"Assets/Textures/Building1_CFT_N.png");
	PBRmaterial->LoadVertexShaders(device, context, L"PBRVertexShader");
	PBRmaterial->LoadPixelShaders(device, context, L"PBRPixelShader");

	enviDiffuseMaterial = new Material(device, context, kMaterialCubemap, L"Assets/Textures/Texture1.dds", nullptr, nullptr, nullptr, nullptr, nullptr);
	enviDiffuseMaterial->LoadVertexShaders(device, context, L"EnvironmentDiffuseVS");
	enviDiffuseMaterial->LoadPixelShaders(device, context, L"EnvironmentDiffusePS");

	prefilteredMaterial = new Material(device, context, kMaterialCubemap, L"Assets/Textures/Texture1.dds", nullptr, nullptr, nullptr, nullptr, nullptr);
	prefilteredMaterial->LoadVertexShaders(device, context, L"EnvironmentSpecularPrefilteredVS");
	prefilteredMaterial->LoadPixelShaders(device, context, L"EnvironmentSpecularPrefilteredPS");

	brdfLUTMaterial = new Material();
	brdfLUTMaterial->LoadVertexShaders(device, context, L"BRDFLookupTextureVS");
	brdfLUTMaterial->LoadPixelShaders(device, context, L"BRDFLookupTexturePS");

	shadowMap = new Material();
	shadowMap->LoadVertexShaders(device, context, L"ShadowVS");
	
	
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

	albedo = XMFLOAT3(1,0,0);
	for (int i = 0; i < 5; i++) {
		metallic[i] = 0.1+0.2*i;
		roughness[i] = 0.1+0.2*i;
	}
	ao = 0.5f;
}

void Scene::CreateEntities()
{
	snowMan0Mesh = new Mesh();
	skyBoxMesh = cube;

	quads[0] = new Entity(cube, PBRmaterial);
	quads[1] = new Entity(cube, PBRmaterial);

	skyBox = new Entity(skyBoxMesh, skyBoxMaterial);
	cubeForCaptureEnviDiffuse = new Entity(skyBoxMesh, enviDiffuseMaterial);
	cubeForCapturePrefiltered = new Entity(skyBoxMesh, prefilteredMaterial);
	brdfLUT = new Entity(plane2dMesh, brdfLUTMaterial);

	// For shadow map
	

	entitiesOpaque.push_back(quads[0]);
	entitiesOpaque.push_back(quads[1]);
}

void Scene::init(ID3D11Device * device, ID3D11DeviceContext * context)
{
	CreateBasicGeometry(device);
	CreateMaterial(device, context);
	CreateEntities();
	CreateLights();
	
}
