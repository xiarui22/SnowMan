#include "Game.h"
#include "Vertex.h"
#include <iostream>

// For the DirectX Math library
using namespace DirectX;
using namespace std;
// --------------------------------------------------------
// Constructor
//
// DXCore (base class) constructor will set up underlying fields.
// DirectX itself, and our window, are not ready yet!
//
// hInstance - the application's OS-level handle (unique ID)
// --------------------------------------------------------
Game::Game(HINSTANCE hInstance)
	: DXCore( 
		hInstance,		   // The application's handle
		"DirectX Game",	   // Text for the window's title bar
		1280,			   // Width of the window's client area
		720,			   // Height of the window's client area
		true)			   // Show extra stats (fps) in title bar?
{
	// Initialize fields
//	vertexBuffer = 0;
//	indexBuffer = 0;


#if defined(DEBUG) || defined(_DEBUG)
	// Do we want a console window?  Probably only in debug mode
	CreateConsoleWindow(500, 120, 32, 120);
	printf("Console window created successfully.  Feel free to printf() here.");
#endif
}

// --------------------------------------------------------
// Destructor - Clean up anything our game has created:
//  - Release all DirectX objects created here
//  - Delete any objects to prevent memory leaks
// --------------------------------------------------------
Game::~Game()
{
	// Release any (and all!) DirectX objects
	// we've made in the Game class
	delete camera;
	delete scene;
//	delete environmentDiffuseCapturer;
//	delete prefilteredCapturer;
//	delete brdfLUTCapturer;
	delete shadowMapRender;
}

// --------------------------------------------------------
// Called once per program, after DirectX and the window
// are initialized but before the game loop.
// --------------------------------------------------------
void Game::Init()
{
	// Helper methods for loading shaders, creating some basic
	// geometry to draw and some simple camera matrices.
	//  - You'll be expanding and/or replacing these later
	scene = new Scene();
	scene->init(device, context);
	CreateMatrices();
    context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);	

//	PreComputeCubemaps();
//	PreComputerBrdfLUT();
	
	shadowMapRender = new ShadowMapRenderer();
	shadowMapRender->Init(device, context,2048,2048);
	// Tell the input assembler stage of the pipeline what kind of
	// geometric primitives (points, lines or triangles) we want to draw.  
	// Essentially: "What kind of shape should the GPU draw with our data?"
}

// --------------------------------------------------------
// Initializes the matrices necessary to represent our geometry's 
// transformations and our 3D camera
// --------------------------------------------------------
void Game::CreateMatrices()
{
	camera = new Camera(XMFLOAT3(2.0f, 2.0f, -8.0f), XMFLOAT3(0.0f, 0.0f, 1.0f));
	//camera = new Camera(XMFLOAT3(0,2,-8), XMFLOAT3(0.0f, 0.0f, 1.0f));
	camera->UpdateProjection(width, height);
}



//void Game::PreComputeCubemaps()
//{
//	environmentDiffuseCapturer = new CaptureIrradiance();
//	prefilteredCapturer = new CaptureIrradiance();
//
//	if (environmentDiffuseCapturer->EnvironmentDiffuseMapExists(device));
//	else {
//		environmentDiffuseCapturer->Init(device,context, 512, 512);
//		environmentDiffuseCapturer->RenderEnvironmentDiffuseMap(context, scene->cubeForCaptureEnviDiffuse);
//		if (environmentDiffuseCapturer->SaveEnvironmentDiffuseMap(device, context));
//		else cout << "saved diffuse irradiance map failed" << endl;
//
//		context->OMSetRenderTargets(1, &backBufferRTV, depthStencilView);
//		D3D11_VIEWPORT viewport = {};
//		viewport.TopLeftX = 0;
//		viewport.TopLeftY = 0;
//		viewport.Width = (float)width;
//		viewport.Height = (float)height;
//		viewport.MinDepth = 0.0f;
//		viewport.MaxDepth = 1.0f;
//		context->RSSetViewports(1, &viewport);
//	}
//	if (prefilteredCapturer->PrefilteredMapExists(device));
//	else {
//		prefilteredCapturer->Init(device,context, 512, 512);
//		prefilteredCapturer->RenderPrefilteredMap(context, scene->cubeForCapturePrefiltered);
//		if (prefilteredCapturer->SavePrefilteredMap(device, context));
//		else cout << "saved prefiltered map failed" << endl;
//
//		context->OMSetRenderTargets(1, &backBufferRTV, depthStencilView);
//		D3D11_VIEWPORT viewport = {};
//		viewport.TopLeftX = 0;
//		viewport.TopLeftY = 0;
//		viewport.Width = (float)width;
//		viewport.Height = (float)height;
//		viewport.MinDepth = 0.0f;
//		viewport.MaxDepth = 1.0f;
//		context->RSSetViewports(1, &viewport);
//	}
//	
//	
//}
//
//void Game::PreComputerBrdfLUT()
//{
//	brdfLUTCapturer = new CaptureTexture2d();
//
//    if (brdfLUTCapturer->BrdfLUTExists(device, context));
//	else {
//		brdfLUTCapturer->Init(device, context, 512, 512);
//		brdfLUTCapturer->RenderBrdfLUT(context, scene->brdfLUT);
//		if (brdfLUTCapturer->SaveBrdfLUT(device, context));
//		else cout << "saved BrdfLUT failed" << endl;
//
//		context->OMSetRenderTargets(1, &backBufferRTV, depthStencilView);
//		D3D11_VIEWPORT viewport = {};
//		viewport.TopLeftX = 0;
//		viewport.TopLeftY = 0;
//		viewport.Width = (float)width;
//		viewport.Height = (float)height;
//		viewport.MinDepth = 0.0f;
//		viewport.MaxDepth = 1.0f;
//		context->RSSetViewports(1, &viewport);
//	}
//
//}



