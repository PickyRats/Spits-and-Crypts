#include "GuiManager.h"
#include "App.h"
#include "Textures.h"

#include "GuiControlButton.h"
#include "GuiControlCheckBox.h"
#include "GuiControlSlider.h"

#include "Audio.h"

GuiManager::GuiManager() :Module()
{
	name.Create("guiManager");
}

GuiManager::~GuiManager() {}

bool GuiManager::Start()
{
	return true;
}

// L15: DONE1: Implement CreateGuiControl function that instantiates a new GUI control and add it to the list of controls
		//It has different textures for each state. Don't draw the button if it's hidden and not working but draw it if it's disabled
GuiControl* GuiManager::CreateGuiControl(GuiControlType type, int id, SDL_Texture* textureDisabled, SDL_Texture* textureNormal, SDL_Texture* textureFocused, SDL_Texture* texturePressed, SDL_Rect bounds, Module* observer, SDL_Rect sliderBounds)
{
	GuiControl* guiControl = nullptr;

	//Call the constructor according to the GuiControlType
	switch (type)
	{
	case GuiControlType::BUTTON:
		guiControl = new GuiControlButton(id, bounds, textureDisabled,textureNormal, textureFocused, texturePressed);
		break;
	case GuiControlType::CHECKBOX:
		guiControl = new GuiControlCheckBox(id, bounds, textureDisabled, textureNormal, textureFocused, texturePressed);
		break;
	case GuiControlType::SLIDER:
		guiControl = new GuiControlSlider(id, bounds, textureDisabled, textureNormal, textureFocused, texturePressed, sliderBounds);
		break;
	}

	//Set the observer
	guiControl->observer = observer;

	// Created GuiControls are add it to the list of controls
	guiControlsList.Add(guiControl);

	return guiControl;
}

bool GuiManager::Update(float dt)
{	

	ListItem<GuiControl*>* control = guiControlsList.start;

	while (control != nullptr)
	{
		control->data->Update(dt);
		control = control->next;
	}

	// enable or disable debug gui
	if (app->input->GetKey(SDL_SCANCODE_F8) == KEY_DOWN)
	{
		debugGui = !debugGui;
	}

	return true;
}

bool GuiManager::CleanUp()
{
	ListItem<GuiControl*>* control = guiControlsList.start;

	while (control != nullptr)
	{
		RELEASE(control);
	}

	return true;

	return false;
}



