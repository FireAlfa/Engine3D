#include "ModuleCamera3D.h"

#include "Application.h"
#include "ModuleInput.h"
#include "ModuleEditor.h"
#include "ComponentTransform.h"
#include "ComponentMesh.h"
#include "GameObject.h"

#include "Globals.h"



ModuleCamera3D::ModuleCamera3D(Application* app, bool startEnabled) : Module(app, startEnabled)
{

	right = float3(1.0f, 0.0f, 0.0f);
	up = float3(0.0f, 1.0f, 0.0f);
	front = float3(0.0f, 0.0f, 1.0f);

	position = float3(0.0f, 5.0f, -15.0f);
	reference = float3(0.0f, 0.0f, 0.0f);
	
	CalculateViewMatrix();

}

ModuleCamera3D::~ModuleCamera3D()
{}

// -----------------------------------------------------------------
bool ModuleCamera3D::Start()
{
	TTLOG("+++++ Loading Camera Module +++++\n");

	LookAt(float3::zero);

	bool ret = true;

	return ret;
}

// -----------------------------------------------------------------
bool ModuleCamera3D::CleanUp()
{
	TTLOG("+++++ Quitting Camera Module +++++\n");

	return true;
}

// -----------------------------------------------------------------
UpdateStatus ModuleCamera3D::Update(float dt)
{
	float3 newPos(0,0,0);
	float speed = cameraSpeed * dt;
	if(app->input->GetKey(SDL_SCANCODE_LSHIFT) == KeyState::KEY_REPEAT)
		speed *= 4.f;

	if(app->input->GetKey(SDL_SCANCODE_Q) == KeyState::KEY_REPEAT)
		newPos.y += speed;
	if(app->input->GetKey(SDL_SCANCODE_E) == KeyState::KEY_REPEAT)
		newPos.y -= speed;
		
	// Focus
	if (app->input->GetKey(SDL_SCANCODE_F) == KeyState::KEY_DOWN)
	{
		if(app->editor->gameobjectSelected != nullptr)
		{			
			if (ComponentMesh* mesh = app->editor->gameobjectSelected->GetComponent<ComponentMesh>())
			{
				const float3 meshCenter = mesh->GetCenterPointInWorldCoords();
				LookAt(meshCenter);
				const float meshRadius = mesh->GetSphereRadius();
				const float currentDistance = meshCenter.Distance(position);
				const float desiredDistance = (meshRadius * 2) / atan(cameraFrustum.horizontalFov);
				position = position + front * (currentDistance - desiredDistance);
			}
			else
			{
				LookAt(app->editor->gameobjectSelected->transform->GetPosition());
			}
		}
	}
	
	if(app->input->GetKey(SDL_SCANCODE_W) == KeyState::KEY_REPEAT)
		newPos += front * speed;
	if(app->input->GetKey(SDL_SCANCODE_S) == KeyState::KEY_REPEAT)
		newPos -= front * speed;


	if(app->input->GetKey(SDL_SCANCODE_A) == KeyState::KEY_REPEAT)
		newPos += right * speed;
	if(app->input->GetKey(SDL_SCANCODE_D) == KeyState::KEY_REPEAT)
		newPos -= right * speed;

	if (app->input->GetMouseZ() > 0) 
		newPos += front * speed * 4;
	if (app->input->GetMouseZ() < 0) 
		newPos -= front * speed * 4;

	position += newPos;

	// Mouse motion ----------------

	bool hasRotated = false;

	if(app->input->GetMouseButton(SDL_BUTTON_RIGHT) == KeyState::KEY_REPEAT)
	{
		int dx = -app->input->GetMouseXMotion();
		int dy = -app->input->GetMouseYMotion();

		if (app->input->GetKey(SDL_SCANCODE_LALT) == KeyState::KEY_REPEAT) {
			if (app->editor->gameobjectSelected != nullptr)
			{
				const float newDeltaX = (float)dx * cameraSensitivity;
				const float newDeltaY = (float)dy * cameraSensitivity;

				reference = app->editor->gameobjectSelected->transform->GetPosition();
				Quat orbitMat = Quat::RotateY(newDeltaX * .1f);								
				
				if (abs(up.y) < 0.3f) // Avoid gimball lock on up & down apex
				{
					if (position.y > reference.y && newDeltaY < 0.f)
						orbitMat = orbitMat * math::Quat::RotateAxisAngle(right, newDeltaY * .1f);
					if (position.y < reference.y && newDeltaY > 0.f)
						orbitMat = orbitMat * math::Quat::RotateAxisAngle(right, newDeltaY * .1f);
				}
				else
				{
					orbitMat = orbitMat * math::Quat::RotateAxisAngle(right, newDeltaY * .1f);
				}
				
				position = orbitMat * (position - reference) + reference;

				CalculateViewMatrix();
				LookAt(reference);
			}
		}
		else
		{

			if (dx != 0)
			{
				const float newDeltaX = (float)dx * cameraSensitivity;
				float deltaX = newDeltaX + 0.95f * (lastDeltaX - newDeltaX); // lerp for smooth rotation acceleration to avoid jittering
				lastDeltaX = deltaX;
				Quat rotateY = Quat::RotateY(up.y >= 0.f ? deltaX * .1f : -deltaX * .1f);
				up = rotateY * up;
				front = rotateY * front;
				CalculateViewMatrix();
				hasRotated = true;
			}

			if (dy != 0)
			{
				const float newDeltaY = (float)dy * cameraSensitivity;
				float deltaY = newDeltaY + 0.95f * (lastDeltaY - newDeltaY); // lerp for smooth rotation acceleration to avoid jittering
				lastDeltaY = deltaY;
				Quat rotateX = Quat::RotateAxisAngle(right, -deltaY * .1f);
				up = rotateX * up;
				front = rotateX * front;
				CalculateViewMatrix();
				hasRotated = true;
			}
		}
	}

	!hasRotated ? lastDeltaX = lastDeltaY = 0.f : 0.f;

	return UpdateStatus::UPDATE_CONTINUE;
}


