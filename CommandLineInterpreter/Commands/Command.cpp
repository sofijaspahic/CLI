#include "Command.h"
#include "../Errors/Exception.h"
#include <sstream>
#include <algorithm>
#include <cctype>

using namespace std;

Command::Command(shared_ptr<OutputStream> out, shared_ptr<OutputStream> err)
	: outStream(out), errStream(err) {}

void Command::setArgs(const vector<Arg>& newArgs) {
    args = newArgs;
}

void Command::setOutStream(shared_ptr<OutputStream> out) {
    outStream = out;
}

void Command::setErrStream(shared_ptr<OutputStream> err) {
    errStream = err;
}

std::ostream* Command::consoleOutput() const {
    return outStream->isConsole() ? &outStream->stream() : nullptr;
}

StreamCommand::StreamCommand(shared_ptr<InputStream> in, shared_ptr<OutputStream> out, shared_ptr<OutputStream> err) 
    : Command(out,err),inStream(in){}

void StreamCommand::setInStream(shared_ptr<InputStream> in) {
    inStream = in;
}

bool StreamCommand::hasArgumentInput() const {
    return !args.empty();
}

void StreamCommand::withResolvedInput(const Arg* filename, const function<void(istream&)>& consume) {
    if (filename) {
        // "Hello"
        if (filename->quoted) {
            istringstream iss(filename->value);
            consume(iss);
        }
        // fajl.txt
        else {
            ifstream file(filename->value);
            if (!file.is_open()) throw FileNotFoundError(filename->value);
            consume(file);
            file.close();
        }
    }
    // Hello
    else {
        consume(inStream->stream());
    }
}

BatchCommand::BatchCommand(Interpreter* interp, shared_ptr<OutputStream> out, shared_ptr<OutputStream> err) 
    : Command(out, err), interpreter(interp) {}


void BatchCommand::setInterpreter(Interpreter* interp) {
    interpreter = interp;
}

bool FileCommand::fileExists(const std::string& filename) const {
    std::ifstream file(filename.c_str());
    return file.is_open();
}


size_t Command::parseCountOption(const string& arg, const string& prefix, const string& cmdName) {
	string countStr = arg.substr(prefix.size());
	if (countStr.empty() || countStr.length() > 9 || !all_of(countStr.begin(), countStr.end(), [](unsigned char c) { return isdigit(c); })) {
		throw InvalidOptionError(cmdName, arg);
	}
	try {
		return static_cast<size_t>(stoul(countStr));
	}
	catch (const exception&) {
		throw InvalidOptionError(cmdName, arg);
	}
}

const Arg& Command::requireSingleArg(const string& cmdName) const {
	if (args.empty()) {
		throw MissingArgumentError(cmdName);
	}
	if (args.size() > 1) {
		throw TooManyArgumentsError(cmdName);
	}
	return args[0];
}

tm SimpleCommand::localNow() {
	time_t now = time(nullptr);
	tm local{};
	localtime_s(&local, &now);
	return local;
}

string StreamCommand::readAllInput(const Arg* filename) {
	string content;
	const bool fromConsole = (!filename && inStream->isConsole());

	withResolvedInput(filename, [&](istream& is) {
		char ch;
		while (is.get(ch)) {
			if (fromConsole && ch == '\x1A') break;
			content.push_back(ch);
		}
		});

	if (fromConsole && !content.empty() && content.back() == '\n') {
		content.pop_back();
	}
	return content;
}

vector<string> StreamCommand::readInputLines(const Arg* filename) {
	vector<string> lines;
	istringstream iss(readAllInput(filename));
	string line;
	while (getline(iss, line)) {
		lines.push_back(line);
	}
	return lines;
}

string StreamCommand::joinLines(const vector<string>& lines) {
	string result;
	for (size_t i = 0; i < lines.size(); ++i) {
		if (i > 0) result += "\n";
		result += lines[i];
	}
	return result;
}

void FileCommand::requireExistingFile(const string& filename) const {
	if (!fileExists(filename)) {
		throw FileNotFoundError(filename);
	}
}
