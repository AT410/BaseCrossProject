#include "ShaderH.hlsli"

POMapPS main( POMapVS input)
{
    POMapPS result;
    float4 pos = float4(input.pos.xyz, 1.0f);
    float4 Eyep = float4(CameraEyePos.xyz, 1.0f);
    //ワールド変換
    pos = mul(pos, World);
    Eyep = mul(Eyep, World);
    result.objectViewDir = pos - Eyep.xyz;
    result.objectPos = pos;
    //ビュー変換
    pos = mul(pos, View);
    //射影変換
    pos = mul(pos, Projection);

    //
    result.position = pos;

    result.normal = mul(input.norm, (float3x3) World);
    result.normal = normalize(result.normal);
    //UV
    result.uv = input.tex;

    //視線ベクトルを計算
    float4 ip = float4(input.pos.xyz, 1.0f);
    float4 ep = float4(CameraEyePos.xyz, 1.0f);
    //ワールド変換
    ip = mul(ip, World);
    ep = mul(ep, World);
    float3 Eye = normalize(ep - ip).xyz;

    //視線ベクトルを頂点座標系に変換する
    //result.objectViewDir.x = dot(Eye, input.tangent);
    //result.objectViewDir.y = dot(Eye, input.binormal);
    //result.objectViewDir.z = dot(Eye, input.norm);
    //result.objectViewDir = normalize(result.objectViewDir);
    //result.objectViewDir = Eye;

    return result;
}