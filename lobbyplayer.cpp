#include "lobbyplayer.h"
#include "resourceloader.h"

LobbyPlayer::LobbyPlayer(int i, string n) : id(i), name(n) {}

void LobbyPlayer::moveTo(Tile* tile, Player* player) {
	if (piece == nullptr) piece = new King(tile, player); // change to if(!piece)
	else piece->moveTo(tile, 1);

	rect.setSize(sf::Vector2f(LOBBY_PLAYER_NAME_RECT_WIDTH, LOBBY_PLAYER_NAME_RECT_HEIGHT));
	rect.setPosition(sf::Vector2f(TILESIZE * tile->column + LOBBY_PLAYER_NAME_RECT_WIDTH_OFFSET, TILESIZE * tile->row + LOBBY_PLAYER_NAME_RECT_HEIGHT_OFFSET));
	rect.setFillColor(LOBBY_PLAYER_NAME_RECT_COLOR);

	font = *resource::loadFont(LOBBY_PLAYER_NAME_FONT);
	text.setFont(font);
	text.setCharacterSize(LOBBY_PLAYER_NAME_FONTSIZE);
	text.setString(name);
	text.setPosition(sf::Vector2f(TILESIZE * tile->column + LOBBY_PLAYER_NAME_RECT_WIDTH_OFFSET + LOBBY_PLAYER_NAME_RECT_WIDTH / 2 - text.getLocalBounds().width / 2, TILESIZE * tile->row + LOBBY_PLAYER_NAME_RECT_HEIGHT_OFFSET + LOBBY_PLAYER_NAME_RECT_HEIGHT / 2 + LOBBY_PLAYER_NAME_TEXT_HEIGHT_OFFSET));
	text.setFillColor(LOBBY_PLAYER_NAME_COLOR);
}