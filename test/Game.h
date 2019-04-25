#pragma once

#include <memory>
#include "Geometry.h"

class Octopus;
class Player;

#define LpGame (Game::GetInstance())

class Game
{
public:
	static Game & GetInstance()
	{
		return *s_Instance;
	}

	void Init();
	void Run();
	void End();

	const Vector2 & GetScreenSize();
private:
	Game();
	~Game();
	struct GameDeleter
	{
		void operator()(Game * game) const
		{
			delete game;
		}
	};

	static std::unique_ptr<Game, GameDeleter> s_Instance;
	std::shared_ptr<Octopus> octopus;
	std::shared_ptr<Player> player;

	const Vector2 screenSize;
};

