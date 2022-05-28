
#include "TextConverter.h"
#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>


// Possible arguments
// no arguments => show help
// 1 path + flag = conversion from one textfile to the other
// 2 paths + falgs = conversion from one textfile to other on specified location

struct ConverterSettings {

	std::string inputFile{};
	std::string outputFile{};
	bool utf8ToCodePage{ true };
};

bool ParseArguments(int argc, char* argv[], ConverterSettings& args);

void PrintHelp();
void PrintError(const std::string& errorMsg);

int main(int argc, char* argv[]) {

	if (argc < 3) {
		std::cout << "Not enough arguments" << std::endl;
		PrintHelp();
		return 1;
	}

	ConverterSettings settings{};


	bool result = ParseArguments(argc, argv, settings);

	if (!result) {
		return 1;
	}


	TextConverter converter{};

	if (settings.utf8ToCodePage) {

		if (settings.outputFile.empty()) {
			converter.ConvertUTF8FileToCodePage(settings.inputFile);
		}
		else {
			converter.ConvertUTF8FileToCodePage(settings.inputFile, settings.outputFile);
		}
	}
	else {

		if (settings.outputFile.empty()) {
			converter.ConvertCodePageToUTF8(settings.inputFile);
		}
		else {
			converter.ConvertCodePageToUTF8(settings.inputFile, settings.outputFile);
		}
	}

	return 0;
}

// The correct syntax of 3 arguments is >> TextConverterCommandlineTool.exe /u <inputfile string>
bool ParseArguments(int argc, char* argv[], ConverterSettings& settings) {

	std::string arg1{ argv[1] };
	std::string arg2{ argv[2] };

	std::string arg3{};

	if (argc >= 4) {
		arg3 = std::string{ argv[3] };
	}


	if (std::isdigit(*argv[1])) {
		PrintError("First argument cannot be a number");
		PrintHelp();
		return false;
	}

	if (arg1.compare("u") == 0) {
		settings.utf8ToCodePage = true;
	}
	else if (arg1.compare("c") == 0) {
		settings.utf8ToCodePage = false;
	} else {
		PrintError("ERROR: First argument should be either \"u\" or \"c\"");
		PrintHelp();
		return false;
	}

	std::filesystem::path inputFilePath{ arg2 };

	bool result = std::filesystem::exists(inputFilePath);

	if (result) {
		settings.inputFile = arg2;
	}
	else {
		PrintError("inputFile does not exists");
		return false;
	}


	if (arg3.empty())
		return true;

	settings.outputFile = arg3;

	return true;
}

void PrintHelp() {

	std::cout << "TextConverter Command line help" << std::endl;
	std::cout << "===============================" << std::endl;

	std::cout << "Usage: " << std::endl;
	std::cout << "TextConverterCommandLineTool.exe <flag> <inputfile> <outputfile> " << std::endl;
	std::cout << "<flag> options:" << std::endl;
	std::cout << "\tu: convert UTF-8 to Code Page 437" << std::endl;
	std::cout << "\tc: convert Code Page 437 to UTF-8" << std::endl;
}

void PrintError(const std::string& errorMsg) {

	std::cout << "Error: " << errorMsg << std::endl;

}