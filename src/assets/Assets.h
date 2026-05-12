#pragma once
#include <filesystem>
#include <string>
#include <string_view>
#include <vector>


struct TextureData
{
	unsigned int width;
	unsigned int height;
	std::vector<std::byte> buffer;
};


class Assets {
public:
	Assets();
	~Assets();

	static inline Assets& Get() { return *s_instance; }

	std::string LoadText(const std::string_view& filePath);
	TextureData LoadTextureData(const std::string_view& filePath);

private:
	std::filesystem::path GetAssetPath(const std::string_view& filePath) const;

private:
	static inline Assets* s_instance = nullptr;
	std::filesystem::path m_assetsPath = std::filesystem::current_path();

};
