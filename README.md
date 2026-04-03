# OneDriveStandaloneUpdater (Simulation)

This repository contains a simulated `OneDriveStandaloneUpdater.exe` designed to test the side-loading behavior of a custom `version.dll` (like the one developed in the `GraphAPIDLL` project).

## Purpose

The legitimate `OneDriveStandaloneUpdater.exe` is known to load `version.dll` from its local directory before falling back to the system-wide `version.dll`. This project provides a simple C++ executable that mimics this behavior, specifically calling the `GetFileVersionInfoSizeW` function from the loaded `version.dll`.

This allows for easy testing and demonstration of DLL side-loading and persistence techniques.

## How it Works

1.  The `main.cpp` program starts.
2.  It attempts to load `version.dll` from the same directory using `LoadLibrary(L"version.dll")`.
3.  If `version.dll` is loaded successfully, it retrieves the address of the `GetFileVersionInfoSizeW` function.
4.  It then calls `GetFileVersionInfoSizeW`. If your custom `version.dll` (from `GraphAPIDLL`) has been configured to block indefinitely in this function, this executable will appear to hang, keeping the process alive.
5.  If `GetFileVersionInfoSizeW` returns, the executable prints a message and exits.

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
    g++ main.cpp -o OneDriveStandaloneUpdater.exe -lversion -L. -static-libgcc -static-libstdc++ -Wl,--no-undefined -Wl,--allow-multiple-definition
    ```
    This will create `OneDriveStandaloneUpdater.exe` in the current directory.

## Testing with `GraphAPIDLL`

To test your custom `version.dll`:

1.  Build your `version.dll` from the `GraphAPIDLL` project (ensure it has the persistence logic enabled).
2.  Place the compiled `version.dll` in the same directory as `OneDriveStandaloneUpdater.exe`.
3.  Run `OneDriveStandaloneUpdater.exe`.

If your `version.dll` is correctly configured for persistence, the `OneDriveStandaloneUpdater.exe` process will remain running, and your DLL's background tasks will continue to execute.
