#include "TestComponent.h"

#include <Scene.h>
#include <TransformComponent.h>
#include <MeshComponent.h>

std::vector<SerializationField> ComponentDescriptor<TestComponent>::GetSerializationFields()
{
	return {
	};
}

void TestSystem::InitializeSystem(Scene& scene)
{
	m_scene = &scene;
}

float t = 0;
void TestSystem::Update(float deltaTime)
{
	bool doDelete = t > 1.0;
	for (TestComponent *comp : m_scene->GetComponentManager().GetComponents<TestComponent>())
	{
		TransformComponent *transform = comp->Owner.GetComponent<TransformComponent>();
		transform->Rotation += glm::vec3(deltaTime, deltaTime * 0.5f, 0);
		if (doDelete)
		{
			float rnd1 = std::rand() % 100000 / 10000.f - 2.5f;
			float rnd2 = std::rand() % 100000 / 10000.f - 2.5f;
			float rnd3 = std::rand() % 100000 / 10000.f - 7.0f;

			doDelete = false;

			Entity e = comp->Owner.Clone();

			e.GetComponent<TransformComponent>()->Parent = comp->Owner;
			e.GetComponent<TransformComponent>()->Position = glm::vec3(rnd1, rnd2, rnd3);
			e.GetComponent<MeshRenderComponent>()->Batch = nullptr;

			t = 0;
		}
	}
	t += deltaTime;
}

std::string TestSystem::GetName()
{
	return "TestSystem";
}

std::vector<std::string> TestSystem::GetSystemsBefore()
{
	return std::vector<std::string>();
}

std::vector<std::string> TestSystem::GetSystemsAfter()
{
	return std::vector<std::string>();
}

bool TestSystem::IsMainThread()
{
	return false;
}
