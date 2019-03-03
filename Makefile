
all: game bot

game: game.h game.c
	gcc -o game game.c

bot: game.h game.c bot.h bot.c
	gcc -o bot game.c bot.c -D BOT_PLAYING

clean:
	rm game bot
