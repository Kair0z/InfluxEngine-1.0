#pragma once

namespace Influx
{
	class RootSignature final
	{
	public:
		using RootParameter = CD3DX12_ROOT_PARAMETER1;
		static sPtr<RootSignature> Create(comPtr<ID3D12Device> device, const std::vector<RootParameter>& rootParams);

		comPtr<ID3D12RootSignature> GetDxRootSignature() const { return mpDxRootSignature; };

	private:
		RootSignature() = default;

		comPtr<ID3D12RootSignature> mpDxRootSignature;
	};
}



