#pragma once
#include "Module.h"
#include "Globals.h"

#define MAX_MOUSE_BUTTONS 5

enum class KeyState
{
	KEY_IDLE = 0,
	KEY_DOWN,
	KEY_REPEAT,
	KEY_UP
};

class ModuleInput : public Module
{
public:
	
	ModuleInput(Application* app, bool startEnabled = true);
	~ModuleInput();

	bool Init() override;
	UpdateStatus PreUpdate(float dt) override;
	bool CleanUp() override;

	void OnGui();
	void ImGuiEventInput();

	KeyState GetKey(int id) const
	{
		return keyboard[id];
	}

	KeyState GetMouseButton(int id) const
	{
		return mouseButtons[id];
	}

	int GetMouseX() const
	{
		return mouseX;
	}

	int GetMouseY() const
	{
		return mouseY;
	}

	int GetMouseZ() const
	{
		return mouseZ;
	}

	int GetMouseXMotion() const
	{
		return mouseXMotion;
	}

	int GetMouseYMotion() const
	{
		return mouseYMotion;
	}


private:
	KeyState* keyboard;
	KeyState mouseButtons[MAX_MOUSE_BUTTONS];
	int mouseX;
	int mouseY;
	int mouseZ;
	int mouseXMotion;
	int mouseYMotion;
	int mouseZMotion;

public:
	const char* filePath;
};