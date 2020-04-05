#include "ShaderH.hlsli"

NormalMapPS main( NormalMapVS input)
{
    NormalMapPS result;
    float4 pos = float4(input.pos.xyz, 1.0f);
    pos = mul(pos, World);
    pos = mul(pos, View);
    pos = mul(pos, Projection);
    //ピクセルシェーダ＞
    result.pos = pos;
    //ノーマライズ
    result.norm = mul(input.norm, (float3x3) World);
    result.norm = normalize(result.norm);
    //UV
    result.tex = input.tex;

    //接空間行列の逆行列を算出
    float4x4 mat =
    {
        float4(input.tangent, 0.0f),
        float4(input.binorm, 0.0f),
        float4(input.norm, 0.0f),
        float4(0.0f, 0.0f, 0.0f, 0.0f)
    };
    mat = transpose(mat);
    result.lightTargetDir = mul(-LightDir, mat);
    return result;
}