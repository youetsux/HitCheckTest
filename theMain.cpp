#include "DxLib.h"
#include "globals.h"
#include "input.h"
#include "Math2D.h"
#include <string>


namespace
{
	const int BGCOLOR[3] = { 240, 145, 153 };
	int crrTime;
	int prevTime;
	
	enum RECTTYPE
	{
		TOPLEFT,
		BOTTOMLEFT,
		BOTTOMCENTER,
		CENTER
	};
	struct rect
	{
		Vector2D pos;
		float w;
		float h;
	};

	const float MOVE_SPEED = 200.0f;
	const Vector2D FIXED_RECT_POS = { WIN_WIDTH / 2.0f - 60.0f, WIN_HEIGHT / 2.0f - 60.0f };
	const Vector2D MOVE_RECT_POS = { 100.0f, 100.0f };
	const float FIXED_RECT_W = 120.0f;
	const float FIXED_RECT_H = 120.0f;
	const float MOVE_RECT_W = 80.0f;
	const float MOVE_RECT_H = 80.0f;

	bool IsHitRect(const rect& a, const rect& b)
	{
		//aが、固定のやつ、bが、動かすやつ	
		
		bool hit = false;
		float aRight = a.pos.x + a.w;
		float bLeft = b.pos.x;
		//if (aRight > bLeft)
		//	hit = true;
		float aLeft = a.pos.x;
		float bRight = b.pos.x + b.w;
		if ((aLeft < bRight) && (aRight > bLeft))
			hit = true;
		DrawFormatString(30, 100, GetColor(255, 255, 255), "(aLeft < bRight) : %s", (aLeft < bRight) ? "true" : "false");
		DrawFormatString(30, 120, GetColor(255, 255, 255), "(aRight > bLeft) : %s", (aRight > bLeft) ? "true" : "false");
		return hit;
	}


}


float gDeltaTime = 0.0f;

void DxInit()
{
	ChangeWindowMode(true);
	SetWindowSizeChangeEnableFlag(false, false);
	SetMainWindowText("TITLE");
	SetGraphMode(WIN_WIDTH, WIN_HEIGHT, 32);
	SetWindowSizeExtendRate(1.0);
	SetBackgroundColor(BGCOLOR[0],BGCOLOR[1],BGCOLOR[2]);

	// DX library init
	if (DxLib_Init() == -1)
	{
		DxLib_End();
	}

	SetDrawScreen(DX_SCREEN_BACK);
}

void MyGame()
{
	static rect fixedRect = { FIXED_RECT_POS, FIXED_RECT_W, FIXED_RECT_H };
	static rect moveRect = { MOVE_RECT_POS, MOVE_RECT_W, MOVE_RECT_H };

	float deltaTime = GetDeltaTime();

	if (Input::IsKeepKeyDown(KEY_INPUT_LEFT))
	{
		moveRect.pos.x -= MOVE_SPEED * deltaTime;
	}
	if (Input::IsKeepKeyDown(KEY_INPUT_RIGHT))
	{
		moveRect.pos.x += MOVE_SPEED * deltaTime;
	}
	if (Input::IsKeepKeyDown(KEY_INPUT_UP))
	{
		moveRect.pos.y -= MOVE_SPEED * deltaTime;
	}
	if (Input::IsKeepKeyDown(KEY_INPUT_DOWN))
	{
		moveRect.pos.y += MOVE_SPEED * deltaTime;
	}

	bool hit = IsHitRect(fixedRect, moveRect);
	int bgColor;
	if (hit)
	{
		bgColor = GetColor(64, 64, 160);
	}
	else
	{
		bgColor = GetColor(BGCOLOR[0], BGCOLOR[1], BGCOLOR[2]);
	}


	DrawBox(0, 0, WIN_WIDTH, WIN_HEIGHT, bgColor, TRUE);
	DrawBox((int)fixedRect.pos.x, (int)fixedRect.pos.y,
		(int)(fixedRect.pos.x + fixedRect.w), (int)(fixedRect.pos.y + fixedRect.h),
		GetColor(200, 200, 200), TRUE);
	DrawBox((int)moveRect.pos.x, (int)moveRect.pos.y,
		(int)(moveRect.pos.x + moveRect.w), (int)(moveRect.pos.y + moveRect.h),
		GetColor(255, 255, 0), TRUE);
	std::string hitStr[2] = { "当たってない", "当たった！" };
	int fsize = GetFontSize();
	SetFontSize(50);
	DrawString(30, 30, hitStr[(int)hit].c_str(), GetColor(255, 255, 255), GetColor(255, 255, 0));
	SetFontSize(fsize);
	IsHitRect(fixedRect, moveRect);
}



int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	DxInit();
	crrTime = GetNowCount();
	prevTime = GetNowCount();

	while (true)
	{
		ClearDrawScreen();
		Input::KeyStateUpdate();

		crrTime = GetNowCount();
		float frameDeltaTime = (crrTime - prevTime) / 1000.0f;
		gDeltaTime = frameDeltaTime;

		MyGame();


		ScreenFlip();
		WaitTimer(16);

		prevTime = crrTime;

		if (ProcessMessage() == -1)
			break;
		if (CheckHitKey(KEY_INPUT_ESCAPE) == 1)
			break;
	}

	DxLib_End();
	return 0;
}