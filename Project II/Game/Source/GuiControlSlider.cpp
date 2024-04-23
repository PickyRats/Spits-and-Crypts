#include "GuiControlSlider.h"
#include "Render.h"
#include "App.h"
#include "Audio.h"
#include "Log.h"
#include "GuiManager.h"

GuiControlSlider::GuiControlSlider(uint32 id, SDL_Rect bounds, SDL_Texture* textureDisabled, SDL_Texture* textureNormal, SDL_Texture* textureFocused, SDL_Texture* texturePressed, SDL_Rect sliderBounds) : GuiControl(GuiControlType::BUTTON, id)
{
    this->bounds = bounds;
    this->text = text;
    this->textureDisabled = textureDisabled;
    this->textureNormal = textureNormal;
    this->textureFocused = textureFocused;
    this->texturePressed = texturePressed;
    this->sliderBounds = sliderBounds;

    canClick = true;
    drawBasic = false;

    
}

GuiControlSlider::~GuiControlSlider()
{

}

bool GuiControlSlider::Update(float dt)
{
    sliderPosition = bounds.x;
   
    if (state != GuiControlState::HIDDEN)
    {

        app->input->GetMousePosition(mouseX, mouseY);

       // if position of mouse is inside the bounds of the button, change state
        if (mouseX > sliderBounds.x && mouseX < sliderBounds.x + sliderBounds.w - bounds.w && mouseY > bounds.y && mouseY < bounds.y + bounds.h) {
            state = GuiControlState::FOCUSED;

            // if mouse button is pressed, change state and set initial mouse position
            if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN) {
                state = GuiControlState::PRESSED;
                initialMouseX = mouseX;
            }

            // if mouse button is pressed, change state and set initial mouse position
            if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT) {
                state = GuiControlState::PRESSED;
                bounds.x = mouseX;
            }

            // if mouse button is released, change state and notify observer
            if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP) {
                NotifyObserver();
            }
        }
        else {
            state = GuiControlState::NORMAL;
        }

        if (app->guiManager->debugGui)
        {
            app->render->DrawRectangle({ bounds.x - 6 + app->render->camera.x, bounds.y - 3 - app->render->camera.y, bounds.w + 6, bounds.h + 6 }, 0, 0, 255, 100);
        }

        //draw slider texture
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
            app->render->DrawTexture(textureFocused, bounds.x, bounds.y, NULL, SDL_FLIP_NONE, 0);
            break;
        }
    }

    return false;
}
