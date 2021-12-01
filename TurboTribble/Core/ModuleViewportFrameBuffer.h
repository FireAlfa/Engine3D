#ifndef __MODULE_VIEWPORT_FRAME_BUFFER_H__
#define __MODULE_VIEWPORT_FRAME_BUFFER_H__

#include "Module.h"

#include "Globals.h"

#include <string>



class ModuleViewportFrameBuffer : public Module
{
public:

	ModuleViewportFrameBuffer(Application* app, bool startEnabled = true);
	~ModuleViewportFrameBuffer();

	bool Init() override;
	bool Start() override;
	UpdateStatus PreUpdate(float dt) override;
	UpdateStatus PostUpdate(float dt) override;
	bool CleanUp();

public:

	uint frameBuffer = 0;
	uint renderBufferoutput = 0;
	uint texture = 0;
	bool showViewportWindow = true;

};

#endif // !__MODULE_VIEWPORT_FRAME_BUFFER_H__