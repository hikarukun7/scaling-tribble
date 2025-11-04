#pragma once

#include <d3d12.h>
#include <dxgi1_4.h>

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

#include <vector>

#include <wrl.h>
using namespace Microsoft::WRL;


class Renderer
{
public:
	Renderer(class Game* game, float r = 0.0f, float g = 0.0f, float b = 0.0f);
	~Renderer();

	bool initialize();

	void begin();
	void end();

	void setBackColor(float r, float g, float b);
	const float* getBackColor() const { return m_backColor; }

private:
	static const UINT FrameNum = 2;
	class Game* m_game;

	ComPtr<IDXGIFactory4> m_dxgiFactory;
	D3D_FEATURE_LEVEL m_featureLevel;
	ComPtr<ID3D12Device>     m_device;
	ComPtr<ID3D12CommandQueue> m_cmdQueue;
	ComPtr<IDXGISwapChain3>    m_swapchain;
	UINT                       m_bufferIndex;
	ComPtr<ID3D12CommandAllocator> m_cmdAllocators[FrameNum];
	ComPtr<ID3D12GraphicsCommandList> m_cmdList;
	ComPtr<ID3D12DescriptorHeap> m_rtvHeap;
	UINT                         m_rtvIncSize;
	ComPtr<ID3D12Resource>       m_backBuffers[FrameNum];
	ComPtr<ID3D12Fence> m_fence;
	UINT64              m_fenceValues[FrameNum];

	float m_backColor[3];

	bool createFactory();
	bool createDevice(const wchar_t* adapterName);
	bool createCommandQueue();
	bool createSwapchain();
	bool createCommandAllocators();
	bool createCommandList();
	bool createRenderTargetView();
	bool createFence();
	void moveToNextFrame();
	void waitForGPU();
	void setResourceBarrier(D3D12_RESOURCE_STATES stateBefore,
		D3D12_RESOURCE_STATES stateAfter);
	void enableDebugLayer();

};