// -----------------------------------------------------------------
void ModuleCamera3D::LookAt(const float3& point)
{		
	reference = point;

	front = (reference - position).Normalized();
	right = float3(0.0f, 1.0f, 0.0f).Cross(front).Normalized();
	up = front.Cross(right);

	CalculateViewMatrix();
}



// -----------------------------------------------------------------
void ModuleCamera3D::CalculateViewMatrix()
{
	if (projectionIsDirty)
		RecalculateProjection();

	cameraFrustum.pos = position;
	cameraFrustum.front = front.Normalized();
	cameraFrustum.up = up.Normalized();
	float3::Orthonormalize(cameraFrustum.front, cameraFrustum.up);
	right = up.Cross(front);
	viewMatrix = cameraFrustum.ViewMatrix();
}

void ModuleCamera3D::RecalculateProjection()
{
	cameraFrustum.type = FrustumType::PerspectiveFrustum;
	cameraFrustum.nearPlaneDistance = nearPlaneDistance;
	cameraFrustum.farPlaneDistance = farPlaneDistance;
	cameraFrustum.verticalFov = (verticalFOV * 3.141592 / 2) / 180.f;
	cameraFrustum.horizontalFov = 2.f * atanf(tanf(cameraFrustum.verticalFov * 0.5f) * aspectRatio);
}

void ModuleCamera3D::OnGui()
{
	if (ImGui::CollapsingHeader("Editor Camera"))
	{
		if (ImGui::DragFloat("Vertical fov", &verticalFOV))
		{
			projectionIsDirty = true;
		}
		if (ImGui::DragFloat("Near plane distance", &nearPlaneDistance))
		{
			projectionIsDirty = true;
		}
		if (ImGui::DragFloat("Far plane distance", &farPlaneDistance))
		{
			projectionIsDirty = true;
		}
	}
}

void ModuleCamera3D::OnSave(JSONWriter& writer) const
{
	writer.String("camera");	
	writer.StartObject();
	SAVE_JSON_FLOAT(verticalFOV)
	SAVE_JSON_FLOAT(nearPlaneDistance)
	SAVE_JSON_FLOAT(farPlaneDistance)
	SAVE_JSON_FLOAT(cameraSpeed)
	SAVE_JSON_FLOAT(cameraSensitivity)
	writer.EndObject();
}

void ModuleCamera3D::OnLoad(const JSONReader& reader)
{
	if (reader.HasMember("camera"))
	{
		const auto& config = reader["camera"];
		LOAD_JSON_FLOAT(verticalFOV);
		LOAD_JSON_FLOAT(nearPlaneDistance);
		LOAD_JSON_FLOAT(farPlaneDistance);
		LOAD_JSON_FLOAT(cameraSpeed);
		LOAD_JSON_FLOAT(cameraSensitivity);
	}
	RecalculateProjection();
}