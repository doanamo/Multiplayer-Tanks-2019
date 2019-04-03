#include <cassert>
#include <string>
#include <vector>
#include <iostream>
#include <Windows.h>

struct GameProcess
{
    GameProcess() :
        index(0),
        arguments(),
        hWindow(nullptr),
        dwThreadIdArray(0),
        hThreadArray(nullptr)
    {
    }

    int index;
    std::string path;
    std::string arguments;

    HWND hWindow;
    DWORD dwThreadIdArray;
    HANDLE hThreadArray;
};

BOOL CALLBACK EnumThreadWndProc(HWND hWnd, LPARAM lParam)
{
    *(HWND*)lParam = hWnd;
    return false;
}

DWORD WINAPI CreateGameProcess(LPVOID lpParam)
{
    // Retrieve thread data.
    GameProcess* gameProcess = (GameProcess*)lpParam;
    assert(gameProcess != nullptr);

    // Make process DPI aware (needed for window resize).
    SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
    
    // Start child process.
    STARTUPINFO startupInfo;
    memset(&startupInfo, 0, sizeof(startupInfo));
    startupInfo.cb = sizeof(startupInfo);

    PROCESS_INFORMATION processInfo;
    memset(&processInfo, 0, sizeof(processInfo));

    if(!CreateProcess(nullptr, &gameProcess->arguments[0], nullptr, nullptr,
        false, 0, nullptr, nullptr, &startupInfo, &processInfo))
    {
        MessageBox(nullptr, "Failed to create a child process!", "Error", MB_OK);
        return -1;
    }

    // Reposition window.
    while(true)
    {
        WaitForInputIdle(processInfo.hProcess, 5000);
        EnumThreadWindows(processInfo.dwThreadId, EnumThreadWndProc, (LPARAM)&gameProcess->hWindow);

        if(gameProcess->hWindow != nullptr)
        {
            const int windowWidth = 640;
            const int windowHeight = 480;

            RECT rect;
            rect.top = 0;
            rect.left = 0;
            rect.right = windowWidth;
            rect.bottom = windowHeight;

            UINT dpi = GetDpiForWindow(gameProcess->hWindow);

            AdjustWindowRectExForDpi(&rect, GetWindowLong(gameProcess->hWindow, GWL_STYLE),
                FALSE, GetWindowLong(gameProcess->hWindow, GWL_EXSTYLE), dpi);

            const int windowPosX = rect.left + (rect.right + rect.left) * (gameProcess->index / 2);
            const int windowPosY = (windowHeight - rect.top) * (gameProcess->index % 2);

            SetWindowPos(gameProcess->hWindow, HWND_TOP, windowPosX, windowPosY, rect.right - rect.left, rect.bottom - rect.top, 0);

            break;
        }
    }

    return 0;
}

int main(int argc, char* argv[])
{
    // Determine the executable directory.
    if(argc < 1)
    {
        std::cout << "Invalid number of program arguments!";
        return -1;
    }

    std::string launcherPath(argv[0]);
    std::string launcherDirectory = launcherPath.substr(0, launcherPath.find_last_of('\\') + 1);
    std::string gamePath = launcherDirectory + "Game.exe";

    // Display launcher menu.
    std::cout << "Welcome to Tanks launcher!\n\n";
    std::cout << "Please select an option:\n";
    std::cout << "1. Start single instance as host (other players will be able to connect)\n";
    std::cout << "2. Start single instance as client (you will be prompted to enter IP address)\n";
    std::cout << "3. Start two instances in local session (one host and one client)\n";
    std::cout << "4. Start four instances in local session (one host and three clients)\n";
    std::cout << "5. Start six instances in local session (one host and five client)\n";
    std::cout << std::endl;

    // Get user input.
    std::cout << "Please enter your selection: ";

    std::string menuInput;
    std::cin >> menuInput;

    // Parse user input.
    int localPlayerCount = 0;
    bool firstPlayerHosts = true;
    std::string serverAddress = "127.0.0.1";

    if(menuInput == "1")
    {
        localPlayerCount = 1;
        firstPlayerHosts = true;
    }
    else if(menuInput == "2")
    {
        localPlayerCount = 1;
        firstPlayerHosts = false;

        std::cout << "Please enter IP address: ";
        std::cin >> serverAddress;
    }
    else if(menuInput == "3")
    {
        localPlayerCount = 2;
        firstPlayerHosts = true;
    }
    else if(menuInput == "4")
    {
        localPlayerCount = 4;
        firstPlayerHosts = true;
    }
    else if(menuInput == "5")
    {
        localPlayerCount = 6;
        firstPlayerHosts = true;
    }
    else
    {
        std::cout << "Invalid selection! Aborting...";
        std::cin.get();
        return 0;
    }

    // Launch game processes.
    std::vector<GameProcess> gameProcesses;
    gameProcesses.resize(localPlayerCount);

    // Spawn game process for each player..
    for(std::size_t i = 0; i < gameProcesses.size(); ++i)
    {
        // Create thread data.
        gameProcesses[i].index = i;
        gameProcesses[i].path = gamePath;
        gameProcesses[i].arguments += gamePath + " ";
        gameProcesses[i].arguments += "-showConsole 1 ";

        // Make the first player a host.
        if(i == 0 && firstPlayerHosts)
        {
            gameProcesses[i].arguments += "-host ";
        }
        else
        {
            gameProcesses[i].arguments += "-connect " + serverAddress + " ";
        }

        // Spawn new thread.
        gameProcesses[i].hThreadArray = CreateThread(nullptr, 0, CreateGameProcess, &gameProcesses[i], 0, &gameProcesses[i].dwThreadIdArray);
        assert(gameProcesses[i].hThreadArray != nullptr);
    }

    // Wait for threads to finish.
    for(auto & gameProcess : gameProcesses)
    {
        WaitForSingleObject(gameProcess.hThreadArray, INFINITE);
    }

    return 0;
}
