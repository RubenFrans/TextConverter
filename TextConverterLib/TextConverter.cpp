//#include "pch.h"
#include "TextConverter.h"
#include <iostream>
#include <fstream>
#include <bitset>
#include <algorithm>

TextConverter::TextConverter()
	: m_LookupTable{}
{
	InitializeLookupTable();
}

void TextConverter::ConvertUTF8FileToCodePage(const std::string& path, const std::string& outputFilename)
{
	std::wcout << "Converting UTF8 to CodePage" << std::endl;

	std::ifstream input{path, std::ifstream::binary};
	std::ofstream output{ outputFilename, std::ofstream::binary };
	while (input.good()) {
		
		char32_t codePoint = ReadNextCodePoint(input);

		if (!input.good())
			break;

		// If the read codepoint is in the ascii table just use the ascii character
		if (!(codePoint < 128)) {
			char codePage = CodePointToByte(codePoint);
			//std::cout << codePage;
			output << codePage;
		}
		else {
			//std::cout << char(codePoint);
			output << char(codePoint);
		}
	}

	output.close();
	input.close();

}

uint32_t TextConverter::ReadNextCodePoint(std::ifstream& input) {
	
	uint32_t codePoint{ 0 };
	char readByte;

	// Read the first byte
	input.read(&readByte, sizeof(char));
	uint8_t byteAmount = readByte | m_OneBytesMask;

	// if starts with 0
	if (byteAmount > m_OneBytesMask && byteAmount < m_TwoBytesMask) {
		// Read 1 byte (no additional bytes to read)
		// Codepoint = 7 least significant bits == ascii
		//std::wcout << "zero" << std::endl;

		char ascii = readByte & (0b0111'1111);
		codePoint += ascii;
		return codePoint;
	}

	byteAmount = m_TwoBytesMask | readByte;

	// if starts with 11
	if (byteAmount > m_TwoBytesMask && byteAmount < m_ThreeBytesMask) {
		// Read 2 bytes (1 additional)
		// Codepoint == 5 least significant bits of first byte followed by 6 least significant bits of second byte
		
		// ignore first 3 bits of the first read byte
		char firstPart = readByte & (0b0001'1111);
		codePoint += firstPart;
		codePoint = codePoint << 6;

		char secondByte;
		input.read(&secondByte, sizeof(char));

		char secondPart = secondByte & (0b0011'1111);
		codePoint += secondPart;
		return codePoint;

	}

	byteAmount = readByte | m_ThreeBytesMask;

	if (byteAmount > m_ThreeBytesMask && byteAmount < m_FourBytesMask) {
		// Read 3 bytes (2 additional bytes to read)
		// Get the first part of the code point from the first read byte by negating the byteamount indication
		char firstPart = readByte & (0b0000'1111);
		codePoint += firstPart;
		// shift codepoint 6 bits to the left to make room for second part
		codePoint = codePoint << 6;

		// Read the second byte
		char secondByte;
		input.read(&secondByte, sizeof(char));

		// ignore first two bits of second byte
		char secondPart = secondByte & (0b0011'1111);
		codePoint += secondPart;
		// Shift codePoint to make room for last part
		codePoint = codePoint << 6;

		char thirdByte;
		input.read(&thirdByte, sizeof(char));
		char thirdPart = thirdByte & (0b0011'1111);
		codePoint += thirdPart;
		return codePoint;
	}

	byteAmount = readByte | m_FourBytesMask;

	if (byteAmount > m_FourBytesMask) {

		// Not nessecary for this exercise might implement if I have extra time.

	}

	return codePoint;

}

void TextConverter::ConvertCodePageToUTF8(const std::string& path, const std::string& outputFileName)
{
	std::wcout << "Converting CodePage to UTF8" << std::endl;
	std::ifstream input{ path, std::ifstream::binary };
	std::ofstream output{ outputFileName, std::ofstream::binary };
	char c {};

	uint32_t utf8;

	while (input.good()) {
		input.read(&c, sizeof(char));

		// Nessecary because the previous read could set the good flag to false meaning it has already reached the end of the file
		// Without this the last character will be written twice.
		if (!input.good()) {
			break;
		}

		if (unsigned char(c) < 128) {
			 utf8 = ConstructUTF8FromCodePointAndWriteToFile(output, c);
		}
		else {

			uint32_t codePoint = ByteToCodePoint(c);
			utf8 = ConstructUTF8FromCodePointAndWriteToFile(output, codePoint);
		}
	}

	output.close();
	input.close();
}

uint32_t TextConverter::ConstructUTF8FromCodePointAndWriteToFile(std::ofstream& output, uint32_t codePoint) {

	if (codePoint > 0x0000 && codePoint < 0x007F) {
		uint8_t utf8{ 0 };
		utf8 += uint8_t(codePoint);
		output << utf8;
		return utf8;
	}

	if (codePoint > 0x0080 && codePoint < 0x07FF) {
		// net nessecarry for this exercise, might include if I find the time.

	}

	if (codePoint > 0x0800 && codePoint < 0xFFFF) {
		char32_t utf8{ 0 };

		uint8_t firstPart{ 0b1110'0000 };
		// We only want the first 4 bits of the codepoint for the first part
		char32_t codePoint1 = codePoint & (0b1111'0000'0000'0000);
		// we shift right 2 bytes
		codePoint1 = codePoint1 >> 12;
		// Construct the first byte of utf8
		firstPart += uint8_t(codePoint1);

		utf8 += firstPart;
		// shift utf8 left 1 byte to create room for second part
		utf8 = utf8 << 8;

		uint8_t secondPart{ 0b1000'0000 };
		char32_t codePoint2 = codePoint & (0b0000'1111'1100'0000);
		// shift right 1 byte
		codePoint2 = codePoint2 >> 6;
		// Construct second byte of utf8
		secondPart += uint8_t(codePoint2);

		utf8 += secondPart;
		// Shift left 1 byte to make room for the final byte
		utf8 = utf8 << 8;

		uint8_t thirdPart{ 0b1000'0000 };
		char32_t codePoint3 = codePoint & (0b0000'0000'0011'1111);
		// No need to shift right because its the last 6 bits of the code point
		thirdPart += uint8_t(codePoint3);

		utf8 += thirdPart;

		output.write((char*)&firstPart, sizeof(char));
		output.write((char*)&secondPart, sizeof(char));
		output.write((char*)&thirdPart, sizeof(char));
		
		return utf8;
	}	

	if (codePoint > m_FourBytesMask) {
		// net nessecarry for this exercise, might include if I find the time.

	}

	// Codepoint lies outside of utf8 specification
	return char32_t{ 0 };
}

uint8_t TextConverter::CodePointToByte(uint32_t codePoint)
{
	// Find the correct Code point in the pair and return the corresponding code page 437 code
	auto codePageIt = std::find_if(m_LookupTable.begin(), m_LookupTable.end(), [&](std::pair<uint32_t, uint8_t> entry) {

			if (entry.first == codePoint) {
				return true;
			}
			else {
				return false;
			}

		});

	if (codePageIt != m_LookupTable.end())
		return (*codePageIt).second;
	else
		return ' ';
}

uint32_t TextConverter::ByteToCodePoint(uint8_t codePageBits)
{
	// Find the correct codepage 437 byte and return the corresponding utf8 codepoint
	auto codePageIt = std::find_if(m_LookupTable.begin(), m_LookupTable.end(), [&](std::pair<uint32_t, uint8_t> entry) {

		if (entry.second == codePageBits) {
			return true;
		}
		else {
			return false;
		}

		});

	if (codePageIt != m_LookupTable.end())
		return (*codePageIt).first;
	else
		return L' '; // Could be replaced by an 'Error' character so its clear something went wrong
}

void TextConverter::InitializeLookupTable() {
	m_LookupTable.push_back({uint32_t(0x2554), uint8_t(0xC9)}); // Left Top Corner Piece
	m_LookupTable.push_back({uint32_t(0x2550), uint8_t(0xCD)}); // Horizontal piece
	m_LookupTable.push_back({uint32_t(0x2557), uint8_t(0xBB)}); // Right Top Corner Piece
	m_LookupTable.push_back({uint32_t(0x2566), uint8_t(0xCB)}); // Horizontal Top T Piece
	m_LookupTable.push_back({uint32_t(0x2569), uint8_t(0xCA)}); // Horizontal Bottom T Piece
	m_LookupTable.push_back({uint32_t(0x2560), uint8_t(0xCC)}); // Vertical Left T Piece
	m_LookupTable.push_back({uint32_t(0x2563), uint8_t(0xB9)}); // Vertical Right T Piece
	m_LookupTable.push_back({uint32_t(0x2551), uint8_t(0xBA)}); // Vertical Straight Piece
	m_LookupTable.push_back({uint32_t(0x256C), uint8_t(0xCE)}); // Cross Piece
	m_LookupTable.push_back({uint32_t(0x255A), uint8_t(0xC8)}); // Left Bottom Corner Piece
	m_LookupTable.push_back({uint32_t(0x255D), uint8_t(0xBC)}); // Right Bottom Corner Piece

	
	// Extra entries for test.txt

	m_LookupTable.push_back({ uint32_t(0x2502), uint8_t(0xB3) }); // Horizontal single line
	m_LookupTable.push_back({ uint32_t(0x250C), uint8_t(0xDA) }); // Top left corner single line
	m_LookupTable.push_back({ uint32_t(0x2510), uint8_t(0xBF) }); // Top Right corner single line
	m_LookupTable.push_back({ uint32_t(0x2514), uint8_t(0xC0) }); // Bottom left corner single line
	m_LookupTable.push_back({ uint32_t(0x2518), uint8_t(0xD9) }); // Bottom Right corner single line
	m_LookupTable.push_back({ uint32_t(0x2592), uint8_t(0xB1) }); // Medium shade
	m_LookupTable.push_back({ uint32_t(0x253C), uint8_t(0xC5) }); // Single line cross
	m_LookupTable.push_back({ uint32_t(0x253C), uint8_t(0xC5) }); // Single line horizontal
	m_LookupTable.push_back({ uint32_t(0x2500), uint8_t(0xC4) }); // Single line horizontal
	m_LookupTable.push_back({ uint32_t(0x255E), uint8_t(0xC6) }); // Single Vertical T  with 2 horizontal lines left
	m_LookupTable.push_back({ uint32_t(0x2561), uint8_t(0xB5) }); // Single Vertical T  with 2 horizontal lines right
	m_LookupTable.push_back({ uint32_t(0x2564), uint8_t(0xD1) }); // Horizontal double line with single line down
	m_LookupTable.push_back({ uint32_t(0x251C), uint8_t(0xC3) }); // Single vertical T with 1 horizontal right
	m_LookupTable.push_back({ uint32_t(0x2524), uint8_t(0xB4) }); // Single vertical T with 1 horizontal left
	m_LookupTable.push_back({ uint32_t(0x252C), uint8_t(0xC2) }); // Single Horizontal T with 1 horizontal down
	m_LookupTable.push_back({ uint32_t(0x2534), uint8_t(0xC1) }); // Single Horizontal T with 1 horizontal up
	m_LookupTable.push_back({ uint32_t(0x2553), uint8_t(0xD6) }); // Double vertical line with one single top line right
	m_LookupTable.push_back({ uint32_t(0x2556), uint8_t(0xB7) }); // Double vertical line with one single top line left
	m_LookupTable.push_back({ uint32_t(0x2556), uint8_t(0xB7) }); // Double horizontal T with single top line
	m_LookupTable.push_back({ uint32_t(0x2565), uint8_t(0xD2) }); // Double horizontal T with single top line
	m_LookupTable.push_back({ uint32_t(0x2562), uint8_t(0xB6) }); // Double vertical T with single left
	m_LookupTable.push_back({ uint32_t(0x255F), uint8_t(0xC7) }); // Double vertical T with single right
	m_LookupTable.push_back({ uint32_t(0x256B), uint8_t(0xD7) }); // Cross with double verticle single horizontal
	m_LookupTable.push_back({ uint32_t(0x2559), uint8_t(0xD3) }); // Vertical double line with single bottom line right
	m_LookupTable.push_back({ uint32_t(0x255C), uint8_t(0xBD) }); // Vertical double line with single bottom line left
	m_LookupTable.push_back({ uint32_t(0x2568), uint8_t(0xD0) }); // Horizontal T with double line to top
	m_LookupTable.push_back({ uint32_t(0x2552), uint8_t(0xD5) }); // Horizontal double line with single line left down
	m_LookupTable.push_back({ uint32_t(0x2555), uint8_t(0xB8) }); // Horizontal double line with single line right down
	m_LookupTable.push_back({ uint32_t(0x2558), uint8_t(0xD4) }); // Horizontal double line with single line left up
	m_LookupTable.push_back({ uint32_t(0x255B), uint8_t(0xBE) }); // Horizontal double line with single line right up
	m_LookupTable.push_back({ uint32_t(0x256A), uint8_t(0xD8) }); // Cross with horizontal double line and single vertical line
	m_LookupTable.push_back({ uint32_t(0x2567), uint8_t(0xCF) }); // Upsidedown T with double horzontal and single vertical line

}
