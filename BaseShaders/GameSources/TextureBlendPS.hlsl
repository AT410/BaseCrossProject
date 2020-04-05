#include "ShaderH.hlsli"

Texture2D g_MainTex : register(t0);
Texture2D g_SubTex : register(t1);
Texture2D g_MaskTex : register(t2);

SamplerState s_MainSamp : register(s0);
SamplerState s_SubSamp : register(s1);
SamplerState s_MaskSamp : register(s2);

float4 main(TexBlendPS input) : SV_TARGET
{
    //TexBlend
    float4 Mc = g_MainTex.Sample(s_MainSamp, input.uv);
    float4 Sc = g_SubTex.Sample(s_SubSamp, input.uv);
    float4 Mkc = g_MaskTex.Sample(s_MaskSamp, input.uv);
    
    float4 result = lerp(Mc, Sc, Mkc);
	return result;
}