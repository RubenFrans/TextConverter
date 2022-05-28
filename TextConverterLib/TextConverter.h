#pragma once
#include <string>
#include <vector>
class TextConverter final
{
public:
	TextConverter();
	~TextConverter() = default;

	void ConvertUTF8FileToCodePage(const std::string& path, const std::string& outputFilename = "outCodePage.txt");
	void ConvertCodePageToUTF8(const std::string& path, const std::string& outputFileName = "outUTF8.txt");

private:
	void InitializeLookupTable();
	uint8_t CodePointToByte(uint32_t codePoint);
	uint32_t ByteToCodePoint(uint8_t codePageBits);
	uint32_t ReadNextCodePoint(std::ifstream& input);
	uint32_t ConstructUTF8FromCodePointAndWriteToFile(std::ofstream& output, uint32_t codePoint);
	
	std::vector<std::pair<uint32_t, uint8_t>> m_LookupTable;


	const uint8_t m_OneBytesMask{ uint8_t(0b0000'0000) };
	const uint8_t m_TwoBytesMask{ uint8_t(0b1100'0000) };
	const uint8_t m_ThreeBytesMask{ uint8_t(0b1110'0000) };
	const uint8_t m_FourBytesMask{ uint8_t(0b1111'0000) };
};

