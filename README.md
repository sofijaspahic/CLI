# Command Line Interpreter ⌨️

## 📌 Overview
A custom **Command Line Interpreter (CLI)** implemented in C++ as a school project for the **Object-Oriented Programming 1** course at the Faculty of Electrical Engineering, University of Belgrade.

The interpreter mimics (a simplified version of) real operating system command-line interfaces: it reads a command line from its input, executes the requested command, and writes the result to its output — supporting redirection and pipes, just like a real shell.

## ⚙️ How It Works
- The interpreter runs as an interactive loop: it prints a prompt (`$` by default), reads a command line from standard input, executes it, and repeats.
- A command line has the general form:
  ```
  command [-opt] [argument]
  ```
- Commands can also be supplied as a **batch** — a text file containing a sequence of command lines, executed one after another via the `batch` command.
- Invalid command lines (lexical, syntax or semantic errors) are rejected with a descriptive error message and are not executed.

## 🧰 Supported Commands

| Command | Format | Description |
|---|---|---|
| `echo` | `echo [argument]` | Passes characters from input to output unchanged |
| `prompt` | `prompt argument` | Changes the interpreter's command prompt symbol |
| `time` | `time` | Prints the current system time |
| `date` | `date` | Prints the current system date |
| `touch` | `touch filename` | Creates a new empty file in the current directory |
| `truncate` | `truncate filename` | Clears the contents of an existing file |
| `rm` | `rm filename` | Deletes a file from the current directory |
| `wc` | `wc -opt [argument]` | Counts words (`-w`) or characters (`-c`) in the input |
| `tr` | `tr [argument] -what [with]` | Finds occurrences of `what` in the input and replaces them with `with` (or removes them if `with` is omitted) |
| `head` | `head -ncount [argument]` | Outputs only the first `count` lines of the input |
| `batch` | `batch filename` | Executes a sequence of command lines from a script file |

## 🔀 Redirection & Pipes
- **Input redirection** (`<`) — reads a command's input from a file instead of the console.
- **Output redirection** (`>` overwrite, `>>` append) — writes a command's output to a file instead of the console.
- **Pipes** (`|`) — chains multiple commands together, feeding the output of one command as the input of the next (e.g. `time | tr -":" "." | wc -c > time.txt`).

## ⚠️ Error Handling
All lexical, syntax and semantic errors are reported with a descriptive message on the error output stream, and the offending command line is not executed. Runtime errors reported by the operating system (e.g. a missing file) are handled and reported the same way.

## 🛠️ Technologies
- **Language:** C++
- **IDE:** Visual Studio
- **I/O:** Standard console streams, with support for file redirection and inter-command pipes

## 📁 Project Structure

```
CommandLineInterpreter/
├── CommandLineInterpreter.sln
└── CommandLineInterpreter/
    ├── main.cpp
    ├── Core/
    │   ├── Interpreter.cpp / .h      # Main interpreter loop
    │   ├── Reader.cpp / .h           # Reads raw command lines
    │   └── Translator.cpp / .h       # Parses commands, options, arguments
    ├── Commands/
    │   ├── Command.cpp / .h          # Base command class
    │   ├── SimpleCommands/           # Date, Time, Prompt
    │   ├── FileCommands/             # Rm, Touch, Truncate
    │   ├── StreamCommands/           # Echo, Head, Tr, Wc
    │   └── BatchCommands/            # Batch
    ├── Streams/
    │   ├── InputStream.cpp / .h
    │   └── OutputStream.cpp / .h
    ├── Errors/
    │   └── Exception.h
    └── *.txt                         # Example/test scripts and inputs
```

## 👤 Author
**Sofija Spahić** - 2nd Year Student
Faculty of Electrical Engineering, University of Belgrade

## 📚 Reference
Detailed project requirements available in: [Projektni_zadatak_2025_-_v1.0.pdf](./Projektni_zadatak_2025_-_v1.0.pdf)

## 📝 Notes
- Code is organized following an object-oriented, command-pattern design (each command is its own class derived from a common `Command` base).
- Errors are caught and reported to the user in a clear, descriptive way without crashing the interpreter.
- The architecture (separate `Reader`, `Translator`, `Interpreter`, and per-category command folders) is designed to be easily extensible with new commands.
