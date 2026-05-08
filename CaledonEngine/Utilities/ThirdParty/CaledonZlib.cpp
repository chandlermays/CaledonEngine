#include "CaledonZLib.h"

#include <fstream>
#include <iostream>

#define ZLIB_WINAPI
#include "zlib/zlib.h"

#pragma pack(1)
struct ZipHeader
{
	uint32_t signature = 0;
	uint16_t diskIndex = 0;
	uint16_t startDisk = 0;
	uint16_t numFiles = 0;
	uint16_t totalFiles = 0;
	uint32_t dirSize = 0;
	uint32_t dirOffset = 0;
	uint16_t commentLength = 0;
};
#pragma pack()

static constexpr uint32_t kZipSignature = 101010256;

#pragma pack(1)
struct FileHeader
{
	uint32_t signature = 0;
	uint16_t versionMode = 0;
	uint16_t versionNeeded = 0;
	uint16_t flag = 0;
	uint16_t compression = 0;
	uint16_t time = 0;
	uint16_t date = 0;
	uint32_t crcCode = 0;
	uint32_t compressedSize = 0;
	uint32_t uncompressedSize = 0;
	uint16_t nameLength = 0;
	uint16_t extraLength = 0;
	uint16_t commentLength = 0;
	uint16_t startDisk = 0;
	uint16_t internalAttributes = 0;
	uint32_t externalAttributes = 0;
	uint32_t dataOffset = 0;
};
#pragma pack()

static constexpr uint32_t kFileSignature = 33639248;

#pragma pack(1)
struct DataHeader
{
	uint32_t signature = 0;
	uint16_t version = 0;
	uint16_t flag = 0;
	uint16_t compression = 0;
	uint16_t time = 0;
	uint16_t date = 0;
	uint32_t crcCode = 0;
	uint32_t compressedSize = 0;
	uint32_t uncompressedSize = 0;
	uint16_t nameLength = 0;
	uint16_t extraLength = 0;
};

static constexpr uint32_t kDataSignature = 67324752;

/*---------------------
| --- Constructor --- |
---------------------*/

CE::CaledonZLib::CaledonZLib()
{
	std::fstream fileStream;
	fileStream.open("Assets.zip", std::ios::in | std::ios::binary);

	if (!fileStream.is_open())
	{
		std::cout << "Failed to open Assets.zip." << std::endl;
		return;
	}

	// Read the Zip Header
	ZipHeader zipHeader;

	std::streamoff zipHeaderOffset = -static_cast<std::streamoff>(sizeof(ZipHeader));
	fileStream.seekg(zipHeaderOffset, std::ios::end);
	fileStream.read(reinterpret_cast<char*>(&zipHeader), sizeof(ZipHeader));

	if (zipHeader.signature != kZipSignature)
	{
		std::cout << "Zip Header signature is invalid." << std::endl;
		return;
	}

	// Read the Directory Data
	char* pDirectoryData = new char[zipHeader.dirSize];
	memset(pDirectoryData, 0, zipHeader.dirSize);

	std::streamoff dirOffset = zipHeader.dirOffset;
	fileStream.seekg(dirOffset, std::ios::beg);
	fileStream.read(pDirectoryData, zipHeader.dirSize);

	// Read the File Headers
	FileHeader* pFileHeader = reinterpret_cast<FileHeader*>(pDirectoryData);

	for (int iFile = 0; iFile < zipHeader.numFiles; ++iFile)
	{
		if (pFileHeader->signature != kFileSignature)
		{
			std::cout << "File Signature is invalid." << std::endl;
			return;
		}

		char* pNameData = reinterpret_cast<char*>(pFileHeader + 1);
		std::string fileName(pNameData, pFileHeader->nameLength);

		// Get the Data Header and Unzip the File
		DataHeader dataHeader;
		fileStream.seekg(pFileHeader->dataOffset, std::ios::beg);
		fileStream.read(reinterpret_cast<char*>(&dataHeader), sizeof(dataHeader));

		if (dataHeader.signature != kDataSignature)
		{
			std::cout << "Data Header signature is invalid." << std::endl;
			return;
		}

		fileStream.seekg(dataHeader.nameLength, std::ios::cur);

		// Allocate Memory for the Uncompressed Data
		char* pUncompressedData = new char[dataHeader.uncompressedSize + 1];

		if (dataHeader.compression == Z_DEFLATED)
		{
			char* pCompressedData = new char[dataHeader.compressedSize];
			fileStream.read(pCompressedData, dataHeader.compressedSize);

			z_stream zStream{};
			zStream.next_in = (Bytef*)pCompressedData;
			zStream.avail_in = (uInt)dataHeader.compressedSize;
			zStream.next_out = (Byte*)pUncompressedData;
			zStream.avail_out = (uInt)dataHeader.uncompressedSize;
			zStream.zalloc = (alloc_func)0;
			zStream.zfree = (free_func)0;

			int zStatus = inflateInit2(&zStream, -MAX_WBITS);
			if (zStatus == Z_OK)
			{
				zStatus = inflate(&zStream, Z_FINISH);
				if (zStatus != Z_STREAM_END)
				{
					std::cout << "Failed to decompress data" << std::endl;
					delete[] pCompressedData;
					delete[] pUncompressedData;
					return;
				}
				inflateEnd(&zStream);
			}
			else
			{
				std::cout << "Failed to initialize zlib" << std::endl;
				delete[] pCompressedData;
				delete[] pUncompressedData;
				return;
			}
			delete[] pCompressedData;
		}
		else
		{
			fileStream.read(pUncompressedData, dataHeader.uncompressedSize);
		}

		// Append the null-terminator just in case the data is text
		pUncompressedData[dataHeader.uncompressedSize] = NULL;

		// Store the uncompressed data in the map
		m_uncompressedData[fileName] = std::string(pUncompressedData);

		delete[] pUncompressedData;

		// Continue to the next file header
		pFileHeader = reinterpret_cast<FileHeader*>(pNameData + pFileHeader->nameLength + pFileHeader->extraLength + pFileHeader->commentLength);
	}

	// Clean up and close the Assets.zip file
	delete[] pDirectoryData;
	fileStream.close();
}

/*--------------------------------------------------------------------
| --- GetUncompressedData: Get the Data from the Associated File --- |
--------------------------------------------------------------------*/

std::string CE::CaledonZLib::GetUncompressedData(const std::string& filePath) const
{
	auto it = m_uncompressedData.find(filePath);
	if (it != m_uncompressedData.end())
	{
		return it->second;
	}
	return std::string();
}