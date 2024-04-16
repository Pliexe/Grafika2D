#include "grafika2d.h"
#include "rect.h"

using namespace Grafika;

/**
 * Creates a new window for the application.
 *
 * @param title The title of the window.
 * @param width The width of the window.
 * @param height The height of the window.
 * @param resizable Whether the window can be resized.
 * @param maximize Whether the window should be maximized.
 *
 * @return True if the window was successfully created, false otherwise.
 *
 * @throws None
 */
bool App::Create(std::string title, int width, int height, bool resizable, bool maximize)
{
	if (hWnd)
		return false;

	LPCWSTR className = L"GrafikaWindowClass";

	WNDCLASSEX windowClass;

	ZeroMemory(&windowClass, sizeof(WNDCLASSEX));
	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.hbrBackground = (HBRUSH)COLOR_WINDOW;
	windowClass.hInstance = GetModuleHandle(NULL);
	windowClass.lpfnWndProc = App::StaticWindowProc;
	windowClass.lpszClassName = className;
	windowClass.style = CS_HREDRAW | CS_VREDRAW;

	if (!GetClassInfoEx(GetModuleHandle(NULL), className, NULL))
		RegisterClassEx(&windowClass);

	RECT rect = {0, 0, width, height};
	AdjustWindowRectEx(&rect, WS_OVERLAPPEDWINDOW, FALSE, WS_EX_OVERLAPPEDWINDOW);

	int size = MultiByteToWideChar(CP_UTF8, 0, title.c_str(), -1, 0, 0);
	wchar_t *widetitle = new wchar_t[size];
	MultiByteToWideChar(CP_UTF8, 0, title.c_str(), -1, widetitle, size);

	DWORD dwStyle = WS_OVERLAPPEDWINDOW;
	if (!resizable)
		dwStyle &= ~WS_THICKFRAME;
	if (!maximize)
		dwStyle &= ~WS_MAXIMIZEBOX;

	hWnd = CreateWindowEx(
		WS_EX_OVERLAPPEDWINDOW,
		className,
		widetitle,
		dwStyle,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		rect.right - rect.left,
		rect.bottom - rect.top,
		NULL,
		NULL,
		windowClass.hInstance,
		this);

	Time::internal_init();
	return hWnd != NULL;
}

bool App::Destroy()
{
	if (!hWnd)
		return false;

	DestroyWindow(hWnd);
}

void App::Show()
{
	ShowWindow(hWnd, SW_SHOW);
}

void App::Hide()
{
	ShowWindow(hWnd, SW_HIDE);
}

LRESULT App::WindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_LBUTTONUP:
		// if (currentGameSection) currentGameSection->OnClick(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		break;
	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

void App::BeginDraw()
{
	m_RenderTarget->BeginDraw();
}

void App::EndDraw()
{
	m_RenderTarget->EndDraw();
}

void App::ClearScreen(Color color)
{
	m_RenderTarget->Clear(color);
}

Brush * App::CreateSolidBrush(Color color)
{
	ID2D1SolidColorBrush *brush;
	HRESULT hr;
	if (FAILED(hr = m_RenderTarget->CreateSolidColorBrush(color, &brush)))
	{
#ifdef DEBUG
		std::cout << "Graphics Error: Failed to create solid brush" << std::endl;
#endif
		return nullptr;
	}
	return (Brush *)(ID2D1Brush *)brush;
}

Brush * App::CreateSolidBrush(float r, float g, float b, float a)
{
	return CreateSolidBrush(Color(r, g, b, a));
}

/**
 * Draws a rectangle with the specified brush, dimensions, stroke width, and stroke style.
 *
 * @param brush The brush used to draw the rectangle.
 * @param rect The dimensions of the rectangle.
 * @param strokeWidth The width of the rectangle's stroke.
 * @param strokeStyle The style of the stroke.
 *
 * @throws None
 */
void Grafika::App::DrawRectangle(Brush* brush, const RectF& rect, float strokeWidth, StrokeStyle* strokeStyle)
{
	m_RenderTarget->DrawRectangle((D2D1_RECT_F)rect, brush, strokeWidth, strokeStyle);
}

void Grafika::App::DrawRoundedRectangle(Brush* brush, const RoundedRect& roundedRect, float strokeWidth, StrokeStyle* strokeStyle)
{
	m_RenderTarget->DrawRoundedRectangle((D2D1_ROUNDED_RECT)roundedRect, brush, strokeWidth, strokeStyle);
}

/**
 * Draws an ellipse with the specified brush, dimensions, stroke width, and stroke style.
 *
 * @param brush The brush used to fill the ellipse.
 * @param rect The bounding rectangle of the ellipse.
 * @param strokeWidth The width of the ellipse's stroke.
 * @param strokeStyle The style of the stroke.
 */
void Grafika::App::DrawEllipse(Brush* brush, const RectF& rect, float strokeWidth, StrokeStyle* strokeStyle)
{
	m_RenderTarget->DrawEllipse(D2D1::Ellipse(D2D1::Point2F((rect.right - rect.left) / 2.0f + rect.left, (rect.bottom - rect.top) / 2.0f + rect.top), (rect.right - rect.left) / 2.0f, (rect.bottom - rect.top) / 2.0f), brush, strokeWidth, strokeStyle);
}

/**
 * Draws an ellipse with the specified brush, dimensions, stroke width, and stroke style.
 *
 * @param brush The brush used to fill the ellipse.
 * @param ellipse The ellipse to be drawn.
 * @param strokeWidth The width of the ellipse's stroke.
 * @param strokeStyle The style of the stroke.
 */
