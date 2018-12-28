#include <Windows.h>

#include "Snowfall.h"
#include "BasicCamera.h"
#include "MeshAsset.h"

class MyEntity : public Entity
{
public:
	DEFINE_PROTOTYPE(MyEntity)
	MyEntity() : Entity("MyEntity", EntityOptions(false, true, false, true, false, true, false))
	{
	}

	virtual void OnSceneAddition() override
	{
		material.MaterialShader = &Snowfall::GetGameInstance().GetAssetManager().LocateAsset<ShaderAsset>("SimplePBRShader");
		SetMesh(Snowfall::GetGameInstance().GetAssetManager().LocateAsset<MeshAsset>("Assets.Monkey.Suzanne").GetMesh());
		SetMaterial(material);

		Scale = glm::vec3(0.25f, 0.25f, 0.25f);
	}

	virtual void OnUpdate(float deltaTime) override
	{
		Rotation += glm::vec3(0.5f, 0.5f, 0) * deltaTime;
	}

private:
	Material material;
};

class MyCamera : public BasicCamera
{
public:
	DEFINE_PROTOTYPE(MyCamera)
	MyCamera()
	{
		SetName("MyCamera");
		SetClipping(0.3f, 1000.f);
		SetAspect(1.33f);
		SetFOV(1.57f);
	}

	virtual void OnSceneAddition() override
	{
		SetRegion(Quad2D(glm::vec2(0, 0), glm::vec2(800, 600)));
		SetRenderTarget(Framebuffer::GetDefault());
	}

	virtual void OnUpdate(float deltaTime) override
	{
	}
};

class MySceneConfig : public SceneConfiguration
{
public:
	MySceneConfig()
	{
		AddEntityPrototype<MyEntity>();
		AddEntityPrototype<MyCamera>();
	}
};

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	Snowfall::InitGlobalInstance();

	Scene scene(new MySceneConfig);
	
	scene.AddEntity("MyEntity").Position = glm::vec3(-1, 0, -2);
	scene.AddEntity("MyEntity").Position = glm::vec3(0, 0, -2);
	scene.AddEntity("MyEntity").Position = glm::vec3(1, 0, -2);
	scene.AddEntity("MyEntity").Position = glm::vec3(0, 1, -2);

	MyCamera& camera = scene.AddEntity<MyCamera>();
	scene.AddCamera(camera);

	Snowfall::GetGameInstance().SetScene(scene);
	Snowfall::GetGameInstance().StartGame();

	return 0;
}

