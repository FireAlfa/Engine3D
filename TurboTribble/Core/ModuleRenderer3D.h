#ifndef __MODULE_RENDERER_3D_H__
#define __MODULE_RENDERER_3D_H__

#include "Module.h"

#include "Globals.h"
#include "Light.h"
#include "SDL/include/SDL.h"

#define MAX_LIGHTS 8



class ModuleRenderer3D : public Module
{
public:

	ModuleRenderer3D(Application* app, bool startEnabled = true);
	~ModuleRenderer3D();

	bool Init() override;
	bool Start() override;
	UpdateStatus PreUpdate(float dt) override;
	UpdateStatus PostUpdate(float dt) override;
	bool CleanUp() override;

	void OnResize(int width, int height);

	void OnGui() override;
	void OnLoad(const JSONReader& reader) override;
	void OnSave(JSONWriter& writer) const override;
	void DrawingModes(bool currentState, int glMode);

public:

	Light lights[MAX_LIGHTS];
	SDL_GLContext context;

	bool depthTestEnabled;
	bool cullFace;
	bool useLighting;
	bool useTexture;
	bool wireframeMode;
	bool vsyncActive;

};

#endif // !__MODULE_RENDERER_3D_H__