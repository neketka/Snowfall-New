#pragma once
#include "AssetManager.h"
#include "TextureAsset.h"
#include "Framebuffer.h"
#include "Pipeline.h"

#include "export.h"

class TextureLayerAttachment
{
public: 
	TextureLayerAttachment() {}
	TextureLayerAttachment(int tIndex, int level, int layer) : TextureIndex(tIndex), Level(level), Layer(layer) {}
	int TextureIndex = 0;
	int Level = 0;
	int Layer = -1;
};

class RenderTargetAsset : public IAsset
{
public:
	SNOWFALLENGINE_API RenderTargetAsset(std::string path, std::vector<TextureAsset *> textures, std::vector<TextureAsset *> newTextures, std::vector<TextureLayerAttachment> attachments, bool deleteTex=true);
	SNOWFALLENGINE_API RenderTargetAsset(std::string path, IAssetStreamIO *stream);
	SNOWFALLENGINE_API virtual ~RenderTargetAsset() override;
	virtual std::string GetPath() const override 
	{
		return m_path;
	}
	SNOWFALLENGINE_API virtual void SetStream(IAssetStreamIO *stream) override;

	inline TextureAsset *GetTexture(int index)
	{
		return m_textures[index];
	}

	SNOWFALLENGINE_API Framebuffer GetFramebuffer();

	SNOWFALLENGINE_API virtual void Load() override;
	SNOWFALLENGINE_API virtual void Unload() override;
	SNOWFALLENGINE_API virtual bool IsReady() override;
	SNOWFALLENGINE_API virtual bool IsValid() override;

	SNOWFALLENGINE_API void BuildPipeline(Pipeline& pipeline);

	SNOWFALLENGINE_API virtual IAsset *CreateCopy(std::string newPath) override;
	SNOWFALLENGINE_API virtual void Export() override;
private:
	bool m_loaded;

	Framebuffer m_fbo;
	std::vector<TextureAsset *> m_textures;
	std::vector<TextureAsset *> m_newTextures;
	std::vector<TextureLayerAttachment> m_attachments;

	IAssetStreamIO *m_stream;
	std::string m_path;
};

class SNOWFALLENGINE_API RenderTargetAssetReader : public IAssetReader
{
public:
	virtual std::vector<std::string> GetExtensions() override;
	virtual void LoadAssets(std::string ext, IAssetStreamIO *stream, AssetManager& assetManager) override;
};