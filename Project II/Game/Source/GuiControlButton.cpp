#include "GuiControlButton.h"
#include "Render.h"
#include "App.h"
#include "Audio.h"
#include "GuiManager.h"
#include "SceneMenu.h"

GuiControlButton::GuiControlButton(uint32 id, SDL_Rect bounds, SDL_Texture* textureDisabled, SDL_Texture* textureNormal, SDL_Texture* textureFocused, SDL_Texture* texturePressed) : GuiControl(GuiControlType::BUTTON, id)
{
	this->bounds = bounds;
	this->text = text;
	this->textureDisabled = textureDisabled;
	this->textureNormal = textureNormal;
	this->textureFocused = textureFocused;
	this->texturePressed = texturePressed;

	canClick = true;
	drawBasic = false;
}

GuiControlButton::~GuiControlButton()
{

}

bool GuiControlButton::Update(float dt)
{
	GamePad& pad = app->input->pads[0];
	if (state != GuiControlState::HIDDEN)
	{
		// L15: DONE 3: Update the state of the GUiButton according to the mouse position
		app->input->GetMousePosition(mouseX, mouseY);

		//If the position of the mouse if inside the bounds of the button 
		if (state != GuiControlState::DISABLED) {
			if (/*mouseX > bounds.x && mouseX < bounds.x + bounds.w && mouseY > bounds.y && mouseY < bounds.y + bounds.h ||*/ app->sceneMenu->currentId == id) {

				state = GuiControlState::FOCUSED;

				if (/*app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN ||*/ (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN || pad.a == KEY_DOWN)) {
					state = GuiControlState::PRESSED;
				}

				if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP) {
					NotifyObserver();
				}
			}
			else {
				state = GuiControlState::NORMAL;
			}
		}

		if (app->guiManager->debugGui)
		{
			app->render->DrawRectangle({ bounds.x - 3 + app->render->camera.x, bounds.y - 3 - app->render->camera.y, bounds.w + 6, bounds.h + 6 }, 0, 255, 0, 100);
		}

		//draw button texture
		switch (state)
		{
		case GuiControlState::DISABLED:
			app->render->DrawTexture(textureDisabled, bounds.x, bounds.y, NULL, SDL_FLIP_NONE, 0);
			break;
		case GuiControlState::NORMAL:
			app->render->DrawTexture(textureNormal, bounds.x, bounds.y, NULL, SDL_FLIP_NONE, 0);
			break;
		case GuiControlState::FOCUSED:
			app->render->DrawTexture(textureFocused, bounds.x, bounds.y, NULL, SDL_FLIP_NONE, 0);
			break;
		case GuiControlState::PRESSED:
			app->render->DrawTexture(texturePressed, bounds.x, bounds.y, NULL, SDL_FLIP_NONE, 0);
			break;
		}

	}

	return false;
}

