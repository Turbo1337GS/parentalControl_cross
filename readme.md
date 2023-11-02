# Parental Control

<p align="center">
  <img src="https://image.freepik.com/free-vector/parental-control-concept-illustration_114360-644.jpg" alt="Parental Control" width="300" height="300"/>
</p>

## Table of Contents
- [Introduction](#introduction)
- [Requirements](#requirements)
- [Installation](#installation)
- [Usage](#usage)
- [License](#license)

## Introduction
Parental Control is a software application designed for blocking and preventing access to specific websites. It is intended to be used as a parental control tool to protect children from inappropriate online content. The program maintains a blacklist of websites that are not allowed to be accessed. Once the program is running, it continuously checks for connections to these blocked websites and blocks them if detected.

## Requirements
- C++ compiler
- Boost C++ library
- OpenSSL library (Linux only)

## Installation
1. Clone the repository: `git clone https://github.com/Turbo1337GS/parentalControl`
2. Navigate to the project directory: `cd parentalControl`
3. Build the project:
   - Linux: `make`
   - Windows: Open the project in Visual Studio and build it
4. The compiled executable will be available in the `bin` directory.

## Usage
1. Run the application as an administrator or root user.
2. The application will run in the background and continuously monitor for connections to blocked websites.
3. Any detected connections to blocked websites will be automatically blocked by the application.
4. To configure the list of blocked websites, modify the `ListWebsites.hpp` file and add the URLs to be blocked.
5. To stop the application, use the appropriate system tools or terminate the process.

## License
This software is released under the [MIT License](LICENSE.txt). Feel free to modify and distribute it according to your needs.









# Parental Control

Parental Control is a command-line tool designed to block specified websites and IP addresses, preventing access to these sites. It supports both Linux and Windows platforms.

## Usage

To run the program, use the following command:

```
./GigaSoft [--HELP] [--Info] [--root?] [--unblock]
```

The available command-line options are:

- `--HELP`: Displays help information about the program.
- `--Info`: Displays information about the purpose of the program.
- `--root?`: Checks if the user has root/admin privileges.
- `--unblock`: Unblocks all blocked websites and IP addresses.

## Installation

To compile and install the program, follow the steps below:

1. Clone the repository:

   ```
   git clone https://github.com/Turbo1337GS/parentalControl.git
   ```

2. Navigate to the project directory:

   ```
   cd parentalControl
   ```

3. Compile the source code using the provided `makefile`:

   ```
   make all
   ```

4. Run the program:

   ```
   ./GigaSoft
   ```

## Block and Unblock

The program allows you to block specific websites and IP addresses. By default, the program continuously monitors for connections to these sites/IPs and blocks them. To unblock the websites and IP addresses, use the `--unblock` option.

## Supported Platforms

- Linux (requires Netstat)
- Windows

## Dependencies

- Xlib (Linux only)
- libcurl
- Boost (boost.system)
- GTK+ 3 (Linux only)

## License

 * mit
