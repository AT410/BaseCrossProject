#include "ShaderH.hlsli"

WaterPS main( WaterVS input)
{
    WaterPS result;
    float4 pos = float4(input.pos.xyz, 1.0f);
    //ワールド変換
    pos = mul(pos, World);
    //ビュー変換
    pos = mul(pos, View);
    //射影変換
    pos = mul(pos, Projection);
    //ピクセルシェーダ＞
    result.pos = pos;
    //ノーマライズ
    result.norm = mul(input.norm, (float3x3) World);
    result.norm = normalize(result.norm);
    //UV
    result.tex = input.tex;
    return result;
}