# Parental Control

<p align="center">
  <img src="https://image.freepik.com/free-vector/parental-control-concept-illustration_114360-644.jpg" alt="Parental Control" width="300" height="300"/>
</p>



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
