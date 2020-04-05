#include "ShaderH.hlsli"

Texture2D g_Texture : register(t0);

SamplerState Samp0 : register(s0);

float4 main(POMapPS input) : SV_TARGET
{
    float3 rayDir = normalize(input.objectViewDir);
	// ���X��UV�Ɏ������������炵������
    float2 uv = input.uv/* + rayDir.xz * (-0.5f /rayDir.y)*/;
    return g_Texture.Sample(Samp0, uv);
}