void Grafika::App::DrawEllipse(Brush* brush, const Grafika::Ellipse& ellipse, float strokeWidth, StrokeStyle* strokeStyle)
{
	m_RenderTarget->DrawEllipse((D2D1_ELLIPSE)ellipse, brush, strokeWidth, strokeStyle);
}

/**
 * Fills a rounded rectangle with the specified brush.
 *
 * @param brush The brush used to fill the rounded rectangle.
 * @param roundedRect The rounded rectangle to be filled.
 *
 * @throws None
 */
void Grafika::App::FillRoundedRectangle(Brush* brush, const Grafika::RoundedRect& roundedRect)
{
	m_RenderTarget->FillRoundedRectangle((D2D1_ROUNDED_RECT)roundedRect, brush);
}

void App::FillRectangle(Brush *brush, float left, float top, float right, float bottom)
{
	FillRectangle(brush, RectF(left, top, right, bottom));
}

void App::FillRectangle(Brush *brush, const RectF& rect)
{
	m_RenderTarget->FillRectangle((D2D1_RECT_F)rect, brush);
}

void App::FillEllipse(Brush *brush, const RectF& rect)
{
	m_RenderTarget->FillEllipse(D2D1::Ellipse(D2D1::Point2F((rect.right - rect.left) / 2.0f + rect.left, (rect.bottom - rect.top) / 2.0f + rect.top), (rect.right - rect.left) / 2.0f, (rect.bottom - rect.top) / 2.0f), brush);
}

void App::FillEllipse(Brush *brush, const Grafika::Ellipse &ellipse)
{
	m_RenderTarget->FillEllipse((D2D1_ELLIPSE)ellipse, brush);
}

void App::DrawText(std::string text, IDWriteTextFormat *textFormat, ID2D1Brush *brush, D2D1_RECT_F rect)
{
	int size = MultiByteToWideChar(CP_UTF8, 0, text.c_str(), -1, 0, 0);
	wchar_t *widetext = new wchar_t[size];
	MultiByteToWideChar(CP_UTF8, 0, text.c_str(), -1, widetext, size);

	DrawText(widetext, textFormat, brush, rect);
}

void App::DrawTextW(const wchar_t *text, IDWriteTextFormat *textFormat, ID2D1Brush *brush, D2D1_RECT_F rect)
{
	m_RenderTarget->DrawTextW(text, wcslen(text), textFormat, rect, brush);
}

bool App::CreateBitmapImage(std::filesystem::path path, Bitmap **bmp)
{
	CoInitializeEx(nullptr, COINITBASE_MULTITHREADED);
	IWICImagingFactory *wicFactory = NULL;
	if (FAILED(CoCreateInstance(
			CLSID_WICImagingFactory,
			NULL,
			CLSCTX_INPROC_SERVER,
			IID_IWICImagingFactory,
			(LPVOID *)&wicFactory)))
		return false;

	IWICBitmapDecoder *wicDecoder = NULL;

	if (FAILED(wicFactory->CreateDecoderFromFilename(
			path.wstring().c_str(),
			NULL,
			GENERIC_READ,
			WICDecodeMetadataCacheOnLoad,
			&wicDecoder)))
		return false;

	IWICBitmapFrameDecode *wicFrame = NULL;

	if (FAILED(wicDecoder->GetFrame(0, &wicFrame)))
		return false;

	IWICFormatConverter *wicConverter = NULL;

	if (FAILED(wicFactory->CreateFormatConverter(&wicConverter)))
		return false;

	if (FAILED(wicConverter->Initialize(
			wicFrame,
			GUID_WICPixelFormat32bppPBGRA,
			WICBitmapDitherTypeNone,
			NULL,
			0.0,
			WICBitmapPaletteTypeCustom)))
		return false;

	if (FAILED(m_RenderTarget->CreateBitmapFromWicBitmap(
			wicConverter,
			NULL,
			(ID2D1Bitmap **)bmp)))
		return false;

	SafeRelease(&wicFactory);
	SafeRelease(&wicDecoder);
	SafeRelease(&wicConverter);
	SafeRelease(&wicFrame);

	return true;
}

LRESULT CALLBACK App::StaticWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	App* pThis = NULL;

	if (uMsg == WM_NCCREATE)
	{
		CREATESTRUCT *pCreate = (CREATESTRUCT *)lParam;
		pThis = (App*)pCreate->lpCreateParams;
		SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)pThis);

		pThis->hWnd = hWnd;
	}
	else
		pThis = (App*)GetWindowLongPtr(hWnd, GWLP_USERDATA);

	if (pThis)
		return pThis->WindowProc(uMsg, wParam, lParam);
	else
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

bool App::InitGraphics()
{
	if (FAILED(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, m_Factory.ReleaseAndGetAddressOf())))
	{
		MessageBox(hWnd, L"Failed to initialize factory!", L"Graphics Error", MB_ICONERROR | MB_OK);
		return false;
	}

	RECT rect;
	GetClientRect(hWnd, &rect);

	if (FAILED(m_Factory->CreateHwndRenderTarget(
			D2D1::RenderTargetProperties(),
			D2D1::HwndRenderTargetProperties(
				hWnd, D2D1::SizeU(rect.right, rect.bottom)),
			m_RenderTarget.ReleaseAndGetAddressOf())))
	{
		MessageBox(hWnd, L"Failed to initialize render target!", L"Graphics Error", MB_ICONERROR | MB_OK);
		return false;
	}

	if (FAILED(DWriteCreateFactory(
			DWRITE_FACTORY_TYPE_SHARED,
			__uuidof(m_DWriteFactory),
			reinterpret_cast<IUnknown **>(m_DWriteFactory.ReleaseAndGetAddressOf()))))
	{
		MessageBox(hWnd, L"Failed to initialize direct write!", L"Graphics Error", MB_ICONERROR | MB_OK);
		return false;
	}

	return true;
}