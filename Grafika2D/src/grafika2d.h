#include <iostream>
#include <Windows.h>
#include <D2D1.h>
#include <dwrite.h>
#include <wincodec.h>
#include <wrl.h>
#include <map>
#include <filesystem>
#include <chrono>

#pragma once
#ifndef GRAFIKA2D
#define GRAFIKA2D

#include "ellipse.h"
#include "rect.h"
#include "timestep.h"
#include "color.h"
#include "graphics.h"
#include "brush.h"
#include "bitmap.h"

// https://docs.microsoft.com/en-us/windows/win32/medfound/saferelease
template <class T>
void SafeRelease(T **ppT)
{
    if (*ppT)
    {
        (*ppT)->Release();
        *ppT = NULL;
    }
}

namespace Grafika
{
    struct StrokeStyle : public ID2D1StrokeStyle {
        
    };

    class App
    {
    public:
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
        bool Create(std::string title, int width, int height, bool resizable = false, bool maximize = false);
        bool Destroy();

        bool InitGraphics();

        /**
         * Makes the window visible.

         * This method is useful when the window was created hidden using
         * the `resizable` or `maximize` parameter in the `Create` method.
         *
         * @see Create
         */
        void Show();
        /**
         * Hides the window.
         *
         * @see Create
         */
        void Hide();
        void Close();

        /**
         * Processes the Windows message queue and updates the internal frame time.
         *
         * @return `false` if a WM_QUIT message is received, `true` otherwise.
         *
         * @throws None
         */
        bool pool()
        {
            Time::internal_frame_update();
            MSG message;
            message.message = WM_NULL;

            if (PeekMessage(&message, NULL, 0, 0, PM_REMOVE))
            {
                TranslateMessage(&message);
                DispatchMessage(&message);
                if (message.message == WM_QUIT)
                    return false;
            }
            return true;
        }

    /**
     * Executes a loop function until a WM_QUIT message is received.
     *
     * @param loop A pointer to the function to be executed in each iteration.
     *
     * @throws None
     */
        void Loop(void (*loop)())
        {
            if (!loop)
            {
                MessageBox(NULL, L"Loop function not defined", L"Error", MB_OK | MB_ICONERROR);
                return;
            }
            Time::internal_init();
            MSG message;
            message.message = WM_NULL;

            while (message.message != WM_QUIT)
            {
                if (PeekMessage(&message, NULL, 0, 0, PM_REMOVE))
                {
                    TranslateMessage(&message);
                    DispatchMessage(&message);
                }
                else
                {
                    loop();
                }
            }
        }

        /**
         * Retrieves the current mouse position on the window.
         *
         * @return The mouse position as a PointF object.
         */
        const PointF GetMousePos()
        {
            POINT p;
            GetCursorPos(&p);
            ScreenToClient(hWnd, &p);
            return PointF{(float)p.x, (float)p.y};
        }

        Microsoft::WRL::ComPtr<ID2D1HwndRenderTarget> GetRenderTarget() { return m_RenderTarget; }

        /**
         * Begins drawing on the render target.
         */
        void BeginDraw();
        /**
         * Ends drawing on the render target.
        */
        void EndDraw();
        /**
         * Clears the screen with the specified color.
         *
         * @param color The color to clear the screen with.
         *
         * @throws None
         */
        void ClearScreen(Color color = Color::Black);

        /**
         * Creates a solid color brush with the specified color.
         *
         * @param color The color of the brush.
         *
         * @return A pointer to the created brush, or nullptr if the brush creation fails.
         */
        Brush *CreateSolidBrush(Color color);
        /**
         * Creates a solid brush with the specified RGBA color values.
         *
         * @param r The red component of the color (0.0f - 1.0f).
         * @param g The green component of the color (0.0f - 1.0f).
         * @param b The blue component of the color (0.0f - 1.0f).
         * @param a The alpha component of the color (0.0f - 1.0f).
         *
         * @return A pointer to the created solid brush.
         *
         * @throws None
         */
        Brush *CreateSolidBrush(float r, float g, float b, float a);

        /**
         * Draws a bitmap on the render target with optional opacity and interpolation mode.
         *
         * @param bitmap The bitmap to draw.
         * @param rect The destination rectangle of the bitmap.
         * @param opacity The opacity of the bitmap (default is 1.0).
         * @param interpolationMode The interpolation mode for scaling the bitmap.
         *
         * @throws None
         */
        void DrawBitmap(Bitmap *bitmap, const RectF &rect, float opacity = 1.0f, D2D1_BITMAP_INTERPOLATION_MODE interpolationMode = D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR)
        {
            m_RenderTarget->DrawBitmap(
                bitmap, (D2D1_RECT_F)rect,
                opacity, interpolationMode,
                D2D1::RectF(0.0f, 0.0f, bitmap->GetSize().width, bitmap->GetSize().height));
        }

