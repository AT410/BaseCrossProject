#include "ShaderH.hlsli"

Texture2D g_Texture : register(t0);
Texture2D g_NormalMapTex : register(t1);

SamplerState g_Samp0 : register(s0);
SamplerState g_NormSamp1 : register(s1);

float4 main(NormalMapPS input) : SV_TARGET
{
    //�@���}�b�v�̐F�擾
    float4 normalColor;
    if (Activeflags.y == 1)
    {
        normalColor = g_NormalMapTex.Sample(g_NormSamp1, input.tex);
    }
    else
    {
        normalColor = float4(1, 1, 1, 1);
    }
    //�x�N�g���ϊ�
    float4 normalVec = 2.0f * normalColor - 1.0f;
    //�m�[�}���C�Y
    normalVec = normalize(normalVec);

    //���C�g�̌����Ɩ@���}�b�v�̖@���ƂŖ��x�Z�o
    float4 bright = dot(input.lightTargetDir, normalVec);
    bright = max(0.0f, bright);//�}�C�i�X�l��0.0���ɕ␳

    //�e�N�X�`���Ǎ�
    float4 texColor;
    if (Activeflags.x == 1)
    {
        texColor = g_Texture.Sample(g_Samp0, input.tex);
    }
    else
    {
        texColor = float4(1, 1, 1, 1);
    }
    float4 result = float4(bright.xyz * texColor.xyz, 1.0f);

    return result;

}