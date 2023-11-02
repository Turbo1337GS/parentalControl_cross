#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <map>
#include <set>
#include <stack>
#include <queue>
#include <string>
#include <regex>

#ifdef _WIN32
#include <windows.h>
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")

#elif __linux__
#include <X11/Xlib.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <gtk/gtk.h>

#include <X11/Xutil.h>
#include <unistd.h>
#endif

#include <array>
#include <cstdlib>
#include <memory>
#include <stdexcept>

#include <curl/curl.h>
// boost.org
#include <boost/algorithm/string.hpp>
#include <boost/asio.hpp>
#include <boost/filesystem.hpp>
#include <thread>
#include <chrono>
class parentalControl
{

private:
public:
    std::vector<std::string> getOpenWindows();
    bool validIP(const std::string &text);
    std::vector<std::string> getIPsFromURL(const std::string &url);

    std::string execsys(const char *cmd);
    bool checkConnectByIp(std::string url);
    void msg(const char *Title, const char *body);
    void blockIP(const std::string &ip);
    void unblockIP(const std::string &ip);
    void blockURL(std::string url);
void unblockURL(std::string url);
void UnblockALL(std::string url);
bool isLinux();


};