// --------------------------------------------------------
// Handle resizing DirectX "stuff" to match the new window size.
// For instance, updating our projection matrix's aspect ratio.
// --------------------------------------------------------
void Game::OnResize()
{
	// Handle base-level DX resize stuff
	DXCore::OnResize();
	camera->UpdateProjection(width, height);
}

// --------------------------------------------------------
// Update your game here - user input, move objects, AI, etc.
// --------------------------------------------------------
void Game::Update(float deltaTime, float totalTime)
{
	// Quit if the escape key is pressed
	if (GetAsyncKeyState(VK_ESCAPE))
		Quit();

	// Get on the car
	if (GetAsyncKeyState('Q') & 0x8000) {  
		camera->onCar = true;
	}
	if (GetAsyncKeyState('E') & 0x8000) {
		camera->onCar = false;
	}
		
	scene->snowMan0Body->Update();
	scene->snowMan0Head->Update();
	scene->snowMan0Eye0->Update();
	scene->snowMan0Eye1->Update();

	float carx = sin(totalTime) * 2.0f;
	float carz = cos(totalTime) * 2.0f;;
	scene->car->setTranslation(carx, 0, carz);
	scene->car->Update();

	scene->ground->Update();
	scene->skyBox->Update();

	if (camera->onCar) camera->SetParent(scene->car);

	camera->Update(deltaTime);
}

