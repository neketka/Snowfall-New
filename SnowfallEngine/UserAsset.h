#pragma once
#include "AssetManager.h"
#include "MemoryAssetStream.h"
#include <vector>
#include "export.h"

class UserAsset : public IAsset
{
public:
	SNOWFALLENGINE_API UserAsset(std::string path, IAssetStreamIO *stream);
	SNOWFALLENGINE_API virtual ~UserAsset() override;

	SNOWFALLENGINE_API virtual std::string GetPath() const override;
	SNOWFALLENGINE_API virtual void SetStream(IAssetStreamIO *stream) override;
	SNOWFALLENGINE_API virtual void Load() override;
	SNOWFALLENGINE_API virtual void Unload() override;
	SNOWFALLENGINE_API virtual bool IsReady() override;
	SNOWFALLENGINE_API virtual bool IsValid() override;

	SNOWFALLENGINE_API void SetData(char *data, int offset, int length);
	SNOWFALLENGINE_API char *GetData();

	SNOWFALLENGINE_API IAssetStreamIO *GetMemoryStream();
	SNOWFALLENGINE_API IAssetStreamIO *GetIOStream();

	SNOWFALLENGINE_API int GetDataSize();
	SNOWFALLENGINE_API void SetDataSize(int bytes);

	SNOWFALLENGINE_API virtual IAsset *CreateCopy(std::string newPath) override;
	SNOWFALLENGINE_API virtual void Export() override;
private:
	std::vector<char> m_data;
	bool m_loaded;
	std::string m_path;
	IAssetStreamIO *m_stream;
	MemoryAssetStream *m_memStream;
};

