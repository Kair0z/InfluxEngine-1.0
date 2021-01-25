
struct PS_Input
{
	float4 Color : COLOR;
};

float4 PS_Main(PS_Input input) : SV_TARGET
{
	return input.Color;
}