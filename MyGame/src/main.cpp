#include <grafika2d.h>
#include <rect.h>
#include <string>

using namespace Grafika;

int main(void)
{
#pragma region Start app
    App grafika;

    if(!grafika.Create("Grafika2D", 640, 480, false, false))
    {
        MessageBox(NULL, L"Failed to create window", L"Error", MB_OK | MB_ICONERROR);
        return 1;
    }

    if (!grafika.InitGraphics())
    {
        MessageBox(NULL, L"Failed to initialize graphics", L"Error", MB_OK | MB_ICONERROR);
        return 2;
    }

#pragma endregion

#pragma region Load Resources

    IDWriteTextFormat* labelTextFormat;
    if (FAILED(grafika.CreateTextFormat(&labelTextFormat, L"Verdana", NULL, 12))) return 4;

    Bitmap* img1;
    if (!grafika.CreateBitmapImage("assets/Bricks090_1K-JPG_Color.jpg", &img1))
    {
        MessageBox(NULL, L"Failed to load image!", L"Error", MB_OK | MB_ICONERROR);
        return 3;
    }

    Bitmap* img2;
    if (!grafika.CreateBitmapImage("assets/dvd.png", &img2))
    {
        MessageBox(NULL, L"Failed to load image!", L"Error", MB_OK | MB_ICONERROR);
        return 3;
    }

#pragma endregion

    grafika.Show();

    PlaySound(L"assets\\我姓石 (Wo Xing Shi).wav", NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);

    float posx = 25.f, posy = 25.f;
    float velx = 0.5f, vely = 0.5f;

    float dvdspeed = 150.f;

    while (grafika.pool())
    {
        grafika.BeginDraw();
        grafika.ClearScreen();

        posx += velx * Time::DeltaTime() * dvdspeed;
        posy += vely * Time::DeltaTime() * dvdspeed;

        if (GetAsyncKeyState(VK_ESCAPE)) {
            break;
        }

        if (posx >= (640 - 25)) {
            velx = -velx;
        }

        if (posx <= 25) {
            velx = -velx;
        }

        if (posy >= (480 - 25)) {
            vely = -vely;
        }

        if (posy <= 25) {
            vely = -vely;
        }

        if (GetAsyncKeyState(VK_UP)) {
            dvdspeed += 5;
        } else if (GetAsyncKeyState(VK_DOWN)) {
            dvdspeed -= 5;
        }



        grafika.FillRectangle(grafika.GetColorBrush(Color::White), 50, 50, 150, 150);

        grafika.FillEllipse(grafika.GetColorBrush(Color::OrangeRed), Grafika::Ellipse(grafika.GetMousePos(), 25, 25));

        grafika.DrawEllipse(grafika.GetColorBrush(Color::White), Grafika::Ellipse(grafika.GetMousePos(), 25, 25), 2.0f);

        grafika.DrawBitmap(img1, RectF(150, 150, 250, 250));

        grafika.DrawTextW(L"What the HELL", labelTextFormat, grafika.GetColorBrush(Color::Red), 50, 50, 350, 150);
        grafika.DrawTextW((L"DVD Speed: " + std::to_wstring(dvdspeed)).c_str(), labelTextFormat, grafika.GetColorBrush(Color::White), 50, 150, 350, 300);
        grafika.DrawTextW(L"Change speed using UP and DOWN arrow!!!", labelTextFormat, grafika.GetColorBrush(Color::White), 50, 300, 350, 550);

        

        grafika.DrawBitmap(img2, RectF(posx - 50.f, posy - 50.f, posx + 50.f, posy + 50.f)); // 50px image size + 50px cuz dvd image is smoll
        //grafika.DrawBitmap(img2, RectF(posx - 25.f, posy - 25.f, posx + 25.f, posy + 25.f));

        grafika.DrawTextW((L"FPS: " + std::to_wstring(Time::FPS())).c_str(), labelTextFormat, grafika.GetColorBrush(Color::Yellow), 0, 0, 350, 150);


        grafika.EndDraw();
    }
}