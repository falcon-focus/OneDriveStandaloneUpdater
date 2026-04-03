#include <windows.h>
#include <stdio.h>

// Function pointer for GetFileVersionInfoSizeW
typedef DWORD (WINAPI *PGETFILEVERSIONINFOSIZEW)(
    LPCWSTR lptstrFilename,
    LPDWORD lpdwHandle
);

int main()
{
    HMODULE hVersionDll = NULL;
    PGETFILEVERSIONINFOSIZEW pGetFileVersionInfoSizeW = NULL;

    printf("OneDriveStandaloneUpdater.exe: Starting up...\n");
    printf("OneDriveStandaloneUpdater.exe: Attempting to load version.dll...\n");

    // Use a relative path to force the loader to look in the current directory first.
    // This bypasses the "KnownDLLs" check for system DLLs like version.dll.
    hVersionDll = LoadLibraryW(L".\\version.dll");

    if (hVersionDll != NULL)
    {
        printf("OneDriveStandaloneUpdater.exe: version.dll loaded successfully.\n");

        // Get the address of GetFileVersionInfoSizeW
        pGetFileVersionInfoSizeW = (PGETFILEVERSIONINFOSIZEW)GetProcAddress(hVersionDll, "GetFileVersionInfoSizeW");

        if (pGetFileVersionInfoSizeW != NULL)
        {
            printf("OneDriveStandaloneUpdater.exe: Calling GetFileVersionInfoSizeW (this should block indefinitely if DLL is persistent)...\n");
            // Call the function. This is where your DLL's infinite loop will trigger.
            // The EXE's main thread will block here.
            pGetFileVersionInfoSizeW(L"C:\\Windows\\System32\\kernel32.dll", NULL);
            printf("OneDriveStandaloneUpdater.exe: GetFileVersionInfoSizeW returned (this should not happen if DLL is persistent).\n");
        }
        else
        {
            printf("OneDriveStandaloneUpdater.exe: Could not find GetFileVersionInfoSizeW in version.dll. Error: %lu\n", GetLastError());
        }
    }
    else
    {
        printf("OneDriveStandaloneUpdater.exe: Failed to load version.dll. Error: %lu\n", GetLastError());
    }

    printf("OneDriveStandaloneUpdater.exe: Exiting (this line should not be reached if DLL is persistent).\n");
    return 0;
}
