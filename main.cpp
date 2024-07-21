#include <Windows.h>
#include <shellapi.h>
#include <Shlwapi.h> // Include the Shlwapi library
#include <CommCtrl.h>
#include <tchar.h>
#include <wchar.h>
#include <stdio.h>

#pragma comment(lib, "Shlwapi.lib")

// Function prototypes
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void AppendText(HWND hwnd, const wchar_t* text);
bool RunShellScript(wchar_t* output, int outputSize);
bool RunShellScript1(wchar_t* output, int outputSize);

HWND hwndTextbox;       // Handle to the first textbox control
HWND hwndOutputTextbox; // Handle to the second textbox control

//int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
int CALLBACK WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
    // Register the window class
    const wchar_t CLASS_NAME[] = L"Sample Window Class";

    WNDCLASS wc = { 0 };
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    // Create the window
    HWND hwnd = CreateWindowEx(
        0,                              // Optional window styles
        CLASS_NAME,                     // Window class name
        L"Wall-E Defender",                // Window title
        WS_OVERLAPPEDWINDOW,            // Window style

        // Size and position
        CW_USEDEFAULT, CW_USEDEFAULT, 800, 600,  // Set the window size

        NULL,       // Parent window
        NULL,       // Menu
        hInstance,  // Instance handle
        NULL        // Additional application data
    );

    if (hwnd == NULL)
    {
        return 0;
    }

    // Create the "Disable all network activity" button
    HWND hwndDisableButton = CreateWindow(
        L"BUTTON",                              // Button class
        L"Disable all network activity",        // Button text
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,  // Button styles
        10, 10, 200, 30,                        // Position and size
        hwnd, (HMENU)1001, hInstance, NULL      // Assign an ID to the button
    );
    SendMessage(hwndDisableButton, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), MAKELPARAM(TRUE, 0));
    SendMessage(hwndDisableButton, WM_SETTEXT, 0, (LPARAM)L"Disable all network activity");
    
    // Create the "Enable Site" button
    HWND hwndEnableButton = CreateWindow(
        L"BUTTON",                              // Button class
        L"Enable Site",                        // Button text
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,  // Button styles
        10, 50, 200, 30,                        // Position and size
        hwnd, (HMENU)1002, hInstance, NULL      // Assign an ID to the button
    );
    SendMessage(hwndEnableButton, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), MAKELPARAM(TRUE, 0));
    SendMessage(hwndEnableButton, WM_SETTEXT, 0, (LPARAM)L"Enable Site");

    // Create the textbox control
    hwndTextbox = CreateWindowEx(
        0, L"EDIT", L"", WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_READONLY | ES_MULTILINE,
        220, 10, 550, 480, hwnd, NULL, hInstance, NULL
    );
    SendMessage(hwndTextbox, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), MAKELPARAM(TRUE, 0));
    SendMessage(hwndTextbox, EM_SETSEL, 0, -1); // Select all text

    // Create the second textbox control for displaying script output
    hwndOutputTextbox = CreateWindowEx(
        0, L"EDIT", L"Buffer", WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_READONLY | ES_MULTILINE,
        10, 100, 200, 450, hwnd, NULL, hInstance, NULL
    );

    SendMessage(hwndOutputTextbox, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), MAKELPARAM(TRUE, 0));
    SendMessage(hwndOutputTextbox, EM_SETSEL, 0, -1); // Select all text

   
    // Show the window
    ShowWindow(hwnd, nCmdShow);

    // Run the message loop
    MSG msg = { 0 };
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    case WM_COMMAND:
        if (LOWORD(wParam) == 1001) // Button ID for "Disable all network activity"
        {
            // Clear the existing text in the output textbox
            SetWindowText(hwndOutputTextbox, L"");

            // Call the function to run the shell script and capture output
            wchar_t scriptOutput[4512]; // Adjust the size as needed
            scriptOutput[0] = L'\0'; // Initialize the buffer
            bool success = RunShellScript(scriptOutput, sizeof(scriptOutput) / sizeof(wchar_t));

            // Append the script output to the output textbox
            if (success)
            {
                AppendText(hwndOutputTextbox, L"All browser operations blocked.\r\n");
            }
            else
            {
                //AppendText(hwndOutputTextbox, scriptOutput);
                AppendText(hwndOutputTextbox, L"Operation Failed.Refer to 'T.LOG' file\r\n");
            }
        }
        else if (LOWORD(wParam) == 1002) // Button ID for "Enable Site"
        {
            // Clear the existing text in the output textbox
            SetWindowText(hwndOutputTextbox, L"");

            // Call the function to run the shell script and capture output
            wchar_t scriptOutput[4512]; // Adjust the size as needed
            scriptOutput[0] = L'\0'; // Initialize the buffer
            bool success = RunShellScript1(scriptOutput, sizeof(scriptOutput) / sizeof(wchar_t));

            // Append the script output to the output textbox
            if (success)
            {
                AppendText(hwndOutputTextbox, L"Operation Success. Enjoy Browsing\r\n");
            }
            else
            {
                //AppendText(hwndOutputTextbox, scriptOutput);
                AppendText(hwndOutputTextbox, L"Operation Failed.Refer to 'T.LOG' file\r\n");
            }
        }
        return 0;

    case WM_SIZE:
        // Resize the textbox when the window is resized
        MoveWindow(hwndTextbox, 220, 10, LOWORD(lParam) - 230, HIWORD(lParam) - 20, TRUE);
        MoveWindow(hwndOutputTextbox, 10, 100, 200, HIWORD(lParam) - 150, TRUE);

        return 0;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void AppendText(HWND hwnd, const wchar_t* text)
{
    int len = GetWindowTextLength(hwnd);
    SendMessage(hwnd, EM_SETSEL, len, len);
    SendMessage(hwnd, EM_REPLACESEL, 0, (LPARAM)text);
}

