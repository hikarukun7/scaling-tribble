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
	Renderer(class Game* game);
	~Renderer();

	bool initialize();

	void begin();
	void end();

private:
	class Game* m_game;

	ComPtr<IDXGIFactory4> m_dxgiFactory;
	D3D_FEATURE_LEVEL m_featureLevel;
	ComPtr<ID3D12Device>     m_device;
	ComPtr<ID3D12CommandQueue> m_cmdQueue;

	bool createFactory();
	bool createDevice(const wchar_t* adapterName);
	bool createCommandQueue();

	void enableDebugLayer();

};