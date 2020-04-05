
struct WaterVS
{
    float4 pos : SV_POSITION;
    float3 norm : NORMAL;
    float2 tex : TEXCOORD;
};

struct WaterPS
{
    float4 pos : SV_POSITION;
    float3 norm : NORMAL;
    float2 tex : TEXCOORD;
};

//
struct NormalMapVS
{
    float4 pos : SV_POSITION;
    float3 norm : NORMAL;
    float2 tex : TEXCOORD;
    float3 tangent : TANGENT;
    float3 binorm : BINORMAL;
};

struct NormalMapPS
{
    float4 pos : SV_POSITION;
    float3 norm : NORMAL;
    float2 tex : TEXCOORD;
    float4 lightTargetDir : lightTargetDir;
};

//視差マッピング構造体
struct POMapVS
{
    float4 pos : SV_POSITION;
    float3 norm : NORMAL;
    float2 tex : TEXCOORD;
    float3 tangent : TANGENT;//接線ベクトル
    float3 binormal : BINORMAL;//従法線
};

struct POMapPS
{
    float4 position : SV_POSITION;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD0;
    float3 objectViewDir : TEXCOORD1;
    float3 objectPos : TEXCOORD2;
};

//
struct TexBlendVS
{
    float4 pos : SV_POSITION;
    float3 norm : NORMAL;
    float2 uv : TEXCOORD;
};

struct TexBlendPS
{
    float4 pos : SV_POSITION;
    float3 norm : NORMAL;
    float2 uv : TEXCOORD;
};

cbuffer ConstantBuffer : register(b0)
{
    float4x4 World : packoffset(c0);
    float4x4 View : packoffset(c4);
    float4x4 Projection : packoffset(c8);
    float4 Emissive : packoffset(c12);
    float4 Diffuse : packoffset(c13);
    float4 LightDir : packoffset(c14);
    float4 CameraEyePos : packoffset(c16);
    uint4 Activeflags : packoffset(c17); //フラグ
}

//汎用パラメータ
cbuffer ParamBuffer : register(b1)
{
    float4 g_Paramf : packoffset(c0);
}

//汎用関数
//グロースケール変換
float4 ConvertToGray(float4 col)
{
    const float3 lumcoeff = { 0.3, 0.59, 0.11};
    float4 result = col;
    result.rgb = dot(col.rgb, lumcoeff);
    return result;
}