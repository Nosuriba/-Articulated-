#include <DxLib.h>
#include "Game.h"
#include "Input.h"
#include "Player.h"
#include "Octopus.h"

std::unique_ptr<Game, Game::GameDeleter> Game::s_Instance(new Game());

Game::Game() : screenSize(768, 672)
{
}

Game::~Game()
{
}

void Game::Init()
{
	DxLib::SetGraphMode(screenSize.x, screenSize.y, 32);
	DxLib::ChangeWindowMode(true);
	DxLib::SetWindowText("テスト（´・ω・｀）");
	if (DxLib_Init() == -1)
	{
		return;
	}
	DxLib::SetDrawScreen(DX_SCREEN_BACK);

	player = std::make_shared<Player>();
	octopus = std::make_shared<Octopus>();
}

void Game::Run()
{
	Input input;
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		ClsDrawScreen();

		input.Update();
		player->Update(input);
		player->Draw();
		octopus->Update(player->GetPos());
		octopus->Draw();

		ScreenFlip();
	}
}

void Game::End()
{
	DxLib_End();
}

const Vector2 & Game::GetScreenSize()
{
	return screenSize;
}


