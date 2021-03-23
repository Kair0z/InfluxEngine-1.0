#include "pch.h"
#include "RootSignature.h"
#include "Influx/Graphics/DxLayer/DxLayer.h"

namespace Influx
{
	sPtr<RootSignature> RootSignature::Create(comPtr<ID3D12Device> device, const std::vector<RootParameter>& rootParams)
	{
		sPtr<RootSignature> rootSignature(new RootSignature());

		auto rootsigDesc = DxLayer::CreateRootSignatureDesc(rootParams.data(), D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);
		rootSignature->mpDxRootSignature = DxLayer::CreateRootSignature(device.Get(), &rootsigDesc);

		return rootSignature;
	}
}

