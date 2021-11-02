#include "headers/assembly.h"
#include <windows.h>
#include "headers/beacon.h"
#include "headers/userdefs.h"
#include "headers/win32api.h"


// Forward declarations to keep things tidy.
BOOL get_file_list (LPCSTR folderHunt, LPCSTR newb, PDFSTRUCT pdfStruct, formatp* beaconFormatStruct);
void determinePath (const char* options[5], PDFSTRUCT dfsStruct, formatp* beaconFormatStruct);
void displayVanityBanner ();
void dumpEnvironmentSearch (formatp* environmentStructure, PDFSTRUCT dfsStruct);
void go (char* args, int arglength);


// Implementations of forward declarations.
void displayVanityBanner()
{
    char cVanityBanner[] =    "==================================================\n"
                              "=             DLL HIJACK SEARCH BOF              =\n"
                              "=    Author: Justin Lucas  (@the_bit_diddler)    =\n"
                              "==================================================\n";

    BeaconPrintf(CALLBACK_OUTPUT, "%s", cVanityBanner);

    return;
}


BOOL get_file_list(LPCSTR folderHunt, LPCSTR dllToHunt, PDFSTRUCT pdfStruct, formatp* beaconFormatStruct)
{
    if (pdfStruct->bFoundFile == TRUE)
    {
        return pdfStruct->bFoundFile;
    }

    HANDLE hFind;
    WIN32_FIND_DATAA data;

    char cPath[MAX_PATH];

    MSVCRT$_snprintf(cPath, MAX_PATH, "%s\\*", folderHunt);
    hFind = KERNEL32$FindFirstFileA(cPath, &data);

    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            if ((internalstrncmp(".", data.cFileName, 1) != 0) && (internalstrncmp("..", data.cFileName, 1) != 0))
            {
                if (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
                {

                }
                else
                {
                    if (internalstrncmp(dllToHunt, data.cFileName, internalstrlen(dllToHunt)) == 0 && internalstrlen(data.cFileName) == internalstrlen(dllToHunt))
                    {
                        pdfStruct->bFoundFile = TRUE;
                        BeaconFormatPrintf(beaconFormatStruct, "Found:\t\t\t%s\\%s\n", folderHunt, data.cFileName);

                        if (pdfStruct->bCheckCreateFileA)
                        {
                            char fullFilePath[MAX_PATH] = { 0 };
                            MSVCRT$_snprintf(fullFilePath, MAX_PATH, "%s\\%s", folderHunt, data.cFileName);

                            if (internalstrlen(fullFilePath) == (internalstrlen(folderHunt) + internalstrlen("\\") + internalstrlen(data.cFileName)))
                            {
                                HANDLE hResultCheckCreateFile = KERNEL32$CreateFileA(fullFilePath, GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
                                if (hResultCheckCreateFile != INVALID_HANDLE_VALUE)
                                {
                                    BeaconFormatPrintf(beaconFormatStruct, "Mutability:\t\tMutable in current context! (^v^)\n", fullFilePath);
                                    KERNEL32$CloseHandle(hResultCheckCreateFile);
                                }
                                else {
                                    BeaconFormatPrintf(beaconFormatStruct, "Mutability:\t\tImmutable in current context! (-.-)\n", fullFilePath);
                                    BeaconPrintf(CALLBACK_OUTPUT, "ERROR: %ld\n", KERNEL32$GetLastError());
                                }
                            }
                        }
                    }
                    else
                    {
                        pdfStruct->tDepth++;
                    }
                }
            }
        } while (KERNEL32$FindNextFileA(hFind, &data) && pdfStruct->bFoundFile == FALSE);
        KERNEL32$FindClose(hFind);
    }

    return pdfStruct->bFoundFile;
}


void determinePath(const char* options[5], PDFSTRUCT dfsStruct, formatp* beaconFormatStruct)
{
    char* outputString = NULL;
    int sizeOfObject   = 0;

    if (!dfsStruct->bFoundFile)
    {
        BeaconFormatPrintf(beaconFormatStruct, "DLL search order exhausted, DLL file not found!\n");
        outputString = BeaconFormatToString(beaconFormatStruct, &sizeOfObject);
        BeaconOutput(CALLBACK_OUTPUT, outputString, sizeOfObject);
        BeaconFormatFree(beaconFormatStruct);

        return;
    }

    for (int cIndex = 0; cIndex < dfsStruct->cDepth; cIndex++)
    {
        if (dfsStruct->cDepth > 1)
        {
            if (dfsStruct->cDepth != cIndex + 1)
            {
                BeaconFormatPrintf(beaconFormatStruct, "Not found (%d of 5):\t%s\n", cIndex + 1, options[cIndex]);
            }
            else {
                BeaconFormatPrintf(beaconFormatStruct, "Found in: (%d of 5):\t%s\n", cIndex + 1, options[cIndex]);
            }
            
        }
    }

    outputString = BeaconFormatToString(beaconFormatStruct, &sizeOfObject);
    BeaconOutput(CALLBACK_OUTPUT, outputString, sizeOfObject);

    // Free the data structure(s).
    BeaconFormatFree(beaconFormatStruct);

    return;
}


