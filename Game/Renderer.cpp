#include "Renderer.h"
#include "Game.h"

Renderer::Renderer(Game* game)
	: m_game(game)
	, m_featureLevel()
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
	if (!createDevice(L"NVIDIA")) return false;

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
	return true;
}

void Renderer::enableDebugLayer()
{
	ComPtr<ID3D12Debug> debugLayer;
	D3D12GetDebugInterface(IID_PPV_ARGS(debugLayer.GetAddressOf()));
	debugLayer->EnableDebugLayer();
}

//54