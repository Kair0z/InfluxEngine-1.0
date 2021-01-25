
struct MVP { matrix MVP; };
ConstantBuffer<MVP> cbModelViewProjection : register(b0);

struct VS_Input
{
	float3 Position : POSITION;
	float3 Color : COLOR;
};

struct VS_Output
{
	float4 Color : COLOR;
	float4 Position : SV_POSITION;
};

VS_Output VS_Main(VS_Input input)
{
	VS_Output output = (VS_Output)0;

	output.Position = mul(cbModelViewProjection.MVP, float4(input.Position, 1));
	output.Color = float4(input.Color, 1.0f);

	return output;
}