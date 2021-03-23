#pragma once
#include "Influx/Core/Defines.h"
#include "Influx/Core/Alias.h"

struct ID3D12Device;
namespace Influx
{
	class FrameBuffer final
	{
		using DxBuffer = ID3D12Resource;
		using DxDescriptorHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE;

	public:
		struct Desc final
		{
			Vector2u dimensions{};
			uint8_t nColorTargets{};
			bool useDepthTarget = false;
		};
		static sPtr<FrameBuffer> Create(const Desc& desc);
		static sPtr<FrameBuffer> Create(comPtr<ID3D12Device> device, const Desc& desc);
		IFX_DelCpyMove(FrameBuffer);

		~FrameBuffer();

		void ClearColorTarget(Ptr<ID3D12GraphicsCommandList> cmdList, size_t colorTargetIdx, const Vector4f& clearColor);
		void ClearDepth(Ptr<ID3D12GraphicsCommandList> cmdList, const float depth);
		// [REV] Clearing stencils...

		Vector2u GetDimensions() const;
		uint8_t GetNumColorTargets() const;
		bool HasDepthTarget() const;

		comPtr<DxBuffer> GetColorTargetResource(size_t idx) const;
		comPtr<DxBuffer> GetDepthTargetResource() const;

		enum class ViewType{ RTV, SRV };
		D3D12_CPU_DESCRIPTOR_HANDLE GetColorCpuHandle(size_t idx, ViewType asView) const;
		D3D12_GPU_DESCRIPTOR_HANDLE GetColorGpuHandle(size_t idx, ViewType asView) const;
		D3D12_CPU_DESCRIPTOR_HANDLE GetDepthCpuHandle() const;
		D3D12_GPU_DESCRIPTOR_HANDLE GetDepthGpuHandle() const;

	private:
		FrameBuffer() = default;

		// Color Targets:
		comPtr<ID3D12DescriptorHeap> mpRtvDescHeap;
		comPtr<ID3D12DescriptorHeap> mpSrvDescHeap;
		std::vector<comPtr<DxBuffer>> mpBuffers;

		// Depth Target:
		comPtr<ID3D12DescriptorHeap> mpDsvDescHeap;
		comPtr<DxBuffer> mpDepthBuffer;

		Desc mConstructDesc;
		uint32_t mRtvDescSize;
		uint32_t mSrvDescSize;
	};
}


