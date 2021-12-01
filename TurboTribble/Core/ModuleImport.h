#ifndef __MODULE_IMPORT_H__
#define __MODULE_IMPORT_H__

#include "Module.h"

#include <string>



class ComponentMesh;
struct aiScene;

class ModuleImport : public Module
{
public:

	ModuleImport(Application* app, bool startEnabled = true);

	bool Init() override;
	UpdateStatus Update(float dt) override;
	bool CleanUp() override;

	bool LoadGeometry(const char* path);

	void FindNodeName(const aiScene* scene, const size_t i, std::string& name);

};

#endif // !__MODULE_IMPORT_H__