void dumpEnvironmentSearch(formatp* environmentStructure, PDFSTRUCT dfsStruct)
{
    char* outputString = NULL;
    int   sizeOfObject = 0;

    outputString = BeaconFormatToString(environmentStructure, &sizeOfObject);

    if (dfsStruct->bFoundFile == TRUE && dfsStruct->eVar)
    {
        BeaconOutput(CALLBACK_OUTPUT, outputString, sizeOfObject);
    }
    
    BeaconFormatFree(environmentStructure);

    return;
}


void go(char* args, int arglength)
{
    datap parser;
    formatp formatObject;
    formatp environmentObject;

    // Allocate a buffer for the format data structure.
    BeaconFormatAlloc(&formatObject, 64 * 1024);
    BeaconFormatAlloc(&environmentObject, 64 * 1024);

    char* startingDirectory = NULL;
    char* dllToHunt  = NULL;
    char* tokenParse = NULL;

    // Sanity checking that our allocation works as intended, bailing if unsuccessful.
    char* resultData = NULL;
    resultData = (char*)MSVCRT$malloc(4095 * sizeof(char));
    if ( resultData == NULL )
    {
        BeaconPrintf(CALLBACK_OUTPUT, "Failure in malloc, exiting before any further logic commences.\n");
        return;
    }

    // Initialize the members of the data structure.
    DFSStruct dfsStruct;
    dfsStruct.bFoundFile = FALSE;
    dfsStruct.bCheckCreateFileA  = TRUE;
    dfsStruct.bResultCreateFileA = FALSE;
    dfsStruct.cDepth = 1;
    dfsStruct.tDepth = 1;
    dfsStruct.eVar   = 0;

    // Parse user-supplied arguments.
    BeaconDataParse(&parser, args, arglength);
    startingDirectory = BeaconDataExtract(&parser, NULL);
    dllToHunt = BeaconDataExtract(&parser, NULL);

    // Create the variable only after the arguments have been successfuly parsed.
    const char* cSearchArray[5] = { startingDirectory, "C:\\Windows\\System32", "C:\\Windows\\System", "C:\\Windows", "ENVIRONMENT_VARS" };
    
    // Vanity banner
    displayVanityBanner();

    BOOL getOperatingDirectory = get_file_list(startingDirectory, dllToHunt, &dfsStruct, &formatObject);
    if (!getOperatingDirectory)
    {
        dfsStruct.cDepth++;
    }
    else
    {
        determinePath(cSearchArray, &dfsStruct, &formatObject);
        if (resultData != NULL)
        {
            MSVCRT$free(resultData);
        }
        dumpEnvironmentSearch(&environmentObject, &dfsStruct);

        return;
    }

    BOOL getSystem32Info = get_file_list("C:\\Windows\\System32", dllToHunt, &dfsStruct, &formatObject);
    if (!getSystem32Info)
    {
        dfsStruct.cDepth++;
    }
    else
    {
        determinePath(cSearchArray, &dfsStruct, &formatObject);
        if (resultData != NULL)
        {
            MSVCRT$free(resultData);
        }
        dumpEnvironmentSearch(&environmentObject, &dfsStruct);

        return;
    }

    BOOL getWindowsInfo = get_file_list("C:\\Windows", dllToHunt, &dfsStruct, &formatObject);
    if (!getWindowsInfo)
    {
        dfsStruct.cDepth++;
    }
    else
    {
        determinePath(cSearchArray, &dfsStruct, &formatObject);
        if (resultData != NULL)
        {
            MSVCRT$free(resultData);
        }
        dumpEnvironmentSearch(&environmentObject, &dfsStruct);
        
        return;
    }
    
    KERNEL32$GetEnvironmentVariableA("path", resultData, 4000);
    if (resultData != NULL)
    {
        tokenParse = MSVCRT$strtok(resultData, ";");
        while (tokenParse != NULL && dfsStruct.bFoundFile == FALSE)
        {
            get_file_list(tokenParse, dllToHunt, &dfsStruct, &formatObject);
            dfsStruct.eVar++;
            BeaconFormatPrintf(&environmentObject, "Environment Var %03d:\t%s\n", dfsStruct.eVar, tokenParse);
            tokenParse = MSVCRT$strtok(NULL, ";");
        }
    }
    dfsStruct.cDepth++;

    determinePath(cSearchArray, &dfsStruct, &formatObject);
    dumpEnvironmentSearch(&environmentObject, &dfsStruct);

    if (resultData != NULL)
    {
        MSVCRT$free(resultData);
    }
    
    return;
}