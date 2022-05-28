
// Unit tests for UTF8 / CodePage 437
#define CATCH_CONFIG_MAIN
#include "catch2\catch_all.hpp"
#include "TextConverter.h"
#include <string>

#include <fstream>
uint32_t checksum(const std::string& file);

void CheckIfFilesAreEqual(std::ifstream&, std::ifstream&);

const std::string utf8FilePath{ "Resources\\in\\utf-8.txt" };
const std::string codePageFilePath{ "Resources\\in\\code page 437.txt" };
const std::string testFilePath{ "Resources\\in\\test.txt" };
const std::string outputPath{ "Resources\\out\\" };

TEST_CASE("Test converting UTF8 to codePage") {
    TextConverter converter{};
    const std::string outputFileName{ outputPath + "outCodePage.txt" };
    converter.ConvertUTF8FileToCodePage(utf8FilePath, outputFileName);
    REQUIRE(checksum(codePageFilePath) == checksum(outputFileName));
}

TEST_CASE("Test converting codePage to UTF8") {
    TextConverter converter{};
    const std::string outputFileName{ outputPath + "outUTF8.txt" };
    converter.ConvertCodePageToUTF8(codePageFilePath, outputFileName);
    REQUIRE(checksum(utf8FilePath) == checksum(outputFileName));
}

TEST_CASE("Converting from codepage to utf8 and back") {
    TextConverter converter{};
    const std::string outUtf8FileName{ outputPath + "outUTF8.txt" };
    const std::string outCodePageFileName{ outputPath + "outCodePage.txt" };
    converter.ConvertCodePageToUTF8(codePageFilePath, outUtf8FileName);
    converter.ConvertUTF8FileToCodePage(outUtf8FileName, outCodePageFileName);

    // Compare original with double converted file
    REQUIRE(checksum(outCodePageFileName) == checksum(codePageFilePath));

}

TEST_CASE("Converting from utf8 to codepage and back") {
    TextConverter converter{};
    const std::string outUtf8FileName{ outputPath + "outUTF8.txt" };
    const std::string outCodePageFileName{ outputPath + "outCodePage.txt" };
    converter.ConvertUTF8FileToCodePage(utf8FilePath, outCodePageFileName);
    converter.ConvertCodePageToUTF8(outCodePageFileName, outUtf8FileName);
    REQUIRE(checksum(outUtf8FileName) == checksum(utf8FilePath));
}

/*
* Test.txt is encoded in Codepage 437
* Tests if converting test file to utf8 in back results in the same original file
*/
TEST_CASE("Converting Test.txt to UTF8 and back to codepage") {
    TextConverter converter{};
    const std::string outUtf8FileName{ outputPath + "TestFileInUTF8.txt" };
    const std::string outCodePageFileName{ outputPath + "TestTileBackToCodePage.txt" };
    converter.ConvertCodePageToUTF8(testFilePath, outUtf8FileName);
    converter.ConvertUTF8FileToCodePage(outUtf8FileName, outCodePageFileName);
    REQUIRE(checksum(testFilePath) == checksum(outCodePageFileName));

}

// https://codereview.stackexchange.com/questions/104948/32-bit-checksum-of-a-file
uint32_t checksum(const std::string& path)
{
    std::ifstream file{ path, std::ifstream::binary };
    uint32_t sum = 0;

    uint32_t word = 0;
    while (file.read(reinterpret_cast<char*>(&word), sizeof(word))) {
        sum += word;
        word = 0;
    }

    sum += word; // add the last word, could be 0
                 // if the file size is divisible by 4

    return sum;
}

