#include "Assets.h"
#include "utils/Logger.h"

#include <iostream>
#include <fstream>
#include <Windows.h>
#include <wincodec.h>


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

TextureData Assets::LoadTextureData(const std::string_view& filePath)
{
	TextureData textureData{ 0 };
	std::filesystem::path path = GetAssetPath(filePath);
	if (!std::filesystem::exists(path)) {
		LLog("ERROR: File not found! {}", path.string());
		return textureData;
	}
	HRESULT hr = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
	IWICImagingFactory* factory = nullptr;
	hr = CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&factory));
	if (FAILED(hr)) {
		LLog("WIC Factory create failed!");
		return textureData;
	}
	IWICBitmapDecoder* decoder = nullptr;
	hr = factory->CreateDecoderFromFilename(path.c_str(), nullptr, GENERIC_READ, WICDecodeMetadataCacheOnDemand, &decoder);
	if (FAILED(hr)) {
		LLog("Not supporting file format!");
		factory->Release();
		return textureData;
	}
	IWICBitmapFrameDecode* frame = nullptr;
	hr = decoder->GetFrame(0, &frame);
	if (FAILED(hr)) {
		LLog("Can\'t read frame!");
		decoder->Release();
		factory->Release();
		return textureData;
	}
	UINT w = 0, h = 0;
	frame->GetSize(&w, &h);
	IWICFormatConverter* converter = nullptr;
	hr = factory->CreateFormatConverter(&converter);
	hr = converter->Initialize(frame, GUID_WICPixelFormat32bppRGBA, WICBitmapDitherTypeNone, nullptr, 0.f, WICBitmapPaletteTypeCustom);
	if (FAILED(hr)) {
		LLog("Format convertation failed!");
		frame->Release();
		decoder->Release();
		factory->Release();
		return textureData;
	}
	UINT bufferSize = w * h * 4;
	textureData.buffer.resize(bufferSize);
	hr = converter->CopyPixels(nullptr, w * 4, bufferSize, (BYTE*)textureData.buffer.data());
	converter->Release();
	frame->Release();
	decoder->Release();
	factory->Release();
	if (FAILED(hr)) {
		LLog("Copy pixels failed!");
		return textureData;
	}
	textureData.width = w;
	textureData.height = h;
	return textureData;
}

std::filesystem::path Assets::GetAssetPath(const std::string_view& filePath) const
{
	std::filesystem::path path{ filePath };
	if (!path.is_absolute()) {
		path = m_assetsPath / path;
	}
	return path;
}
