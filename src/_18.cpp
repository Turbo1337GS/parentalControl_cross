#include "_18.hpp"
#ifdef _WIN32
#define IS_WINDOWS true
#else
#define IS_WINDOWS false
#endif

std::vector<std::string> parentalControl::getOpenWindows()
{
    std::vector<std::string> windowNames;

    // Check platform for compatibility
#ifdef _WIN32
    // Use EnumWindows on Windows
    EnumWindows([](HWND hwnd, LPARAM lParam) -> BOOL
                {
                    // Get window text and check visibility
                    char buffer[128];
                    GetWindowText(hwnd, buffer, sizeof(buffer));
                    if(IsWindowVisible(hwnd)) {
                        ((std::vector<std::string>*)lParam)->push_back(buffer);
                    }
                    return TRUE; },
                (LPARAM)&windowNames);
#elif __linux__
    // Use XQueryTree on Linux
    Display *display = XOpenDisplay(NULL);
    Window root = DefaultRootWindow(display);
    Window parent;
    Window *children;
    unsigned int nchildren;
    XQueryTree(display, root, &root, &parent, &children, &nchildren);

    char *name = NULL;
    for (unsigned int i = 0; i < nchildren; i++)
    {
        XFetchName(display, children[i], &name);
        if (name)
        {
            windowNames.push_back(name);
            XFree(name);
        }
    }

    XFree(children);
    XCloseDisplay(display);
#endif

    return windowNames;
}

bool parentalControl::isLinux()
{
#ifdef __linux__
    return true;
#else
    return false;
#endif
}

bool parentalControl::validIP(const std::string &text)
{
    int dotCount = 0;
    for (char character : text)
    {
        if (character == '.')
            dotCount++;
    }
    return dotCount == 4;
}
std::string parentalControl::execsys(const char *cmd)
{
    std::array<char, 1280> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
    if (!pipe)
    {
        throw std::runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr)
    {
        result += buffer.data();
    }
    return result;
}

std::vector<std::string> parentalControl::getIPsFromURL(const std::string &url)
{
    std::regex ipPattern(R"(.*Address:\s((?:[0-9]{1,3}\.){3}[0-9]{1,3}))");
    std::smatch match;

    std::string command = IS_WINDOWS ? "nslookup " : "nslookup ";
    std::string bashCommand = command + url;
    std::string outputStr = execsys(bashCommand.c_str());

    std::vector<std::string> ip_addresses;
    std::string::const_iterator searchStart(outputStr.cbegin());
    while (std::regex_search(searchStart, outputStr.cend(), match, ipPattern))
    {
        // We don't want to add a local host that starts with 127.0
        if (match[1].str().find("127") == 0)
        {
            // Skip this IP address
            searchStart = match.suffix().first;
            continue;
        }

        ip_addresses.push_back(match[1].str());
        searchStart = match.suffix().first;
    }

    return ip_addresses;
}

void parentalControl::msg(const char *Title, const char *body)
{
#ifdef _WIN32
    MessageBox(nullptr, body, Title, MB_OK);
#elif __linux__

    GtkWidget *messagebox = gtk_message_dialog_new(NULL,
                                                   GTK_DIALOG_DESTROY_WITH_PARENT,
                                                   GTK_MESSAGE_INFO,
                                                   GTK_BUTTONS_OK,
                                                   "%s", body);
    gtk_window_set_title(GTK_WINDOW(messagebox), Title);
    gtk_dialog_run(GTK_DIALOG(messagebox));
    gtk_widget_destroy(messagebox);
#endif
}

#ifdef _WIN32
void parentalControl::blockIP(const std::string &ip)
{
    std::string commandIn = "netsh advfirewall firewall add rule name=\"BLOCK IN " + ip +
                            "\" dir=in action=block remoteip= " + ip;
    if (system(commandIn.c_str()))
    {
        std::cerr << "Failed to block incoming IP: " << ip << '\n';
    }
    else
    {
        std::cout << "Blocked incoming IP: " << ip << '\n';
    }

    std::string commandOut = "netsh advfirewall firewall add rule name=\"BLOCK OUT " + ip +
                             "\" dir=out action=block remoteip= " + ip;
    if (system(commandOut.c_str()))
    {
        std::cerr << "Failed to block outgoing IP: " << ip << '\n';
    }
    else
    {
        std::cout << "Blocked outgoing IP: " << ip << '\n';
    }
}

void parentalControl::unblockIP(const std::string &ip)
{
    std::string commandIn = "netsh advfirewall firewall delete rule name=\"BLOCK IN " + ip + "\"";
    if (system(commandIn.c_str()))
    {
        std::cerr << "Failed to unblock incoming IP: " << ip << '\n';
    }
    else
    {
        std::cout << "Unblocked incoming IP: " << ip << '\n';
    }

    std::string commandOut = "netsh advfirewall firewall delete rule name=\"BLOCK OUT " + ip + "\"";
    if (system(commandOut.c_str()))
    {
        std::cerr << "Failed to unblock outgoing IP: " << ip << '\n';
    }
    else
    {
        std::cout << "Unblocked outgoing IP: " << ip << '\n';
    }
}
#else

