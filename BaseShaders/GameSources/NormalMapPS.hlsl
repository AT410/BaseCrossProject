#include "ShaderH.hlsli"

Texture2D g_Texture : register(t0);
Texture2D g_NormalMapTex : register(t1);

SamplerState g_Samp0 : register(s0);
SamplerState g_NormSamp1 : register(s1);

float4 main(NormalMapPS input) : SV_TARGET
{
    //法線マップの色取得
    float4 normalColor;
    if (Activeflags.y == 1)
    {
        normalColor = g_NormalMapTex.Sample(g_NormSamp1, input.tex);
    }
    else
    {
        normalColor = float4(1, 1, 1, 1);
    }
    //ベクトル変換
    float4 normalVec = 2.0f * normalColor - 1.0f;
    //ノーマライズ
    normalVec = normalize(normalVec);

    //ライトの向きと法線マップの法線とで明度算出
    float4 bright = dot(input.lightTargetDir, normalVec);
    bright = max(0.0f, bright);//マイナス値は0.0ｆに補正

    //テクスチャ読込
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