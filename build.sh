#!/bin/bash

# Compile the C++ source code into an executable
g++ main.cpp -o OneDriveStandaloneUpdater.exe -lversion -L. -static-libgcc -static-libstdc++ -Wl,--no-undefined -Wl,--allow-multiple-definition

if [ $? -eq 0 ]; then
    echo "OneDriveStandaloneUpdater.exe built successfully."
else
    echo "Error building OneDriveStandaloneUpdater.exe."
fi