// Modify the RunShellScript function to capture output
bool RunShellScript(wchar_t* output, int outputSize)
{
    //FILE* file;
    //wchar_t buffer[4512];
    
     // Get the path of the current executable
    TCHAR exePath[MAX_PATH];
    GetModuleFileName(NULL, exePath, MAX_PATH);

    // Extract the directory path from the executable path
    TCHAR dirPath[MAX_PATH];
    _tcscpy_s(dirPath, exePath);
    PathRemoveFileSpec(dirPath); // Remove the filename from the path

    SHELLEXECUTEINFO shexinfo = { 0 };
    shexinfo.cbSize = sizeof(SHELLEXECUTEINFO);
    shexinfo.fMask = SEE_MASK_NOCLOSEPROCESS;
    shexinfo.hwnd = NULL;
    shexinfo.lpVerb = L"runas"; // Run as administrator
    shexinfo.lpFile = L"Block_Http.bat"; // Replace with the path to your batch file
    shexinfo.lpParameters = NULL;
    shexinfo.lpDirectory = dirPath;
    shexinfo.nShow = SW_SHOWNORMAL;

    if (ShellExecuteEx(&shexinfo)) {
        WaitForSingleObject(shexinfo.hProcess, INFINITE);
        CloseHandle(shexinfo.hProcess);
        return true;
    }
    else {
        return false;
        // Handle error
        // You may want to display an error message here
    }

    return 0;

    /*
    // Open the file for reading
    if (fopen_s(&file, "t.log", "r") != 0) {
        perror("Error opening file");
        // Check the result of running the script
        if (result == 0)
        {
            return false;
        }
    }

    // Read the first 4000 characters from the file into the buffer
    // Read the first 4000 wide characters from the file
    if (fwscanf_s(file, L"%4000ls", buffer, (unsigned)outputSize) == EOF) {

        perror("Error reading file");
        fclose(file);
        // Check the result of running the script
        if (result == 0)
        {
            return false;
        }
    }

    // Copy the 'buffer' to 'output'
    if (wcscpy_s(output, outputSize, buffer) != 0) {
        perror("Error copying strings");
        // Close the file
        fclose(file);
        // Check the result of running the script
        if (result == 0)
        {
            return false;
        }
    }

    // Null-terminate 'output'
    output[outputSize - 1] = L'\0';

    // Close the file
    fclose(file);

    return true; // Script executed successfully
    */
}


// Modify the RunShellScript function to capture output
bool RunShellScript1(wchar_t* output, int outputSize)
{
    //FILE* file;
    //wchar_t buffer[4512];

     // Get the path of the current executable
    TCHAR exePath[MAX_PATH];
    GetModuleFileName(NULL, exePath, MAX_PATH);

    // Extract the directory path from the executable path
    TCHAR dirPath[MAX_PATH];
    _tcscpy_s(dirPath, exePath);
    PathRemoveFileSpec(dirPath); // Remove the filename from the path

    SHELLEXECUTEINFO shexinfo = { 0 };
    shexinfo.cbSize = sizeof(SHELLEXECUTEINFO);
    shexinfo.fMask = SEE_MASK_NOCLOSEPROCESS;
    shexinfo.hwnd = NULL;
    shexinfo.lpVerb = L"runas"; // Run as administrator
    shexinfo.lpFile = L"Enable_Http.bat"; // Replace with the path to your batch file
    shexinfo.lpParameters = NULL;
    shexinfo.lpDirectory = dirPath;
    shexinfo.nShow = SW_SHOWNORMAL;

    if (ShellExecuteEx(&shexinfo)) {
        WaitForSingleObject(shexinfo.hProcess, INFINITE);
        CloseHandle(shexinfo.hProcess);
        return true;
    }
    else {
        return false;
        // Handle error
        // You may want to display an error message here
    }

    return 0;
}