#include "ShaderH.hlsli"

TexBlendPS main( TexBlendVS input)
{
    TexBlendPS result;
    float4 pos = float4(input.pos.xyz, 1.0f);
    //���[���h�ϊ�
    pos = mul(pos, World);
    //�r���[�ϊ�
    pos = mul(pos, View);
    //�ˉe�ϊ�
    pos = mul(pos, Projection);
    //
    result.pos = pos;
    //
    result.norm = mul(input.norm, (float3x3)World);
    result.norm = normalize(result.norm);
    //
    result.uv = input.uv;
    return result;
}