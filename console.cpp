#include <Windows.h>

#include <fstream>
#include <random>
#include <string>
#include <iostream>

#include "console.hpp"


static std::string GenerateString(size_t length = 16) {
	static const char charset[] =
		"0123456789"
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
		"abcdefghijklmnopqrstuvwxyz";

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dist(0, sizeof(charset) - 2);

	std::string result;
	result.reserve(length);

	for (size_t i = 0; i < length; ++i) {
		result += charset[dist(gen)];
	}

	return result;
}

void Console::Alloc( ) {
#ifndef DISABLE_LOGGING_CONSOLE
	if (!AllocConsole()) {
		return;
	}

	std::string randomTitle = GenerateString();
	SetConsoleTitleA(randomTitle.c_str());

	FILE* fDummy;
	freopen_s(&fDummy, "CONOUT$", "w", stdout);
	freopen_s(&fDummy, "CONOUT$", "w", stderr);
	freopen_s(&fDummy, "CONIN$", "r", stdin);

	std::cout.clear();
	std::clog.clear();
	std::cerr.clear();
	std::cin.clear();

	std::ios::sync_with_stdio();
	SetConsoleOutputCP(CP_UTF8);
#endif
}

void Console::Free( ) {
#ifndef DISABLE_LOGGING_CONSOLE
	fclose(stdout);
	fclose(stderr);
	fclose(stdin);
	FreeConsole();

#endif
}
