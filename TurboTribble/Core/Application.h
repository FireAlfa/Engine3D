#ifndef __APPLICATION_H__
#define __APPLICATION_H__

#include <vector>

//#include "Globals.h"
#include "Timer.h"
#include "PerfTimer.h"



// Forward declarations

class Module;
class ModuleWindow;
class ModuleInput;
class ModuleScene;
class ModuleRenderer3D;
class ModuleCamera3D;
class ModuleEditor;
class ModuleViewportFrameBuffer;
class ModuleImport;
class ModuleFileSystem;
class ModuleTextures;

class Application
{
public:

	ModuleWindow* window { nullptr };
	ModuleInput* input { nullptr };
	ModuleScene* scene { nullptr };
	ModuleRenderer3D* renderer3D { nullptr };
	ModuleCamera3D* camera { nullptr };
	ModuleEditor* editor { nullptr };
	ModuleViewportFrameBuffer* viewportBuffer { nullptr };
	ModuleImport* import { nullptr };
	ModuleFileSystem* fileSystem { nullptr };
	ModuleTextures* textures { nullptr };

	Application();
	~Application();

	bool Init();
	void LoadEngineConfig();
	UpdateStatus Update();
	bool CleanUp();


	void SaveEngineConfig();

	void DrawFPSDiagram();
	void DrawHardwareConsole();


	void AddModule(Module* mod);
	void PrepareUpdate();
	void FinishUpdate();

	void OnGui();


	// FPS core
	float				fps;
	float				dt;
	int					cap;
	PerfTimer			ptimer;
	Timer				msTimer;
	Timer				frameTime;
	uint64				frameCount = 0;
	Timer				startupTime;
	Timer				lastSecFrameTime;
	uint32				lastSecFrameCount = 0;
	uint32				prevLastSecFrameCount = 0;
	

	std::vector<float> fpsLog;
	std::vector<float> msLog;

	//Engine configuration
	bool closeEngine;
	bool vsync;



private:

	std::vector<Module*> modules;

};

extern Application* app;


#endif // !__APPLICATION_H__