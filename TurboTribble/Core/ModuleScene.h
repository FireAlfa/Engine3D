#ifndef __MODULE_SCENE_H__
#define __MODULE_SCENE_H__

#include "Module.h"
#include "ModuleImport.h"
#include "GameObject.h"

#include "Globals.h"



class ModuleScene : public Module
{
public:

	ModuleScene(Application* app, bool startEnabled = true);

	bool Start() override;
	UpdateStatus Update(float dt) override;
	bool CleanUp() override;

	GameObject* CreateGameObject(GameObject* parent = nullptr);
	GameObject* CreateGameObject(const std::string name, GameObject* parent = nullptr);

public:

	GameObject* root;
};


#endif // !__MODULE_SCENE_H__