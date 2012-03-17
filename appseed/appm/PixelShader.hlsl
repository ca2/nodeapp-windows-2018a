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

struct PixelShaderInput
{
    float4 position : SV_POSITION;
    float2 textureUV : TEXCOORD0;
    float3 vertexToEye : TEXCOORD1;
    float3 normal : TEXCOORD2;
    float3 vertexToLight0 : TEXCOORD3;
    float3 vertexToLight1 : TEXCOORD4;
    float3 vertexToLight2 : TEXCOORD5;
    float3 vertexToLight3 : TEXCOORD6;
};

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 main(PixelShaderInput input) : SV_Target
{
    float diffuseLuminance =
        max(0.0f, dot(input.normal, input.vertexToLight0)) +
        max(0.0f, dot(input.normal, input.vertexToLight1)) +
        max(0.0f, dot(input.normal, input.vertexToLight2)) +
        max(0.0f, dot(input.normal, input.vertexToLight3));

    // Normalize view space vertex-to-eye
    input.vertexToEye = normalize(input.vertexToEye);

    float specularLuminance = 
        pow(max(0.0f, dot(input.normal, normalize(input.vertexToEye + input.vertexToLight0))), specularExponent) +
        pow(max(0.0f, dot(input.normal, normalize(input.vertexToEye + input.vertexToLight1))), specularExponent) +
        pow(max(0.0f, dot(input.normal, normalize(input.vertexToEye + input.vertexToLight2))), specularExponent) +
        pow(max(0.0f, dot(input.normal, normalize(input.vertexToEye + input.vertexToLight3))), specularExponent);

    float4 specular;
    specular = specularColor * specularLuminance * 0.5f;

   return diffuseTexture.Sample(linearSampler, input.textureUV) *  diffuseColor * diffuseLuminance * 0.5f + specular;
}
//--------------------------------------------------------------------------------------
