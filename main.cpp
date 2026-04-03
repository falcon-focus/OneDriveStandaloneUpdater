#include <windows.h>
#include <stdio.h>

// Function pointer type for OPENSSL_init_crypto
typedef int (*OPENSSL_init_crypto_ptr)(unsigned long long, const unsigned char*);

int main()
{
    HMODULE hLib = NULL;
    OPENSSL_init_crypto_ptr pOpenSSL_init_crypto = NULL;

    printf("OneDriveStandaloneUpdater.exe: Starting up...\n");
    printf("OneDriveStandaloneUpdater.exe: Attempting to load libcrypto-3-x64.dll...\n");

    // We use a relative path to force the loader to look in the local directory first,
    // bypassing the KnownDLLs check if applicable (though libcrypto is not a KnownDLL).
    hLib = LoadLibraryW(L".\\libcrypto-3-x64.dll");
    
    if (hLib != NULL)
    {
        printf("OneDriveStandaloneUpdater.exe: libcrypto-3-x64.dll loaded successfully.\n");

        // Get the address of OPENSSL_init_crypto
        pOpenSSL_init_crypto = (OPENSSL_init_crypto_ptr)GetProcAddress(hLib, "OPENSSL_init_crypto");
        
        if (pOpenSSL_init_crypto != NULL)
        {
            printf("OneDriveStandaloneUpdater.exe: Calling OPENSSL_init_crypto (this should block indefinitely if DLL is persistent)...\n");
            // Call the function. In our custom DLL, this will block the main thread.
            // This simulates the behavior of a legitimate application initializing OpenSSL.
            pOpenSSL_init_crypto(0, NULL);
            printf("OneDriveStandaloneUpdater.exe: OPENSSL_init_crypto returned (this should not happen if DLL is persistent).\n");
        }
        else
        {
            printf("OneDriveStandaloneUpdater.exe: Could not find OPENSSL_init_crypto in libcrypto-3-x64.dll. Error: %lu\n", GetLastError());
        }
        FreeLibrary(hLib);
    }
    else
    {
        printf("OneDriveStandaloneUpdater.exe: Failed to load libcrypto-3-x64.dll. Error: %lu\n", GetLastError());
    }

    printf("OneDriveStandaloneUpdater.exe: Exiting (this line should not be reached if DLL is persistent).\n");
    return 0;
}
