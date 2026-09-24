#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <exception>
#include <string>
#include <vector>
#include <utility>

class Exception : public std::exception {
public:
	explicit Exception(std::string msg) : message(std::move(msg)) {}
	const char* what() const noexcept override {
		return message.c_str();
	}
		
protected:
	std::string message;

};

class LexicalError : public Exception {
public:
	using Exception::Exception;
};

class UnexpectedCharacterError : public LexicalError {
public:
	UnexpectedCharacterError(const std::string& line, const std::vector<size_t>& badPositions)
		:LexicalError(buildMessage(line, badPositions)) {}

private:
	static std::string buildMessage(const std::string& line, const std::vector<size_t>& badPositions) {
		std::string marker(line.size(), ' ');
		for (size_t pos : badPositions) {
			if (pos < marker.size()) {
				marker[pos] = '^';
			}
		}
		return "Error - unexpected characters :\n" + line + "\n" + marker;
	}
};

class MissingClosingQuoteError : public LexicalError {
public:
	MissingClosingQuoteError(const std::string& line)
		:LexicalError("Error - missing closing quote:\n" + line) {}
};

class UnknownCommandError :public Exception {
public:
	UnknownCommandError(const std::string& cmdName)
		:Exception("Unknown command: " + cmdName) {}
};

class SyntaxError : public Exception {
public:
	using Exception::Exception;
};

class EmptyCommandNameError : public SyntaxError {
public:
	EmptyCommandNameError() :SyntaxError("Expected a command name but found none.") {}
};

class MissingRedirectionFilenameError :public SyntaxError {
public:
	MissingRedirectionFilenameError(char symbol)
		:SyntaxError(std::string("Redirection '") + symbol + "' is missing a filename.") {}
};

class InvalidRedirectionSyntaxError : public SyntaxError {
public:
	InvalidRedirectionSyntaxError(char unexpected)
		:SyntaxError(std::string("Unexpected character '") + unexpected + "' in redirection.") {
	}
};

class ArgumentMustBeQuotedError : public SyntaxError {
public:
	ArgumentMustBeQuotedError(const std::string& cmdName)
		:SyntaxError("Command '" + cmdName + "' requires its argument to be quoted.") {
	}
};

class SemanticError : public Exception {
public:
	using Exception::Exception;
};

class DuplicateInputRedirectionError : public SemanticError {
public:
	DuplicateInputRedirectionError()
		:SemanticError("Input redirection specified more than once.") {
	}
};

class DuplicateOutputRedirectionError : public SemanticError {
public:
	DuplicateOutputRedirectionError()
		:SemanticError("Output redirection specified more than once.") {
	}
};

class ArgumentInputConflictError : public SemanticError {
public:
	ArgumentInputConflictError(const std::string& conflictingSource)
		:SemanticError("Command already has an argument defining its input, cannot also " + conflictingSource + ".") {
	}
};

class PipeInputNotSupportedError : public SemanticError {
public:
	PipeInputNotSupportedError()
		:SemanticError("This command cannot receive piped input.") {
	}
};

class RedirectionInputNotSupportedError : public SemanticError {
public:
	RedirectionInputNotSupportedError()
		:SemanticError("This command does not support input redirection.") {
	}
};

class RedirectionOutputNotSupportedError : public SemanticError {
public:
	RedirectionOutputNotSupportedError()
		:SemanticError("This command does not support output redirection.") {
	}
};

class PipeOutputNotSupportedError : public SemanticError {
public:
	PipeOutputNotSupportedError()
		:SemanticError("This command does not produce output that can be piped.") {
	}
};

class MissingArgumentError :public SemanticError {
public:
	MissingArgumentError(const std::string& cmdName)
		:SemanticError("Command '" + cmdName + "' requires arguments.") {
	}
};

class TooManyArgumentsError :public SemanticError {
public:
	TooManyArgumentsError(const std::string& cmdName)
		:SemanticError("Command '" + cmdName + "' received too many arguments.") {
	}
};

class InvalidOptionError :public SemanticError {
public:
	InvalidOptionError(const std::string& cmdName, const std::string& option)
		:SemanticError("Unrecognized option '" + option + "' for command '" + cmdName + "'.") {
	}
};

class FileAlreadyExistsError : public SemanticError {
public:
	FileAlreadyExistsError(const std::string& filename)
		:SemanticError("File '" + filename + "' already exists.") {
	}
};

class InterpreterError : public SemanticError {
public:
	InterpreterError(const std::string& cmdName)
		:SemanticError("Interpreter for command '" + cmdName + "' is not valid.") {
	}
};

class FileSystemError : public Exception {
public:
	using Exception::Exception;
};

class FileNotFoundError : public FileSystemError {
public:
	FileNotFoundError(const std::string& filename)
		:FileSystemError("File '" + filename + "' not found.") {}
};

class FileAccessError : public FileSystemError {
public:
	FileAccessError(const std::string& filename)
		:FileSystemError("Cannot access file '" + filename + "'.") {}
};

#endif // !EXCEPTION_H
