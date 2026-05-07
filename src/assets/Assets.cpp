#include "Assets.h"
#include "utils/Logger.h"

#include <iostream>
#include <fstream>


Assets::Assets()
{
	s_instance = this;
	while (!std::filesystem::exists(m_assetsPath / "assets") && m_assetsPath != m_assetsPath.root_path()) {
		m_assetsPath = m_assetsPath.parent_path();
	}
	if (m_assetsPath != m_assetsPath.root_path()) {
		m_assetsPath /= "assets";
	}
}

Assets::~Assets()
{

}

std::string Assets::LoadText(const std::string_view& filePath)
{
	std::filesystem::path path = GetAssetPath(filePath);
	if (!std::filesystem::exists(path)) {
		LLog("ERROR: File not found! {}", path.string());
		return "";
	}
	std::ifstream fs(path.c_str(), std::ios::binary | std::ios::ate);
	if (fs.is_open()) {
		size_t size = (size_t)fs.tellg();
		std::string buffer(size, ' ');
		fs.seekg(std::ios::beg);
		fs.read(buffer.data(), size);
		return buffer;
	}
	return "";
}

std::filesystem::path Assets::GetAssetPath(const std::string_view& filePath) const
{
	std::filesystem::path path{ filePath };
	if (!path.is_absolute()) {
		path = m_assetsPath / path;
	}
	return path;
}
