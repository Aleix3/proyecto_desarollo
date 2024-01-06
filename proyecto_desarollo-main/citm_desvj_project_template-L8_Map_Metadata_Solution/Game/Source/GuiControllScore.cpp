#include "GuiControllScore.h"
#include "Scene.h"
#include "App.h"

GuiControlScore::GuiControlScore(uint32 id, SDL_Rect bounds) : GuiControl(GuiControlType::VALUEBOX, bounds, ""), lastScore(-1), surface(NULL), texture(NULL)
{
}

GuiControlScore::~GuiControlScore()
{
	if (surface != NULL)
		SDL_FreeSurface(surface);
	if (texture != NULL)
		SDL_DestroyTexture(texture);
}

bool GuiControlScore::Update(float dt)
{
	if (app->scene->score != lastScore)
	{
		lastScore = app->scene->score;

		// Libera la superficie y la textura antiguas
		if (surface != NULL)
			SDL_FreeSurface(surface);
		if (texture != NULL)
			SDL_DestroyTexture(texture);

		// Comprueba si la fuente se ha cargado correctamente
		if (app->render->font == NULL)
		{
			LOG("TTF_OpenFont: %s\n", TTF_GetError());
			return false;
		}

		// Crea una nueva superficie de texto
		SDL_Color color = { 255, 255, 255 }; // Color del texto en RGB (blanco)
		std::string scoreText = "Score: " + std::to_string(lastScore); // Texto a mostrar
		surface = TTF_RenderText_Solid(app->render->font, scoreText.c_str(), color);
		if (surface == NULL)
		{
			LOG("TTF_RenderText_Solid: %s\n", TTF_GetError());
			return false;
		}

		// Crea una nueva textura a partir de esa superficie
		texture = SDL_CreateTextureFromSurface(app->render->renderer, surface);
		if (texture == NULL)
		{
			LOG("SDL_CreateTextureFromSurface: %s\n", SDL_GetError());
			return false;
		}
	}

	// Define un rectángulo donde se mostrará el texto
	SDL_Rect textRect;
	textRect.x = 0; // Posición en el eje X
	textRect.y = 0; // Posición en el eje Y
	textRect.w = surface->w; // Ancho del texto
	textRect.h = surface->h; // Alto del texto

	// Copia la textura en el renderizador
	SDL_RenderCopy(app->render->renderer, texture, NULL, &textRect);

	return true;
}
