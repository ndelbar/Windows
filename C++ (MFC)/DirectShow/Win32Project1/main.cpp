#include<dshow.h>

void ThrowIfError(HRESULT hr);

int CALLBACK WinMain(
	_In_ HINSTANCE hInstance,
	_In_ HINSTANCE hPrevInstance,
	_In_ LPSTR     lpCmdLine,
	_In_ int       nCmdShow
)
{
	HRESULT hr = CoInitialize(NULL);
	ThrowIfError(hr);

	IGraphBuilder* pGraph;
	hr = CoCreateInstance(CLSID_FilterGraph, NULL,
		CLSCTX_INPROC_SERVER, IID_IGraphBuilder, (void**)&pGraph);

	ThrowIfError(hr);

	CoUninitialize();
	return hr;
}

void ThrowIfError(HRESULT hr)
{
	if (FAILED(hr))
	{
		TCHAR szErr[MAX_ERROR_TEXT_LEN];
		DWORD res = AMGetErrorText(hr, szErr, MAX_ERROR_TEXT_LEN);

		if (res == 0)
		{
			StringCchPrintf(szErr, MAX_ERROR_TEXT_LEN, TEXT("Unknown Error: 0x%2x"), hr);
		}

		//MessageBox(0, szErr, TEXT("Error!"), MB_OK | MB_ICONERROR);
		throw szErr;
	}
}