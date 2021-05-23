#include "TestApp.h"

#include <iostream>

#include "MapEditorSystem.h"
#include "MovementSystem.h"
#include "Core/EntitySystem.h"
#include "Graphics/Camera.h"
#include "Graphics/CameraSystem.h"
#include "Graphics/RenderSystem.h"
#include "Notie.h"

void TestApp::Setup(Notie& engine)
{	
	EntitySystem& entitySystem = EntitySystem::GetInstance();

	CameraSystem* cameraSystem = new CameraSystem();
	entitySystem.AddSystem(*cameraSystem);

	EntityId mainCam = entitySystem.AddEntity();
	{
		Camera* cam = entitySystem.AddComponent<Camera>(mainCam);
		Window* activeWindow = RenderSystem::GetActiveWindow();
		cam->Resize(Vector2Float(0, 0), activeWindow->GetSize());
		activeWindow->SetCamera(*cam);
	}

	//MovementSystem* movementSystem = new MovementSystem();
	//entitySystem.AddSystem(*movementSystem);

	//for (int i = 0; i < 10000; i++)
	//{
	//	EntityId entity = entitySystem.AddEntity();
	//	{
	//		Sprite* sprite = entitySystem.AddComponent<Sprite>(entity);
	//		MovementComponent* movement = entitySystem.AddComponent<MovementComponent>(entity);
	//		sprite->SetTexture("tree");
	//		sprite->SetPosition(rand() % 1920, rand() % 1080);
	//		movement->Speed = rand() % 50 + 1000;
	//	}
	//}
	
	MapEditorSystem* editorSystem = new MapEditorSystem(*engine.GetIsoEngine());
	entitySystem.AddSystem(*editorSystem);
}
