
struct PS_Input
{
	float4 Color : COLOR;
};

float4 PS_Main(PS_Input input) : SV_TARGET
{
	return float4(1.0f, 0.0f, 0.0f, 1.0f);
}