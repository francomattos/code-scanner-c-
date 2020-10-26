#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
//windows library needed to open file via gui.
#include <windows.h>
//To use GetOpenFileName() function you must add libcomdlg32.a, to do so go to Project>Build Options>Linker Settings>Add>Find file under MINIGW Lib. :)

#define OPEN_FILE_BUTTON 1
#define COUNTERS 13
int i;

//function for behavior of window
LRESULT CALLBACK WindowProcedure(HWND hWnd, UINT msg,WPARAM wp,LPARAM lp){
    switch(msg) {
        case WM_COMMAND:
            {
            switch(wp) {
                case OPEN_FILE_BUTTON:
                    OpenProgramFile(hWnd);
                    break;
            }
        }
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        //this calls functions to create button and box text
        case WM_CREATE:
            AddControls(hWnd);
            break;
        default:
            return DefWindowProcW(hWnd,msg,wp,lp);
    }
}
//function to add controls to gui
void AddControls(HWND hWnd) {
    //text                                               //x   y  w   h
    CreateWindowW(L"static",L"Press to search for a file\nFile types: .c, .txt, or .js", WS_VISIBLE | WS_CHILD, 100,30,250,50,
                  hWnd,NULL,NULL,NULL);
    //button                                                  //x   y  w  h
    CreateWindowW(L"Button", L"Search", WS_VISIBLE | WS_CHILD, 135,65,75,30,
                  hWnd,(HMENU)OPEN_FILE_BUTTON,NULL,NULL);
}
//initialized from button press
void OpenProgramFile(HWND hWnd) {
    char filePathGet[300];

    OPENFILENAME filePathObject;
    //defines structure of file browser window
    ZeroMemory(&filePathObject, sizeof(OPENFILENAME));
    filePathObject.lStructSize = sizeof(OPENFILENAME);
    filePathObject.lpstrFile = filePathGet;
    filePathObject.lpstrFile[0] = '\0';
    filePathObject.nMaxFile = 300;
    filePathObject.lpstrFilter = "C/Javascript/Text Files\0""*.c;*.js;*.txt\0";
    filePathObject.nFilterIndex = 1;
    //open file browser window
    GetOpenFileName(&filePathObject);
    //initiates function to open and process file
    ProcessFile(filePathObject.lpstrFile);
}
//gets path from OpenProgramFile function
void ProcessFile(char* filePathPass) {
    char lineRead[800] = { 0 }, newFile[50] = "Success, results saved as: ";
    /*
    initiates counter, 0=lines, 1=blank lines, 2=comments, 3=variable declarations
    4=if's, 5=else's, 6=for, 7=switch, 8=semicolons, 9=structs, 10=arrays, blocks=11
    */
    int _size, fileCounter[COUNTERS] = { 0 }, i;

    char* result = NULL;

    FILE* File_Path;
    FILE* Stats_File;
    //open file path received from OpenProgramFile
    File_Path = fopen(filePathPass,"r") ;
    if (File_Path == NULL) {
        MessageBox(NULL,"File Did Not Open",NULL,MB_OK);
        return 1 ;
    }

    //finds size of file
    fseek(File_Path,0,SEEK_END);
    _size = ftell(File_Path);
    rewind(File_Path);
    //starts counting occurrences
    result = fgets(lineRead,790, File_Path);
    while (result != NULL) {
            ++fileCounter[0];
        if (strstr(lineRead,"//") != NULL || strstr(lineRead,"/*") != NULL)
            ++fileCounter[2];
        if (strstr(lineRead,"int") != NULL || strstr(lineRead,"long") != NULL
            || strstr(lineRead,"char") != NULL || strstr(lineRead,"float") != NULL
            || strstr(lineRead,"double") != NULL || strstr(lineRead,"#define") != NULL)
            ++fileCounter[3];
        if (strstr(lineRead,"if ") != NULL || strstr(lineRead,"if(") != NULL)
            ++fileCounter[4];
        if (strstr(lineRead,"else") != NULL)
            ++fileCounter[5];
        if (strstr(lineRead,"for") != NULL || strstr(lineRead,"for(") != NULL)
            ++fileCounter[6];
        if (strstr(lineRead,"switch") != NULL)
            ++fileCounter[7];
        if (strstr(lineRead,";") != NULL)
            ++fileCounter[8];
        if (strstr(lineRead,"struct") != NULL)
            ++fileCounter[9];
        if (strstr(lineRead,"[") != NULL && strstr(lineRead,"]") != NULL)
            ++fileCounter[10];
        if (strstr(lineRead,"{") != NULL)
            ++fileCounter[11];
       if (strstr(lineRead,"include") != NULL)
            ++fileCounter[12];
       if (CheckForEmptyLine(lineRead))
            ++fileCounter[1];

    result = fgets(lineRead,790, File_Path);
    }

    //begin exporting
    strcat(filePathPass, ".txt");
    remove(filePathPass);
    Stats_File = fopen(filePathPass,"w") ;
    fprintf(Stats_File,
            "Total file size is %d bytes\n\n"
            "There are %d total lines of code \n"
            "There are %d blank lines, representing %.2lf%% of the lines\n\n"
            "Statistical data:\n"
            "There are %d comments, found in %.2lf%% of the lines\n"
            "There are %d variable declarations, found in %.2lf%% of the lines\n"
            "There are %d if statements, found in %.2lf%% of the lines\n"
            "There are %d else statements, found in %.2lf%% of the lines\n"
            "There are %d for loops, found in %.2lf%% of the lines\n"
            "There are %d switch statements, found in %.2lf%% of the lines\n"
            "There are %d semicolons, found in %.2lf%% of the lines\n"
            "There are %d structs, found in %.2lf%% of the lines\n"
            "There are %d instances of array, found in %.2lf%% of the lines\n"
            "There are %d blocks, found in %.2lf%% of the lines\n"
            "There are %d included files, found in %.2lf%% of the lines\n",
            _size, fileCounter[0],
            fileCounter[1],fileCounter[1]*100/(double)fileCounter[0],fileCounter[2],fileCounter[2]*100/(double)fileCounter[0],
            fileCounter[3],fileCounter[3]*100/(double)fileCounter[0],fileCounter[4],fileCounter[4]*100/(double)fileCounter[0],
            fileCounter[5],fileCounter[5]*100/(double)fileCounter[0],fileCounter[6],fileCounter[6]*100/(double)fileCounter[0],
            fileCounter[7],fileCounter[7]*100/(double)fileCounter[0],fileCounter[8],fileCounter[8]*100/(double)fileCounter[0],
            fileCounter[9],fileCounter[9]*100/(double)fileCounter[0],fileCounter[10],fileCounter[10]*100/(double)fileCounter[0],
            fileCounter[11],fileCounter[11]*100/(double)fileCounter[0],fileCounter[12],fileCounter[12]*100/(double)fileCounter[0]);
                /*  initiates counter, 0=lines, 1=blank lines, 2=comments, 3=variable declarations
                4=if's, 5=else's, 6=for, 7=switch, 8=semicolons, 9=structs, 10=arrays, blocks=11, include=12  */

    //removes file path of new file to print only file name at message box
    for (i=strlen(filePathPass)-1;i>=0;--i)  {
        if (filePathPass[i] == '\\') {
            strncat(newFile, filePathPass + ++i, strlen(filePathPass) - i);
            break;
        }
    }
    MessageBox(NULL,newFile,"Success",MB_OK);
        //closes file
    fclose (File_Path) ;
    fclose (Stats_File) ;

    return 0;
}



//checks for white spaces
int CheckForEmptyLine(char *stringIn) {
    char newString[strlen(stringIn)];
    i=0;

    while (*stringIn != '\0') {
        if (!isspace(*stringIn)) {
           newString[i] = *stringIn;
           i++;
        }
        *stringIn++;
    }
    newString[i] = '\0';
    //returns 1 if empty line and 0 if not
    return (newString[0] == '\0') ? 1:0;
}

//this is the new main with WINAPI for windows gui capability
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR args, int ncmdshow) {
    WNDCLASSW wc = {0};

    //defines window box
    wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
    wc.hCursor = LoadCursor(NULL,IDC_ARROW);
    wc.hInstance = hInst;
    wc.lpszClassName = L"myWindowClass";
    wc.lpfnWndProc = WindowProcedure;

    //if not registered correctly end
    if(!RegisterClassW(&wc))
        return 1;
    //creates gui window box                                                            x   y   w   h
    CreateWindowW(L"myWindowClass", L"Code Scanner", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 600,400,350,200,
                        NULL,NULL,NULL,NULL);
    MSG msg = {0};

    while (GetMessage(&msg,NULL,NULL,NULL)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}


