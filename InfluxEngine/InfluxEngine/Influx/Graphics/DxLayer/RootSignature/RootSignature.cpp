#include "pch.h"
#include "RootSignature.h"
#include "Influx/Graphics/DxLayer/DxLayer.h"

namespace Influx
{
	sPtr<RootSignature> RootSignature::Create(comPtr<ID3D12Device> device)
	{
		sPtr<RootSignature> rootSignature(new RootSignature());

		// [TODO: adding root parameters]
		CD3DX12_ROOT_PARAMETER1 rootParams[1];
		rootParams[0].InitAsConstantBufferView(0);
		rootParams[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

		auto rootsigDesc = DxLayer::CreateRootSignatureDesc(rootParams, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);
		rootSignature->mpDxRootSignature = DxLayer::CreateRootSignature(device.Get(), &rootsigDesc);

		return rootSignature;
	}
}