        inline void DrawRectangle(Brush *brush, float left, float top, float right, float bottom, float strokeWidth = 1.0f, StrokeStyle *strokeStyle = NULL) { DrawRectangle(brush, RectF(left, top, right, bottom), strokeWidth, strokeStyle); }
        void DrawRectangle(Brush *brush, const RectF &rect, float strokeWidth = 1.0f, StrokeStyle *strokeStyle = NULL);

        inline void DrawRoundedRectangle(Brush *brush, float left, float top, float right, float bottom, float radius, float strokeWidth = 1.0f, StrokeStyle *strokeStyle = NULL) { DrawRoundedRectangle(brush, RoundedRect(left, top, right, bottom, radius), strokeWidth, strokeStyle); }
        inline void DrawRoundedRectangle(Brush *brush, float left, float top, float right, float bottom, float radiusX, float radiusY, float strokeWidth = 1.0f, StrokeStyle *strokeStyle = NULL) { DrawRoundedRectangle(brush, RoundedRect(left, top, right, bottom, radiusX, radiusY), strokeWidth, strokeStyle); }
        inline void DrawRoundedRectangle(Brush *brush, const RectF &rect, float radius, float strokeWidth = 1.0f, StrokeStyle *strokeStyle = NULL) { DrawRoundedRectangle(brush, RoundedRect(rect, radius), strokeWidth, strokeStyle); }
        inline void DrawRoundedRectangle(Brush *brush, const RectF &rect, float radiusX, float radiusY, float strokeWidth = 1.0f, StrokeStyle *strokeStyle = NULL) { DrawRoundedRectangle(brush, RoundedRect(rect, radiusX, radiusY), strokeWidth, strokeStyle); }
        void DrawRoundedRectangle(Brush *brush, const RoundedRect &roundedRect, float strokeWidth = 1.0f, StrokeStyle *strokeStyle = NULL);

        void DrawEllipse(Brush *brush, const RectF &rect, float strokeWidth = 1.0f, StrokeStyle *strokeStyle = NULL);
        inline void DrawEllipse(Brush *brush, float left, float top, float right, float bottom, float strokeWidth = 1.0f, StrokeStyle *strokeStyle = NULL) { DrawEllipse(brush, RectF(left, top, right, bottom), strokeWidth, strokeStyle); }
        inline void DrawEllipse(Brush *brush, float centerX, float centerY, float radius, float strokeWidth = 1.0f, StrokeStyle *strokeStyle = NULL) { DrawEllipse(brush, Grafika::Ellipse(centerX, centerY, radius, radius), strokeWidth, strokeStyle); }
        inline void DrawEllipse(Brush *brush, const PointF &center, float radiusX, float radiusY, float strokeWidth = 1.0f, StrokeStyle *strokeStyle = NULL) { DrawEllipse(brush, Grafika::Ellipse(center, radiusX, radiusY), strokeWidth, strokeStyle); }
        inline void DrawEllipse(Brush *brush, const PointF &center, float radius, float strokeWidth = 1.0f, StrokeStyle *strokeStyle = NULL) { DrawEllipse(brush, Grafika::Ellipse(center, radius), strokeWidth, strokeStyle); }
        void DrawEllipse(Brush *brush, const Grafika::Ellipse &ellipse, float strokeWidth = 1.0f, StrokeStyle *strokeStyle = NULL);
        
        inline void FillRoundedRectangle(Brush *brush, float left, float top, float right, float bottom, float radius) { FillRoundedRectangle(brush, RoundedRect(left, top, right, bottom, radius)); }
        inline void FillRoundedRectangle(Brush *brush, float left, float top, float right, float bottom, float radiusX, float radiusY) { FillRoundedRectangle(brush, RoundedRect(left, top, right, bottom, radiusX, radiusY)); }
        inline void FillRoundedRectangle(Brush *brush, const RectF &rect, float radius) { FillRoundedRectangle(brush, RoundedRect(rect, radius)); }
        inline void FillRoundedRectangle(Brush *brush, const RectF &rect, float radiusX, float radiusY) { FillRoundedRectangle(brush, RoundedRect(rect, radiusX, radiusY)); }
        void FillRoundedRectangle(Brush *brush, const Grafika::RoundedRect &roundedRect);