void parentalControl::blockIP(const std::string &ip)
{
    std::string commandIn = "iptables -A INPUT -s " + ip + " -j DROP";
    if (system(commandIn.c_str()))
    {
        std::cerr << "Failed to block incoming IP: " << ip << '\n';
    }
    else
    {
        std::cout << "Blocked incoming IP: " << ip << '\n';
    }

    std::string commandOut = "iptables -A OUTPUT -s " + ip + " -j DROP";
    if (system(commandOut.c_str()))
    {
        std::cerr << "Failed to block outgoing IP: " << ip << '\n';
    }
    else
    {
        std::cout << "Blocked outgoing IP: " << ip << '\n';
    }
}

void parentalControl::unblockIP(const std::string &ip)
{

    std::string commandIn = "iptables -D INPUT -s " + ip + " -j DROP";
    if (system(commandIn.c_str()))
    {
        std::cerr << "Failed to unblock incoming IP: " << ip << '\n';
    }
    else
    {
        std::cout << "Unblocked incoming IP: " << ip << '\n';
    }

    std::string commandOut = "iptables -D OUTPUT -s " + ip + " -j DROP";
    if (system(commandOut.c_str()))
    {
        std::cerr << "Failed to unblock outgoing IP: " << ip << '\n';
    }
    else
    {
        std::cout << "Unblocked outgoing IP: " << ip << '\n';
    }
}
#endif
void parentalControl::unblockURL(std::string url)
{
    std::string line;
    std::string tempUrl = url.find("www.") == 0 ? url.substr(4) : url;

    std::ifstream hosts_in; 
    std::stringstream ss;

#ifdef __linux__
    hosts_in.open("/etc/hosts");
#elif _WIN32
    hosts_in.open("C:\\Windows\\System32\\drivers\\etc\\hosts");
#endif

    if (hosts_in.is_open())
    {
        while (std::getline(hosts_in, line))
        {
            if (line.find(tempUrl) == std::string::npos)
            {
                ss << line << "\n";
            }
        }
        hosts_in.close();

#ifdef __linux__
        std::ofstream hosts_out("/etc/hosts");
#elif _WIN32
        std::ofstream hosts_out("C:\\Windows\\System32\\drivers\\etc\\hosts");
#endif

        if (hosts_out.is_open())
        {
            hosts_out << ss.str();
            hosts_out.close();
            std::cout << "Unblocked: " << url << std::endl;
        }
        else
        {
            std::cerr << "Error: Unable to open hosts file for writing. Make sure you're running this program with administrative privileges." << std::endl;
        }
    }
    else
    {
        std::cerr << "Error: Unable to open hosts file for reading. Make sure you're running this program with administrative privileges." << std::endl;
    }
}
void parentalControl::blockURL(std::string url)
{
    std::string block;
    if (url.find("www.") == 0)
    {
        block = "\n127.0.0.1 " + url.substr(4) + " " + url;
    }
    else
    {
        block = "\n127.0.0.1 " + url + " www." + url;
    }

    std::ofstream hosts;

#ifdef __linux__
    hosts.open("/etc/hosts", std::ios::app);
#elif _WIN32
    hosts.open("C:\\Windows\\System32\\drivers\\etc\\hosts", std::ios::app);
#endif

    if (hosts.is_open())
    {
        hosts << block;
        hosts.close();
        std::cout << "Blocked: " << url << std::endl;
    }
    else
    {
        std::cerr << "Error: Unable to open hosts file. Make sure you're running this program with administrative privileges." << std::endl;
    }
}

void parentalControl::UnblockALL(std::string url)
{
    auto IPs = getIPsFromURL(url);

    for (const auto &IP : IPs)
    {
        unblockIP(IP);
    }
    unblockURL(url);
}
bool parentalControl::checkConnectByIp(std::string url)
{
#ifdef __linux__

    auto IPs = getIPsFromURL(url);
    for (const auto &IP : IPs)
    {
        if (!IP.empty())
        {
            if (isLinux())
            {
                // netstat -tupn | grep
                blockIP(IP);
                blockURL(url);
                std::cout << "Blocked: " << std::string("Detected a connection to ").append(url) << std::endl;
            }
        }
    }
#elif _WIN32

    auto IPs = getIPsFromURL(url);
    std::cout << "Url: " << url << std::endl;
    for (const auto &IP : IPs)
    {
        if (!IP.empty())
        {
            blockIP(IP);
            blockURL(url);
            std::cout << "Blocked: " << std::string("Detected a connection to ").append(url) << std::endl;
        }
    }
#endif

return true;
}