// --------------------------------------------------------
// Clear the screen, redraw everything, present to the user
// --------------------------------------------------------
void Game::Draw(float deltaTime, float totalTime)
{
	
	shadowMapRender->RenderDepthMap(context, scene);

	context->OMSetRenderTargets(1, &backBufferRTV, depthStencilView);
	D3D11_VIEWPORT viewport = {};
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = (float)width;
	viewport.Height = (float)height;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	context->RSSetViewports(1, &viewport);
	context->RSSetState(0);

	// Background color (Cornflower Blue in this case) for clearing
	const float color[4] = {0.4f, 0.6f, 0.75f, 0.0f};
	//const float color[4] = { 1, 1, 0, 0.0f };

	//// Clear the render target and depth buffer (erases what's on the screen)
	////  - Do this ONCE PER FRAME
	////  - At the beginning of Draw (before drawing *anything*)
	context->ClearRenderTargetView(backBufferRTV, color);
	context->ClearDepthStencilView(
		depthStencilView, 
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f,
		0);

	// Set buffers in the input assembler
	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	for (Entity *q : scene->entitiesOpaque) {

		q->setWorld();

		q->GetMaterial()->SetVertexShaderMatrix(q->getWorld(), camera->GetView(), camera->GetProjection());
		q->GetMaterial()->GetvertexShader()->SetMatrix4x4("lightView", shadowMapRender->lightViewMatrix);
		q->GetMaterial()->GetvertexShader()->SetMatrix4x4("lightProjection", shadowMapRender->lightProjectionMatrix);
		q->GetMaterial()->GetvertexShader()->CopyAllBufferData();

		q->GetMaterial()->GetvertexShader()->SetShader();

		q->GetMaterial()->GetpixelShader()->SetData("pl0", &scene->pointLight0, sizeof(PointLight));
		q->GetMaterial()->GetpixelShader()->SetData("pl1", &scene->pointLight1, sizeof(PointLight));
		q->GetMaterial()->GetpixelShader()->SetData("pl2", &scene->pointLight2, sizeof(PointLight));
		q->GetMaterial()->GetpixelShader()->SetData("pl3", &scene->pointLight3, sizeof(PointLight));

		q->GetMaterial()->GetpixelShader()->SetFloat3("camPos", camera->GetCamPos());
		//q->GetMaterial()->SetEnvironmentDiffuseSrvForPBR(environmentDiffuseCapturer->GetShaderResourceView());
		//q->GetMaterial()->SetPrefilterMapSrvForPBR(prefilteredCapturer->GetShaderResourceView());
		//q->GetMaterial()->SetBRDFLUTSrvForPBR(brdfLUTCapturer->GetShaderResourceView());
		//scene->quads[i]->getMaterial()->SetShadowStuff(shadowMapRender->GetShaderResourceView(), shadowMapRender->GetShadowSampler());
		q->GetMaterial()->SetShadowStuff(shadowMapRender->GetShaderResourceView(), shadowMapRender->GetShadowSampler());
		//q->GetMaterial()->SetPBRPixelShaderSrv();
		q->GetMaterial()->SetPixelShaderSrv();

		ID3D11Buffer* vertexBuffer = q->GetMesh()->GetVertexBuffer();

		context->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);

		context->IASetIndexBuffer(q->GetMesh()->GetIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);

		context->DrawIndexed(
			q->GetMesh()->GetIndexCount(),     // The number of indices to use (we could draw a subset if we wanted)
			0,     // Offset to the first index we want to use
			0);    // Offset to add to each index when looking up vertices
	}


		

	    

		//
		//

		////draw skybox

		ID3D11Buffer* vertexBuffer = scene->skyBox->GetMesh()->GetVertexBuffer();

		context->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
		context->IASetIndexBuffer(scene->skyBox->GetMesh()->GetIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);

		scene->skyBox->GetMaterial()->GetvertexShader()->SetMatrix4x4("view", camera->GetView());
		scene->skyBox->GetMaterial()->GetvertexShader()->SetMatrix4x4("projection", camera->GetProjection());
		scene->skyBox->GetMaterial()->GetvertexShader()->CopyAllBufferData();
		scene->skyBox->GetMaterial()->GetvertexShader()->SetShader();

		/*scene->skyBox->getMaterial()->GetpixelShader()->SetShaderResourceView("environmentMap", prefilteredCapturer->GetShaderResourceView());
		scene->skyBox->getMaterial()->GetpixelShader()->SetSamplerState("basicSampler", scene->skyBox->getMaterial()->GetSamplerState());
		scene->skyBox->getMaterial()->GetpixelShader()->CopyAllBufferData();
		scene->skyBox->getMaterial()->GetpixelShader()->SetShader();*/
		//scene->skyBox->getMaterial()->GetpixelShader()->SetFloat("roughness", 0.8);
		scene->skyBox->GetMaterial()->SetSkyPixelShaderSrv();
		
		context->RSSetState(scene->skyBox->GetMaterial()->GetRastState());
		context->OMSetDepthStencilState(scene->skyBox->GetMaterial()->GetDepthStencilState(), 0);
		context->DrawIndexed(scene->skyBox->GetMesh()->GetIndexCount(), 0, 0);

		// Reset the render states we've changed
		context->RSSetState(0);
		context->OMSetDepthStencilState(0, 0);

		scene->skyBox->GetMaterial()->GetpixelShader()->SetShaderResourceView("environmentMap", 0);
	// Present the back buffer to the user
	//  - Puts the final frame we're drawing into the window so the user can see it
	//  - Do this exactly ONCE PER FRAME (always at the very end of the frame)
	swapChain->Present(0, 0);
}


#pragma region Mouse Input

// --------------------------------------------------------
// Helper method for mouse clicking.  We get this information
// from the OS-level messages anyway, so these helpers have
// been created to provide basic mouse input if you want it.
// --------------------------------------------------------
void Game::OnMouseDown(WPARAM buttonState, int x, int y)
{
	// Add any custom code here...
	/*if (buttonState & 0x0001) {
		
		prevMousePos.x = x;
		prevMousePos.y = y;
		cout << prevMousePos.y;
	}*/




	// Save the previous mouse position, so we have it for the future
	//prevMousePos.x = x;
	//prevMousePos.y = y;

	// Caputure the mouse so we keep getting mouse move
	// events even if the mouse leaves the window.  we'll be
	// releasing the capture once a mouse button is released
	SetCapture(hWnd);
}

// --------------------------------------------------------
// Helper method for mouse release
// --------------------------------------------------------
void Game::OnMouseUp(WPARAM buttonState, int x, int y)
{
	// Add any custom code here...
	

		
	
	// We don't care about the tracking the cursor outside
	// the window anymore (we're not dragging if the mouse is up)
	ReleaseCapture();
}

// --------------------------------------------------------
// Helper method for mouse movement.  We only get this message
// if the mouse is currently over the window, or if we're 
// currently capturing the mouse.
// --------------------------------------------------------
void Game::OnMouseMove(WPARAM buttonState, int x, int y)
{
	// Add any custom code here...
	if (buttonState & 0x0001)
    camera->SetRotation( (y - prevMousePos.y)*0.005,(x - prevMousePos.x)*0.005);
	//cc->setRotation(x - prevMousePos.x, y - prevMousePos.y);
	//cout << cc->rotationX;

	// Save the previous mouse position, so we have it for the future
	prevMousePos.x = x;
	prevMousePos.y = y;
	

}

// --------------------------------------------------------
// Helper method for mouse wheel scrolling.  
// WheelDelta may be positive or negative, depending 
// on the direction of the scroll
// --------------------------------------------------------
void Game::OnMouseWheel(float wheelDelta, int x, int y)
{
	// Add any custom code here...
}
#pragma endregion