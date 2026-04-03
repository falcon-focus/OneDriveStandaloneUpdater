# OneDriveStandaloneUpdater (Simulation)

This repository contains a simulated `OneDriveStandaloneUpdater.exe` designed to test the side-loading behavior of a custom `libcrypto-3-x64.dll` (like the one developed in the `GraphAPIDLL` project).

## Purpose

The legitimate `OneDriveStandaloneUpdater.exe` is known to load `libcrypto-3-x64.dll` from its local directory. This project provides a simple C++ executable that mimics this behavior, specifically calling the `OPENSSL_init_crypto` function from the loaded `libcrypto-3-x64.dll`.

This allows for easy testing and demonstration of DLL side-loading and persistence techniques.

## How it Works

1.  The `main.cpp` program starts.
2.  It attempts to load `libcrypto-3-x64.dll` from the same directory using `LoadLibraryW(L".\\libcrypto-3-x64.dll")`.
3.  If `libcrypto-3-x64.dll` is loaded successfully, it retrieves the address of the `OPENSSL_init_crypto` function.
4.  It then calls `OPENSSL_init_crypto`. If your custom `libcrypto-3-x64.dll` (from `GraphAPIDLL`) has been configured to block indefinitely in this function, this executable will appear to hang, keeping the process alive.
5.  If `OPENSSL_init_crypto` returns, the executable prints a message and exits.

## Building the Executable

### Prerequisites

-   A C++ compiler (e.g., MinGW-w64 on Windows, or `g++` on Linux/WSL for cross-compilation).

### Steps

1.  Navigate to the repository directory:
    ```bash
    cd OneDriveStandaloneUpdater_repo
    ```
2.  Compile the `main.cpp` using the provided `build.sh` script (or manually):
    ```bash
    ./build.sh
    ```
    Alternatively, for manual compilation (e.g., on Windows with MinGW-w64):
    ```bash
    g++ main.cpp -o OneDriveStandaloneUpdater.exe -static-libgcc -static-libstdc++ -Wl,--no-undefined -Wl,--allow-multiple-definition
    ```
    This will create `OneDriveStandaloneUpdater.exe` in the current directory.

## Testing with `GraphAPIDLL`

To test your custom `libcrypto-3-x64.dll`:

1.  Build your `libcrypto-3-x64.dll` from the `GraphAPIDLL` project (ensure it has the persistence logic enabled).
2.  Place the compiled `libcrypto-3-x64.dll` in the same directory as `OneDriveStandaloneUpdater.exe`.
3.  Run `OneDriveStandaloneUpdater.exe`.

If your `libcrypto-3-x64.dll` is correctly configured for persistence, the `OneDriveStandaloneUpdater.exe` process will remain running, and your DLL's background tasks will continue to execute.
