#include "GuiCheckBox.h"
#include "Render.h"
#include "App.h"
#include "Audio.h"
#include "App.h"
#include "Textures.h"
#include "Scene.h"

GuiCheckBox::GuiCheckBox(uint32 id, SDL_Rect bounds, const char* text) : GuiControl(GuiControlType::BUTTON, id)
{
	this->bounds = bounds;
	this->text = text;

	canClick = true;
	drawBasic = false;
	click = false;

	
}

GuiCheckBox::~GuiCheckBox()
{

}

bool GuiCheckBox::Update(float dt)
{
	img = app->tex->Load("Assets/Textures/Menu/bg.png");
	img2 = app->tex->Load("Assets/Textures/Menu/bg2.png");

	return false;
}

bool GuiCheckBox::PostUpdate()
{

	if (state != GuiControlState::DISABLED)
	{
		// L15: DONE 3: Update the state of the GUiButton according to the mouse position
		app->input->GetMousePosition(mouseX, mouseY);

		SDL_Rect bounds2 = { bounds.x + 430, bounds.y - 375, bounds.w + 20, bounds.h + 20 };

		mouseX *= 2;

		mouseY *= 2;

		//If the position of the mouse if inside the bounds of the button 
		if (mouseX > bounds2.x && mouseX < bounds2.x + bounds2.w && mouseY > bounds2.y && mouseY < bounds2.y + bounds2.h) {

			state = GuiControlState::FOCUSED;

			if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT) {
				state = GuiControlState::PRESSED;
			}

			if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP) {
				NotifyObserver();
				click = !click;
			}
		}
		else {
			state = GuiControlState::NORMAL;
		}
		if (!click)
		{
			switch (state)
			{
			case GuiControlState::DISABLED:
				/*app->render->DrawRectangle(bounds, 200, 200, 200, 255, true, false);*/
				break;
			case GuiControlState::NORMAL:
				app->render->DrawTexture(img, bounds.x, bounds.y);
				break;
			case GuiControlState::FOCUSED:
				app->render->DrawTexture(img, bounds.x, bounds.y);
				break;
			case GuiControlState::PRESSED:
				app->render->DrawTexture(img, bounds.x, bounds.y);
				break;
			}app->render->DrawText(text.GetString(), bounds.x + 200, bounds.y - 375, bounds.w, bounds.h)
			/*app->render->DrawText(text.GetString(), bounds.x + 200, bounds.y - 375, bounds.w, bounds.h)*/;
		}
		if (click)
		{
			switch (state)
			{
			case GuiControlState::DISABLED:
				/*app->render->DrawRectangle(bounds, 200, 200, 200, 255, true, false);*/
				break;
			case GuiControlState::NORMAL:
				app->render->DrawTexture(img2, bounds.x, bounds.y);
				break;
			case GuiControlState::FOCUSED:
				app->render->DrawTexture(img2, bounds.x, bounds.y);
				break;
			case GuiControlState::PRESSED:
				app->render->DrawTexture(img2, bounds.x, bounds.y);
				break;
			}
			app->render->DrawText(text.GetString(), bounds.x + 200, bounds.y - 375, bounds.w, bounds.h);
		}
		//L15: DONE 4: Draw the button according the GuiControl State
		

		

		
		
	}

	else
	{
		app->render->DrawRectangle({ 0,0,0,0 }, 0, 20, 255, true, false);
	}
	return false;
}

