#include "ListWebsites.hpp"
#include <future>
#include "_18.hpp"
parentalControl *pc = new parentalControl();
std::vector<std::string> listIP;
bool unblock = false;
#ifdef _WIN32
BOOL SetPrivilege(
    HANDLE hToken,
    LPCTSTR lpszPrivilege,
    BOOL bEnablePrivilege)
{
    TOKEN_PRIVILEGES tp;
    LUID luid;

    if (!LookupPrivilegeValue(
            NULL,
            lpszPrivilege,
            &luid))
    {
        printf("LookupPrivilegeValue error: %u\n", GetLastError());
        return FALSE;
    }

    tp.PrivilegeCount = 1;
    tp.Privileges[0].Luid = luid;
    if (bEnablePrivilege)
        tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
    else
        tp.Privileges[0].Attributes = 0;

    if (!AdjustTokenPrivileges(
            hToken,
            FALSE,
            &tp,
            sizeof(TOKEN_PRIVILEGES),
            (PTOKEN_PRIVILEGES)NULL,
            (PDWORD)NULL))
    {
        printf("AdjustTokenPrivileges error: %u\n", GetLastError());
        return FALSE;
    }

    if (GetLastError() == ERROR_NOT_ALL_ASSIGNED)

    {
        printf("The token does not have the specified privilege. \n");
        return FALSE;
    }

    return TRUE;
}
#endif

bool CheckRoot()
{
#ifdef _WIN32

    BOOL isAdmin;
    SID_IDENTIFIER_AUTHORITY NtAuthority = SECURITY_NT_AUTHORITY;
    PSID AdministratorsGroup;

    isAdmin = AllocateAndInitializeSid(
        &NtAuthority,
        2,
        SECURITY_BUILTIN_DOMAIN_RID,
        DOMAIN_ALIAS_RID_ADMINS,
        0, 0, 0, 0, 0, 0,
        &AdministratorsGroup);

    if (isAdmin)
    {
        if (!CheckTokenMembershipNULL, AdministratorsGroup, &isAdmin)
        {
            isAdmin = FALSE;
        }
        FreeSid(AdministratorsGroup);
    }

    if (isAdmin)
        return true;
    else
        return false;

#elif __linux__

    if (geteuid() != 0)
    {
        return false;
    }
    else
    {
        return true;
    }
#endif
}

#ifdef _WIN32

void hideApp()
{
    ShowWindow(GetConsoleWindow(), SW_HIDE);
    ShowWindow(GetDesktopWindow(), SW_HIDE);
    SetWindowPos(GetConsoleWindow(), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
}
#elif __linux__

void hideApp()
{
    Display *display = XOpenDisplay(NULL);
    Window window = DefaultRootWindow(display);

    XEvent event;
    event.type = ClientMessage;
    event.xclient.window = window;
    event.xclient.message_type = XInternAtom(display, "WM_PROTOCOLS", False);
    event.xclient.format = 32;
    event.xclient.data.l[0] = 1;
    event.xclient.data.l[1] = XInternAtom(display, "WM_DELETE_WINDOW", False);
    XSendEvent(display, window, False, NoEventMask, &event);

    XCloseDisplay(display);
}
#endif
std::string Help =
    "COMMAND LIST\n"
    "--help \t displays commands\n"
    "--Info \t information about the program's purpose & How this work\n"
    "--root? \t explains why root / admin is needed\n"
    "--unblock \t Unlock all outgoing and incoming calls ";

std::string purpose =
    "Program Purpose\n"
    "The program is intended for parental control.\n"
    "The software blocks and prevents connecting to domains\n"
    "that are on the blacklist.\n"
    "The blacklist of websites is located in the file ListWebsites.hpp\n"
    "as a const char* array.\n"
    "Add your websites there, and it will not be possible to access them!\n"
    "Even from a private browser session.\n";
std::string sudo =
    "The program requires running as root or administrator on a Windows system\n"
    "Linux:\n"
    "- Preventing the process from being terminated by a regular user\n"
    "- The program uses system functions and netstat\n"
    "This works similarly on Windows system\n";

bool isNetstatInstalled()
{
    std::array<char, 128> buffer{};
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen("which netstat", "r"), pclose);

    if (!pipe)
    {
        throw std::runtime_error("popen() failed!");
    }

    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr)
    {
        result += buffer.data();
    }

    return !result.empty();
}

int main(int argc, char *argv[])
{

    for (int i = 1; i < argc; i++)
    {
        if (strstr(argv[i], "--HELP"))
        {
            std::cout << Help << std::endl;
        }
        if (strstr(argv[i], "--Info"))
        {
            std::cout << purpose << std::endl;
        }
        if (strstr(argv[i], "--root?"))
        {
            std::cout << sudo << std::endl;
        }
        if (strstr(argv[i], "--unblock"))
        {
            unblock = true;
        }
    }
    if (!CheckRoot())
    {
        std::cout << "You are not root! Please run this soft as admin / root\nUse flag --HELP" << std::endl;
        return 0;
    }

    hideApp();

#ifdef __linux__

    if (!isNetstatInstalled())
        std::cout << "Please install Netstat, and try again\n run as:\nsudo apt update && apt-get update apt install net-tools " << std::endl;
    gtk_init(&argc, &argv);
#elif
    // Protection proces
    HANDLE hToken;
    if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &hToken))
    {
        std::cerr << "OpenProcessToken error: " << GetLastError() << "\n";
        return 1;
    }
    if (!SetPrivilege(hToken, SE_DEBUG_NAME, TRUE))
    {
        std::cerr << "Failed to enable privilege.\n";
        return 1;
    }
#endif

    std::vector<std::string> windows = pc->getOpenWindows();
    int arrayLength = sizeof(ListOfWebsites) / sizeof(ListOfWebsites[0]);
    if (unblock == false)
    {
        while (true)
        {
            for (size_t i = 0; i < arrayLength; i++)
            {
                std::future<bool> result(std::async(&parentalControl::checkConnectByIp, pc, ListOfWebsites[i]));
            }
            std::this_thread::sleep_for(std::chrono::seconds(3));
        }
    }
    else if (unblock == true)
    {
        for (size_t i = 0; i < arrayLength; i++)
        {
            pc->UnblockALL(ListOfWebsites[i]);
            if(pc->isLinux())
            {
                system("sudo systemctl restart systemd-resolved ");
                system("sudo iptables -F INPUT");
                system("sudo iptables -F OUTPUT")


            }
        }
        
    }

    return 0;
}