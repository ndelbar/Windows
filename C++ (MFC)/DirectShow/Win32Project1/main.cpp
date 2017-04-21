#include<dshow.h>

void ThrowIfError(HRESULT hr);

int CALLBACK WinMain(
	_In_ HINSTANCE hInstance,
	_In_ HINSTANCE hPrevInstance,
	_In_ LPSTR     lpCmdLine,
	_In_ int       nCmdShow
)
{
	HRESULT hr = NULL;
	IGraphBuilder* pGraph = NULL;
	IMediaControl* pControl = NULL;
	IMediaEvent* pEvent = NULL;

	try
	{
		HRESULT hr = CoInitialize(NULL);
		ThrowIfError(hr);
		
		hr = CoCreateInstance(CLSID_FilterGraph, NULL,
			CLSCTX_INPROC_SERVER, IID_IGraphBuilder, (void**)&pGraph);

		ThrowIfError(hr);

		hr = pGraph->RenderFile(L"..\\Test.avi", NULL);

		ThrowIfError(hr);


		pGraph->QueryInterface(IID_IMediaControl, (void**)&pControl);
		pGraph->QueryInterface(IID_IMediaEvent, (void**)&pEvent);

		hr = pControl->Run();
		ThrowIfError(hr);

		long evCode = 0;
		pEvent->WaitForCompletion(INFINITE, &evCode);
	}
	catch (TCHAR* szErr)
	{
		MessageBox(NULL, (LPCTSTR)szErr, L"DirectShow Application", 0);
	}


	if (pGraph) 
	{
		pGraph->Release();
		pGraph = NULL;
	}

	if (pControl)
	{
		pControl->Release();
		pControl = NULL;
	}

	if (pEvent)
	{
		pEvent->Release();
		pEvent = NULL;
	}

	CoUninitialize();

	return hr;
}

void ThrowIfError(HRESULT hr)
{
	if (FAILED(hr))
	{
		TCHAR szErr[MAX_ERROR_TEXT_LEN] = {0};
		DWORD res = AMGetErrorText(hr, szErr, MAX_ERROR_TEXT_LEN);

		if (res == 0)
		{
			StringCchPrintf(szErr, MAX_ERROR_TEXT_LEN, TEXT("Unknown Error: 0x%2x"), hr);
		}

		//MessageBox(0, szErr, TEXT("Error!"), MB_OK | MB_ICONERROR);
		throw szErr;
	}
}