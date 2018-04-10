
// Struct representing the data we expect to receive from earlier pipeline stages
// - Should match the output of our corresponding vertex shader
// - The name of the struct itself is unimportant
// - The variable names don't have to match other shaders (just the semantics)
// - Each variable must have a semantic, which defines its usage
struct VertexToPixel
{
	// Data type
	//  |
	//  |   Name          Semantic
	//  |    |                |
	//  v    v                v
	float4 position		: SV_POSITION;
	float3 normal       : NORMAL;
	float3 worldPos		: WORLDPOS;
	float2 uv           : TEXCOORD;
	float4 posForShadow : POSITION1;
};

// --------------------------------------------------------
// The entry point (main method) for our pixel shader
// 
// - Input is the data coming down the pipeline (defined by the struct)
// - Output is a single color (float4)
// - Has a special semantic (SV_TARGET), which means 
//    "put the output of this into the current render target"
// - Named "main" because that's the default the shader compiler looks for
// --------------------------------------------------------



struct DirectionalLight {
	float4 AmbientColor;
	float4 DiffuseColor;
	float3 Direction;
};

struct PointLight {
	float4 pointLightColor;
	float3 pointLightPosition;
	float i;
};

cbuffer externalData : register(b0)
{

	PointLight pl0;
	PointLight pl1;
	PointLight pl2;
	PointLight pl3;

	float3 camPos;
};

Texture2D diffuseTexture  : register(t0);
Texture2D shadowMap		: register(t1);
SamplerState basicSampler : register(s0); 
SamplerState shadowSampler : register(s2);

float4 main(VertexToPixel input) : SV_TARGET
{
	// Just return the input color
	// - This color (like most values passing through the rasterizer) is 
	//   interpolated for each pixel between the corresponding vertices 
	//   of the triangle we're rendering
	float4 surfaceColor = diffuseTexture.Sample(basicSampler, input.uv);

	input.normal = normalize(input.normal);

////directional light 
//float lightAmountDL = saturate(dot(input.normal, normalize(-sunLight.Direction)));
//float4 dlight = sunLight.DiffuseColor* lightAmountDL*surfaceColor + sunLight.AmbientColor;

//point light
PointLight pointLight[4];
pointLight[0] = pl0;
pointLight[1] = pl1;
pointLight[2] = pl2;
pointLight[3] = pl3;
float4 plight = float4(0, 0, 0, 1);
for (int i = 0; i < 2; i++) {
	float3 dirToPointLight = normalize(pointLight[i].pointLightPosition - input.worldPos);
    float lightAmountPL = saturate(dot(input.normal, dirToPointLight));
    plight += pointLight[i].pointLightColor * lightAmountPL*surfaceColor;
}

// Shadow mapping
// Percentage closer filtering

float3 color = float3(0, 0, 0);
float width, height;
shadowMap.GetDimensions(width, height);
float2 textureSize = float2(width, height);
float2 texelSize = 1 / textureSize;
float bias = 0.001f;         //  Set the bias value for fixing the floating point precision issues.
float3 projCoords = input.posForShadow.xyz / input.posForShadow.w;
projCoords.xy = projCoords.xy * 0.5 + 0.5;
projCoords.y = 1 - projCoords.y;


if ((saturate(projCoords.x) == projCoords.x) && (saturate(projCoords.y) == projCoords.y)) {

	float currentDepth = projCoords.z;
	currentDepth -= bias;
	float shadow = 0;
	for (int i = -3; i < 4; i++) {
		for (int j = -3; j < 4; j++) {
			float cloestDepth = shadowMap.Sample(shadowSampler, projCoords.xy + float2(i, j) * texelSize).r;
			shadow += currentDepth > cloestDepth ? 1 : 0;
		}
	}
	shadow /= 25;
	color = plight.xyz * (1 - shadow);
}
else {
	color = plight.xyz;
}
	return float4(color, 1);

}