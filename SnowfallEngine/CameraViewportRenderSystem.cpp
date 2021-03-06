#include "stdafx.h"

#include "CameraViewportRenderSystem.h"
#include "Snowfall.h"
#include "UIRenderer.h"

CameraViewportRenderSystem::CameraViewportRenderSystem()
{
}

CameraViewportRenderSystem::~CameraViewportRenderSystem()
{
	m_sampler.Destroy();
}

void CameraViewportRenderSystem::InitializeSystem(Scene& scene)
{
	m_quad = &AssetManager::LocateAssetGlobal<MeshAsset>("FullScreenQuad");
	m_scene = &scene;
}

void CameraViewportRenderSystem::Update(float deltaTime)
{
	CommandBuffer buffer;
	for (CameraComponent *camera : m_scene->GetComponentManager().GetComponents<CameraComponent>())
	{
		if (!camera->Enabled)
			continue;

		if (!camera->KeepInternal)
			CopyToSDR(buffer, camera);
	}
	buffer.ExecuteCommands();
}

std::string CameraViewportRenderSystem::GetName()
{
	return "CameraViewportRenderSystem";
}

std::vector<std::string> CameraViewportRenderSystem::GetSystemsBefore()
{
	return { "CameraComponent" };
}

std::vector<std::string> CameraViewportRenderSystem::GetSystemsAfter()
{
	return std::vector<std::string>();
}

bool CameraViewportRenderSystem::IsMainThread()
{
	return true;
}

void CameraViewportRenderSystem::CopyToSDR(CommandBuffer& buffer, CameraComponent *camera)
{
	Pipeline pipe;

	pipe.Shader = AssetManager::LocateAssetGlobal<ShaderAsset>("CopyToSDR").GetShaderVariant({});

	pipe.VertexStage.VertexArray = Snowfall::GetGameInstance().GetMeshManager().GetVertexArray();
	pipe.VertexStage.FrontFaceCulling = false;
	pipe.VertexStage.BackFaceCulling = false;

	pipe.FragmentStage.DrawTargets = { 0 };
	pipe.FragmentStage.DepthTest = false;
	pipe.FragmentStage.DepthMask = false;

	if (camera->RenderTarget)
		pipe.FragmentStage.Framebuffer = camera->RenderTarget->GetFramebuffer();
	pipe.FragmentStage.Viewport = camera->Region;

	ShaderConstants consts;

	consts.AddConstant(0, camera->HdrBuffer->GetTexture(camera->colorAttachment)->GetTextureObject(), m_sampler);

	buffer.BindPipelineCommand(pipe);
	buffer.BindConstantsCommand(consts);
	m_quad->DrawMeshDirect(buffer);
}