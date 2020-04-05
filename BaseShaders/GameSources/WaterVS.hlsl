#include "ShaderH.hlsli"

WaterPS main( WaterVS input)
{
    WaterPS result;
    float4 pos = float4(input.pos.xyz, 1.0f);
    //���[���h�ϊ�
    pos = mul(pos, World);
    //�r���[�ϊ�
    pos = mul(pos, View);
    //�ˉe�ϊ�
    pos = mul(pos, Projection);
    //�s�N�Z���V�F�[�_��
    result.pos = pos;
    //�m�[�}���C�Y
    result.norm = mul(input.norm, (float3x3) World);
    result.norm = normalize(result.norm);
    //UV
    result.tex = input.tex;
    return result;
}