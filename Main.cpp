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
int amount = 0;
int firstopen = 0;
void RunNew();

int WinMain(HINSTANCE hinstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int cmdShow)
{
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
				Sleep(250);
			}
		}

		if (dwProcID != 0)
		{
			isrunning = IsProcessRunning(dwProcID);

			if (!isrunning)
			{
				if (amount < 1)
				{
					amount = 1;
				}
				else if (amount >= 1)
				{
					amount = amount * 2;
				}
				for (int run = 0; run < amount; run++)
				{
					RunNew();
				}

				Sleep(500);
				dwProcID = 0;
			}
		}
	}
	MessageBoxA(0, "[BG] Self-Replicating Notepad v1", "Credits", MB_ICONINFORMATION);
}

BOOL IsProcessRunning(DWORD pid)
{
	HANDLE process = OpenProcess(SYNCHRONIZE, FALSE, pid);
	DWORD ret = WaitForSingleObject(process, 0);
	CloseHandle(process);
	return ret == WAIT_TIMEOUT;
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