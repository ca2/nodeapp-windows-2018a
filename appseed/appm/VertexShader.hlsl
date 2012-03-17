// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
Texture2D diffuseTexture : register(t0);
SamplerState linearSampler : register(s0);

cbuffer ConstantBufferNeverChanges : register(b0)
{
    float4 lightPosition[4];
    float4 lightColor;
}

cbuffer ConstantBufferChangeOnResize : register(b1)
{
    matrix projection;
};

cbuffer ConstantBufferChangesEveryFrame : register(b2)
{
    matrix view;
};

cbuffer ConstantBufferChangesEveryPrim : register (b3)
{
    matrix world;
    float4 meshColor;
    float4 diffuseColor;
    float4 specularColor;
    float  specularExponent;
};

//--------------------------------------------------------------------------------------
struct VertextShaderInput
{
    float4 position : POSITION;
    float4 normal : NORMAL;
    float2 textureUV : TEXCOORD0;
};

struct PixelShaderInput
{
    float4 position : SV_POSITION;
    float2 textureUV : TEXCOORD0;
    float3 vertexToEye : TEXCOORD1;
    float3 normal : TEXCOORD2;
    float3 VertexToLight0 : TEXCOORD3;
    float3 VertexToLight1 : TEXCOORD4;
    float3 VertexToLight2 : TEXCOORD5;
    float3 VertexToLight3 : TEXCOORD6;
};

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
PixelShaderInput main(VertextShaderInput input)
{
    PixelShaderInput output = (PixelShaderInput)0;

    output.position = mul(mul(mul(input.position, world), view), projection);
    output.textureUV = input.textureUV;

    // compute view space normal
    output.normal = normalize (mul(mul(input.normal.xyz, (float3x3)world), (float3x3)view));

    // Vertex pos in view space (normalize in pixel shader)
    output.vertexToEye = -mul(mul(input.position, world), view).xyz;

    // Compute view space vertex to light vectors (normalized)
    output.VertexToLight0 = normalize(mul(lightPosition[0], view ).xyz + output.vertexToEye);
    output.VertexToLight1 = normalize(mul(lightPosition[1], view ).xyz + output.vertexToEye);
    output.VertexToLight2 = normalize(mul(lightPosition[2], view ).xyz + output.vertexToEye);
    output.VertexToLight3 = normalize(mul(lightPosition[3], view ).xyz + output.vertexToEye);

    return output;
}
//--------------------------------------------------------------------------------------