        void FillRectangle(Brush *brush, float left, float top, float right, float bottom);
        void FillRectangle(Brush *brush, const RectF &rect);

        void FillEllipse(Brush *brush, const RectF &rect);
        void FillEllipse(Brush *brush, const Grafika::Ellipse &ellipse);
        inline void FillEllipse(Brush *brush, float left, float top, float right, float bottom) { FillEllipse(brush, RectF(left, top, right, bottom)); }
        inline void FillEllipse(Brush *brush, float centerX, float centerY, float radius) { FillEllipse(brush, Grafika::Ellipse(centerX, centerY, radius, radius)); }
        inline void FillEllipse(Brush *brush, const PointF &center, float radiusX, float radiusY) { FillEllipse(brush, Grafika::Ellipse(center, radiusX, radiusY)); }
        inline void FillEllipse(Brush *brush, const PointF &center, float radius) { FillEllipse(brush, Grafika::Ellipse(center, radius)); }

        void DrawText(std::string text, IDWriteTextFormat *textFormat, ID2D1Brush *brush, float left, float top, float right, float bottom) { DrawText(text, textFormat, brush, D2D1::RectF(left, top, right, bottom)); }
        void DrawText(std::string text, IDWriteTextFormat *textFormat, ID2D1Brush *brush, D2D1_RECT_F rect);
        void DrawTextW(const wchar_t *text, IDWriteTextFormat *textFormat, ID2D1Brush *brush, float left, float top, float right, float bottom) { DrawText(text, textFormat, brush, D2D1::RectF(left, top, right, bottom)); }
        void DrawTextW(const wchar_t *text, IDWriteTextFormat *textFormat, ID2D1Brush *brush, D2D1_RECT_F rect);

        /**
         * Retrieves the brush associated with the given color.
         *
         * @param color the color for which to retrieve the brush
         *
         * @return the brush associated with the given color
         *
         * @throws None
         */
        Brush *GetColorBrush(Color::Enum color)
        {
            static std::map<Color::Enum, Brush *> brushes;
            auto it = brushes.find(color);
            if (it == brushes.end())
            {
                it = brushes.insert(std::make_pair(color, CreateSolidBrush(color))).first;
            }
            return it->second;
        }

        /**
         * Creates a BitmapImage from a file.
         *
         * @param path the path to the file
         * @param bmp a pointer to a pointer to a BitmapImage
         *
         * @return true if successful, false otherwise
         *
         * @throws None
         */
        bool CreateBitmapImage(std::filesystem::path path, Bitmap **bmp);

        /**
         * Creates a text format object using the DirectWrite library.
         *
         * @param ref a pointer to a pointer to an IDWriteTextFormat interface
         * @param fontFamilyName the name of the font family to use
         * @param fontCollection the font collection to use
         * @param fontSize the size of the font
         * @param fontWeight the weight of the font (default: DWRITE_FONT_WEIGHT_NORMAL)
         * @param fontStyle the style of the font (default: DWRITE_FONT_STYLE_NORMAL)
         * @param fontStretch the stretch of the font (default: DWRITE_FONT_STRETCH_NORMAL)
         * @param localeName the locale name to use (default: "")
         *
         * @return an HRESULT indicating success or failure
         *
         * @throws None
         */
        HRESULT CreateTextFormat(IDWriteTextFormat **ref, const WCHAR *fontFamilyName, IDWriteFontCollection *fontCollection, FLOAT fontSize, DWRITE_FONT_WEIGHT fontWeight = DWRITE_FONT_WEIGHT_NORMAL,
                                 DWRITE_FONT_STYLE fontStyle = DWRITE_FONT_STYLE_NORMAL,
                                 DWRITE_FONT_STRETCH fontStretch = DWRITE_FONT_STRETCH_NORMAL,
                                 const WCHAR *localeName = L"")
        {
            return m_DWriteFactory->CreateTextFormat(
                fontFamilyName, fontCollection, fontWeight, fontStyle, fontStretch,
                fontSize, localeName, ref);
        }

    private:
        HWND hWnd = NULL;

        Microsoft::WRL::ComPtr<ID2D1HwndRenderTarget> m_RenderTarget;
        Microsoft::WRL::ComPtr<ID2D1Factory> m_Factory;
        Microsoft::WRL::ComPtr<IDWriteFactory> m_DWriteFactory;

        static LRESULT CALLBACK StaticWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
        LRESULT WindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
    };
}

#endif
