#include "ShaderH.hlsli"

TexBlendPS main( TexBlendVS input)
{
    TexBlendPS result;
    float4 pos = float4(input.pos.xyz, 1.0f);
    //ワールド変換
    pos = mul(pos, World);
    //ビュー変換
    pos = mul(pos, View);
    //射影変換
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