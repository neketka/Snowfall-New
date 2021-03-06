#pragma once
#include "AssetManager.h"
#include "Shader.h"
#include "ShaderPreprocessor.h"
#include <map>
#include <set>

#include "export.h"

class ShaderAsset : public IAsset
{
public:
	SNOWFALLENGINE_API ShaderAsset(std::string path, std::string src);
	SNOWFALLENGINE_API ShaderAsset(std::string path, IAssetStreamIO *stream);
	SNOWFALLENGINE_API virtual ~ShaderAsset() override;

	inline std::string GetSource() 
	{
		if (!IsReady())
			Load(); 
		return m_preprocessed.GetProcessedSource(); 
	}

	inline PreprocessedShader GetPreprocessed()
	{
		if (!IsReady())
			Load();
		return m_preprocessed;
	}

	SNOWFALLENGINE_API Shader& GetShaderVariant(std::set<std::string> qualifiers);

	virtual std::string GetPath() const override 
	{
		return m_path;
	}

	SNOWFALLENGINE_API virtual void SetStream(IAssetStreamIO *stream) override;

	SNOWFALLENGINE_API virtual void Load() override;
	SNOWFALLENGINE_API virtual void Unload() override;
	SNOWFALLENGINE_API virtual bool IsReady() override;
	SNOWFALLENGINE_API virtual bool IsValid() override;

	SNOWFALLENGINE_API virtual IAsset *CreateCopy(std::string newPath) override;
	SNOWFALLENGINE_API virtual void Export() override;
private:
	bool m_loaded;
	bool m_compileSuccess;
	bool m_isStreamedSource;

	IAssetStreamIO *m_stream;
	std::string m_path;
	std::string m_rawSource;
	std::map<std::set<std::string>, Shader *> m_compiledShaders;
	PreprocessedShader m_preprocessed;
};

class SNOWFALLENGINE_API ShaderAssetReader : public IAssetReader
{
public:
	ShaderAssetReader();

	virtual std::vector<std::string> GetExtensions() override;
	virtual void LoadAssets(std::string ext, IAssetStreamIO *stream, AssetManager& assetManager) override;
};