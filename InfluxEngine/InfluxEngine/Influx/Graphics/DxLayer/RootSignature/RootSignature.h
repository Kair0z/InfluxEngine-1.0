#pragma once

namespace Influx
{
	class RootSignature final
	{
	public:
		static sPtr<RootSignature> Create(comPtr<ID3D12Device> device);

		comPtr<ID3D12RootSignature> GetDxRootSignature() const { return mpDxRootSignature; };

	private:
		RootSignature() = default;

		comPtr<ID3D12RootSignature> mpDxRootSignature;
	};
}



