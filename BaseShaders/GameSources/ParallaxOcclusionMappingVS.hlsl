#include "ShaderH.hlsli"

POMapPS main( POMapVS input)
{
    POMapPS result;
    float4 pos = float4(input.pos.xyz, 1.0f);
    float4 Eyep = float4(CameraEyePos.xyz, 1.0f);
    //���[���h�ϊ�
    pos = mul(pos, World);
    Eyep = mul(Eyep, World);
    result.objectViewDir = pos - Eyep.xyz;
    result.objectPos = pos;
    //�r���[�ϊ�
    pos = mul(pos, View);
    //�ˉe�ϊ�
    pos = mul(pos, Projection);

    //
    result.position = pos;

    result.normal = mul(input.norm, (float3x3) World);
    result.normal = normalize(result.normal);
    //UV
    result.uv = input.tex;

    //�����x�N�g�����v�Z
    float4 ip = float4(input.pos.xyz, 1.0f);
    float4 ep = float4(CameraEyePos.xyz, 1.0f);
    //���[���h�ϊ�
    ip = mul(ip, World);
    ep = mul(ep, World);
    float3 Eye = normalize(ep - ip).xyz;

    //�����x�N�g���𒸓_���W�n�ɕϊ�����
    //result.objectViewDir.x = dot(Eye, input.tangent);
    //result.objectViewDir.y = dot(Eye, input.binormal);
    //result.objectViewDir.z = dot(Eye, input.norm);
    //result.objectViewDir = normalize(result.objectViewDir);
    //result.objectViewDir = Eye;

    return result;
}