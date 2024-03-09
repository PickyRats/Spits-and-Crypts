#ifndef __MODULE_H__
#define __MODULE_H__

#include "SString.h"

#include "PugiXml/src/pugixml.hpp"

class App;
class GuiControl;

class Module
{
public:

	Module(bool enabled = true) : active(enabled)
	{}

	void Init()
	{
		/*if (!active)
		{
			active = true;
		}*/
	}

	// Called before render is available
	virtual bool Awake(pugi::xml_node&)
	{
		return true;
	}

	// Called before the first frame
	virtual bool Start()
	{
		return true;
	}

	// Called each loop iteration
	virtual bool PreUpdate()
	{
		return true;
	}

	// Called each loop iteration
	virtual bool Update(float dt)
	{
		return true;
	}

	// Called each loop iteration
	virtual bool PostUpdate()
	{
		return true;
	}

	// Called before quitting
	virtual bool CleanUp()
	{
		return true;
	}

	void Module::Enable()
	{
		if (!active)
		{
			active = true;
			Start();
		}
	}

	void Module::Disable()
	{
		if (active)
		{
			active = false;
			CleanUp();
		}
	}

	virtual bool OnGuiMouseClickEvent(GuiControl* control)
	{
		return true;
	}

	virtual bool LoadState(pugi::xml_node node)
	{
		return true;
	}

	// Called when we want to save data from XML
	virtual bool SaveState(pugi::xml_node node)
	{
		return true;
	}

public:

	SString name;
	bool active;

};

#endif // __MODULE_H__