# DLL Hijack Search Order BOF

## What is this?
- This is a `Cobalt Strike` `BOF` file, meant to use two arguments (path to begin, and a `DLL` filename of interest), that will traverse the `SafeSearch` order of `DLL` resolution
- Optionally, this will also attempt to ascertain a `HANDLE` to the provided file (if found), and alert the operator of its mutability (`WRITE` access)

## What problem are you trying to solve?
- There are tools (mostly in `.NET` or otherwise) that do this job (traditionally `Powershell`-based), but I hadn't seen similar in `C` at the time of writing.  I may just be terrible at `dorking`

## How do I build this?
1. In this case, you have two options:
	1. Use the existing, compiled object file, located in the `dist` directory (AKA proceed to major step two)
    2. Compile from source via the `Makefile`
        1. `cd src`
        2. `make clean`
        3. `make`
2. Load the `Aggressor` file, in the `Script Manager`, located in the `dist` directory

## How do I modify this `BOF` to **not** attempt to get a `HANDLE` on the provided `DLL` filename, if found?
- Within `./src/main.c`, modify `dfsStruct.bCheckCreateFileA` and `dfsStruct.bResultCreateFileA` to `FALSE`.
- Rebuild with the included build instructions

## How do I use this?
- From a given `Beacon`:
    ```sh
    # For accessing the help menu prompt
    hijack_hunter help

    # Example usage
    hijack_hunter C:\Users\User\Desktop superLegit.dll
    ```
##
## Any known downsides?
- We're still using the `Win32` API and `Dynamic Function Resolution`.  This is for you to determine as far as "risk".
- You may attempt to incur a privileged action without sufficient requisite permissions.  I can't keep you from burning your hand.
##
## What does the output look like?
![](https://i.ibb.co/P6TZYKp/image.png)