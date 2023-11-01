#define STB_IMAGE_IMPLEMENTATION

#include <Core/game.h>


int main()
{
	std::unique_ptr<Game> game = std::make_unique<Game>();
 	bool is_game_init = game->initialize();

	if (!is_game_init) return -1;

	game->run();
	game->close();

	return 0;
}