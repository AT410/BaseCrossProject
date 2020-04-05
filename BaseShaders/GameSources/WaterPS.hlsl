#include "ShaderH.hlsli"

Texture2D g_texture : register(t0);

SamplerState Samp0 = sampler_state
{
    AddressU = CLAMP;
    AddressV = CLAMP;
};

float4 main(WaterPS input) : SV_TARGET
{
    float4 result;
    result = g_texture.Sample(Samp0, input.tex - g_Paramf.xy);
    float4 test = float4(1, 0, 0, 1);
    result.a = g_Paramf.z;
    return ConvertToGray(result.rgba);
}