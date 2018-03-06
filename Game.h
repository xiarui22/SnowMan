#pragma once

#include "DXCore.h"
#include <DirectXMath.h>

#include "Camera.h"
#include "Scene.h"
#include "CaptureIrradiance.h"
#include "CaptureTexture2d.h"
#include "ShadowMapRenderer.h"

class Game 
	: public DXCore
{

public:
	Game(HINSTANCE hInstance);
	~Game();

	// Overridden setup and game loop methods, which
	// will be called automatically
	void Init();
	void OnResize();
	void Update(float deltaTime, float totalTime);
	void Draw(float deltaTime, float totalTime);

	// Overridden mouse input helper methods
	void OnMouseDown (WPARAM buttonState, int x, int y);
	void OnMouseUp	 (WPARAM buttonState, int x, int y);
	void OnMouseMove (WPARAM buttonState, int x, int y);
	void OnMouseWheel(float wheelDelta,   int x, int y);
private:

	void CreateMatrices();
	void PreComputeCubemaps();
	void PreComputerBrdfLUT();
	
	// Keeps track of the old mouse position.  Useful for 
	// determining how far the mouse moved in a single frame.
	POINT prevMousePos;
	
	Camera * camera;
	Scene * scene;
	// For captures
	CaptureIrradiance * environmentDiffuseCapturer;
	CaptureIrradiance * prefilteredCapturer;

	CaptureTexture2d * brdfLUTCapturer;
	ShadowMapRenderer * shadowMapRender;
};

