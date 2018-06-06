// wsls-copy.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Windows.h>
#include <Shlwapi.h>
#include <string>
#pragma comment(lib, "shlwapi.lib")

int main(int /*argc*/, char** /*argv*/)
{
	auto lpszCmdLine = GetCommandLine();

	int n = 0;
	LPWSTR* argv = CommandLineToArgvW(lpszCmdLine, &n);

	std::wstring sourcePath, destPath;


	for (int i = 1; i < n; ++i) {
		std::wstring uncPath = L"\\\\?\\";

		// relative path support
		if(!PathIsRelativeW(argv[i]))
			uncPath.append(argv[i]);
		else {
			if (PathFileExists(argv[i])) {
				int nreq = GetFullPathNameW(argv[i], 0, nullptr, nullptr);
				uncPath.resize(nreq + uncPath.size());
				GetFullPathNameW(argv[i], nreq, &uncPath.front() + 4, nullptr);
			}
		}

		if (sourcePath.empty()) {
			if (!PathIsDirectory(uncPath.c_str()) &&
				PathFileExists(uncPath.c_str()))
			{
				sourcePath = std::move(uncPath);
				break;
			}
		}
	}

	int iRet = ERROR_INVALID_PARAMETER;
	if (!sourcePath.empty())
	{
		if (DeleteFileW(sourcePath.c_str()))
			iRet = 0;
		else
			iRet = GetLastError();
	}

	LocalFree(argv);

	return iRet;
}

