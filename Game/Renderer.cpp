#include "Renderer.h"
#include "Game.h"

Renderer::Renderer(Game* game)
	: m_game(game)
	, m_featureLevel()
	, m_bufferIndex(0)
	, m_rtvIncSize(0)
{
}

Renderer::~Renderer()
{
}

bool Renderer::initialize()
{
#ifdef _DEBUG
	enableDebugLayer(); 
#endif  

	if (!createFactory())return false;
	if (!createDevice(L"NVIDIA"))   return false;
	if (!createCommandQueue())      return false;
	if (!createSwapchain())         return false;
	if (!createCommandAllocators()) return false;
	if (!createCommandList())       return false;
	if (!createRenderTargetView())  return false;

	m_cmdList->Close();

	return true;
}

void Renderer::begin()
{
}

void Renderer::end()
{
}

bool Renderer::createFactory()
{
	UINT debugFlag = 0;
#ifdef _DEBUG
	debugFlag = DXGI_CREATE_FACTORY_DEBUG;
#endif // _DEBUG

	HRESULT hr = CreateDXGIFactory2(debugFlag, IID_PPV_ARGS(m_dxgiFactory.GetAddressOf()));

	return SUCCEEDED(hr);
}



bool Renderer::createDevice(const wchar_t* adapterName)
{
	std::vector<IDXGIAdapter*> adapters;
	IDXGIAdapter* sa = nullptr;
	int i = 0;
	while (m_dxgiFactory->EnumAdapters(i, &sa) != DXGI_ERROR_NOT_FOUND)
	{
		adapters.push_back(sa);
		++i;
	}
	sa = nullptr;
	for (auto adp : adapters)
	{
		DXGI_ADAPTER_DESC adpDesc = {};
		adp->GetDesc(&adpDesc);
		std::wstring str = adpDesc.Description;
		if (str.find(adapterName) != std::wstring::npos)
		{
			sa = adp;
			break;
		}
	}
	D3D_FEATURE_LEVEL levels[] =
	{
		D3D_FEATURE_LEVEL_12_1,
		D3D_FEATURE_LEVEL_12_0,
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0
	};

	for (auto level : levels)
	{
		if (SUCCEEDED(D3D12CreateDevice(sa, level, IID_PPV_ARGS(m_device.GetAddressOf()))))
		{
			m_featureLevel = level;
			break;
		}
	}
	for (auto adp : adapters)
	{
		adp->Release();
	}
	

	if (m_device == nullptr) return false;

	m_rtvIncSize = m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	return true;
}

void Renderer::enableDebugLayer()
{
	ComPtr<ID3D12Debug> debugLayer;
	D3D12GetDebugInterface(IID_PPV_ARGS(debugLayer.GetAddressOf()));
	debugLayer->EnableDebugLayer();
}

bool Renderer::createCommandQueue()
{
	D3D12_COMMAND_QUEUE_DESC desc = {};
	desc.Flags    = D3D12_COMMAND_QUEUE_FLAG_NONE;
	desc.NodeMask = 0;
	desc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
	desc.Type     = D3D12_COMMAND_LIST_TYPE_DIRECT;

	HRESULT hr = m_device->CreateCommandQueue(&desc, IID_PPV_ARGS(m_cmdQueue.GetAddressOf()));
	return SUCCEEDED(hr);
}

bool Renderer::createSwapchain()
{
	DXGI_SWAP_CHAIN_DESC1 scDesc = {};
	scDesc.Width              = m_game->getWidth();
	scDesc.Height             = m_game->getHeight();
	scDesc.Format             = DXGI_FORMAT_R8G8B8A8_UNORM;
	scDesc.Stereo             = FALSE;
	scDesc.BufferUsage        = DXGI_USAGE_BACK_BUFFER;
	scDesc.BufferCount        = FrameNum;
	scDesc.SampleDesc.Count   = 1;
	scDesc.SampleDesc.Quality = 0;
	scDesc.Scaling            = DXGI_SCALING_NONE;
	scDesc.SwapEffect         = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	scDesc.AlphaMode          = DXGI_ALPHA_MODE_UNSPECIFIED;

	DXGI_SWAP_CHAIN_FULLSCREEN_DESC fsDesc = {};
	fsDesc.Windowed = TRUE;
	HWND hwnd = m_game->getHwnd();
	ComPtr<IDXGISwapChain1> swapchain;
	HRESULT hr = m_dxgiFactory->CreateSwapChainForHwnd(
		m_cmdQueue.Get(), hwnd, &scDesc, &fsDesc,
		nullptr, swapchain.GetAddressOf());
	if (FAILED(hr)) return false;

	hr = swapchain.As(&m_swapchain);
	if (FAILED(hr)) return false;
	m_bufferIndex = m_swapchain->GetCurrentBackBufferIndex();
	hr = m_dxgiFactory->MakeWindowAssociation(hwnd, DXGI_MWA_NO_ALT_ENTER);

	return SUCCEEDED(hr);
}

bool Renderer::createCommandAllocators()
{
	for (UINT i = 0; i < FrameNum; ++i)
	{
		HRESULT hr = m_device->CreateCommandAllocator(
			D3D12_COMMAND_LIST_TYPE_DIRECT,
			IID_PPV_ARGS(m_cmdAllocators[i].GetAddressOf()));
		if (FAILED(hr)) return false;
	}
	return true;
}

bool Renderer::createCommandList()
{
	HRESULT hr = m_device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT,
		m_cmdAllocators[m_bufferIndex].Get(), nullptr,
		IID_PPV_ARGS(m_cmdList.GetAddressOf()));

	return SUCCEEDED(hr);
}

bool Renderer::createRenderTargetView()
{
	D3D12_DESCRIPTOR_HEAP_DESC desc = {};
	desc.NumDescriptors = FrameNum;
	desc.Type           = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	desc.Flags          = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	desc.NodeMask       = 0;

	HRESULT hr = m_device->CreateDescriptorHeap(
		&desc, IID_PPV_ARGS(m_rtvHeap.GetAddressOf()));
	if (FAILED(hr)) return false;

	auto handle = m_rtvHeap->GetCPUDescriptorHandleForHeapStart();
	for (UINT i = 0; i < FrameNum; ++i)
	{
		hr = m_swapchain->GetBuffer(i, IID_PPV_ARGS(m_backBuffers[i].GetAddressOf()));
		if (FAILED(hr)) return false;

		D3D12_RENDER_TARGET_VIEW_DESC rDesc = {};
		rDesc.Format               = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
		rDesc.ViewDimension        = D3D12_RTV_DIMENSION_TEXTURE2D;
		rDesc.Texture2D.MipSlice   = 0;
		rDesc.Texture2D.PlaneSlice = 0;

		m_device->CreateRenderTargetView(m_backBuffers[i].Get(), &rDesc, handle);
		handle.ptr += m_rtvIncSize;
	}
	return true;
 }