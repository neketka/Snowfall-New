#include <Windows.h>

#include <Snowfall.h>
#include <TextureAsset.h>
#include <MeshAsset.h>

#include <TransformComponent.h>
#include <MeshComponent.h>
#include <CameraComponent.h>
#include <LightComponent.h>
#include <ShadowMapRenderSystem.h>

#include "CameraFlyComponent.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	EngineSettings settings;

	settings.TextureUnits = 84;
	settings.ImageUnits = 32;
	settings.MaxMeshMemoryBytes = 1000000000;
	settings.MaxMeshCommands = 200000;
	settings.ShadowMapResolution = 1024;
	settings.CloseShadowMapResolution = 4096;

	Snowfall::InitGlobalInstance(settings);
	Snowfall::GetGameInstance().GetPrototypeManager().AddComponentDescription<CameraFlyComponent>();

	Sampler sampler;

	Material material;
	material.MaterialShader = &AssetManager::LocateAssetGlobal<ShaderAsset>("SimplePBRShader");
	material.PerObjectParameterCount = 1;

	Snowfall::GetGameInstance().GetAssetManager().AddAsset(new MaterialAsset("FortMaterial", { sampler }, 
		{ &AssetManager::LocateAssetGlobal<TextureAsset>("fortnite") }, { SamplerProperty(0, 0, 5) }, material));

	Scene scene;

	scene.GetSystemManager().AddSystem(new TransformSystem);
	scene.GetSystemManager().AddSystem(new MeshRenderingSystem);
	scene.GetSystemManager().AddSystem(new ShadowMapRenderSystem);
	scene.GetSystemManager().AddSystem(new LightSystem);
	scene.GetSystemManager().AddSystem(new CameraSystem);
	scene.GetSystemManager().AddSystem(new CameraFlySystem);

	scene.GetSystemManager().GetSystem("CameraSystem")->SetEnabled(true);
	scene.GetSystemManager().GetSystem("TransformSystem")->SetEnabled(true);
	scene.GetSystemManager().GetSystem("MeshRenderingSystem")->SetEnabled(true);
	scene.GetSystemManager().GetSystem("CameraFlySystem")->SetEnabled(true);
	scene.GetSystemManager().GetSystem("ShadowMapRenderSystem")->SetEnabled(true);
	scene.GetSystemManager().GetSystem("LightSystem")->SetEnabled(true);

	Entity camera = scene.GetEntityManager().CreateEntity({ "TransformComponent", "CameraComponent", "CameraFlyComponent" });
	camera.GetComponent<CameraComponent>()->LockToTargetResolution = true;
	camera.GetComponent<CameraComponent>()->FovY = 90.f;

	for (int i = 0; i != 15; ++i)
	{
		Entity e = scene.GetEntityManager().CreateEntity({ "TransformComponent", "MeshRenderComponent"});

		float rnd1 = std::rand() % 100000 / 10000.f - 2.5f;
		float rnd2 = std::rand() % 100000 / 10000.f - 2.5f;
		float rnd3 = std::rand() % 100000 / 10000.f - 7.0f;

		float rnd4 = std::rand() % 100000 / 100000.f;

		e.GetComponent<TransformComponent>()->Position = glm::vec3(rnd1, rnd2, rnd3);
		e.GetComponent<TransformComponent>()->Scale = glm::vec3(0.5f, 0.5f, 0.5f);
		e.GetComponent<MeshRenderComponent>()->Material = &AssetManager::LocateAssetGlobal<MaterialAsset>("FortMaterial");
		e.GetComponent<MeshRenderComponent>()->Mesh = &AssetManager::LocateAssetGlobal<MeshAsset>("Cube.Cube_Cube.002");
		e.GetComponent<MeshRenderComponent>()->ObjectParameters = { glm::vec4(0.f, 0.2f, 1, 1) };
		e.GetComponent<MeshRenderComponent>()->BatchingType = BatchingType::Instanced;
	}

	Entity light = scene.GetEntityManager().CreateEntity({ "TransformComponent", "LightComponent" });

	light.GetComponent<TransformComponent>()->Position = glm::vec3(0, 2, 2);
	light.GetComponent<TransformComponent>()->Rotation = glm::vec3(45, 0, 0);
	light.GetComponent<LightComponent>()->Enabled = true;
	light.GetComponent<LightComponent>()->Shadowing = true;
	light.GetComponent<LightComponent>()->Type = LightType::Directional;
	light.GetComponent<LightComponent>()->Color = glm::vec3(1, 1, 1);
	light.GetComponent<LightComponent>()->Intensity = 10.f;
	light.GetComponent<LightComponent>()->Range = 10.f;
	light.GetComponent<LightComponent>()->RangeCutoff = 0.005f;

	scene.GetSystemManager().InitializeSystems();

	Snowfall::GetGameInstance().SetScene(scene);
	Snowfall::GetGameInstance().StartGame();

	return 0;
}

