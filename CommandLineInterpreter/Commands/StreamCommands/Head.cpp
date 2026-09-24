#include "Head.h"
#include "../../Errors/Exception.h"
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <cctype>

using namespace std;

void Head::execute() {
	ostream& out = outStream->stream();

	size_t lineCount = 0;
	bool optionFound = false;
	const Arg* filename = nullptr;

	for (const Arg& arg : args) {
		if (arg.value.rfind("-n", 0) == 0) { 
			string countStr = arg.value.substr(2);

			if (countStr.empty() || countStr.length() > 5 ||
				!all_of(countStr.begin(),countStr.end(),[](unsigned char c) {return isdigit(c);})) {
				throw InvalidOptionError("head",arg.value);
			}

			try {
				lineCount = stoul(countStr);  
			} catch (const exception&) {
				throw InvalidOptionError("head", arg.value);
			}
			optionFound = true;
		} else if (!arg.value.empty() && arg.value[0] == '-') {
			throw InvalidOptionError("head", arg.value);
		} else if (!filename) {
			filename = &arg;
		} else {
			throw TooManyArgumentsError("head");
		}
	}

	if (!optionFound) {
		throw MissingArgumentError("head");
	}

	string result;
	auto processStream = [&](istream& is) {
		string line;
		size_t currentLine = 0;
		bool firstLine = true;
		while (currentLine < lineCount && getline(is, line)) {
			if (!firstLine)  result += "\n";
			firstLine = false;
			result += line;
			currentLine++;
		}
	};

	withResolvedInput(filename, processStream);

	if (!filename) {
		string discarded;
		while (getline(inStream->stream(), discarded)) {}
	}

	out << result;
}

bool Head::hasArgumentInput() const {
	for (const Arg& arg : args) {
		if (arg.value.rfind("-n",0)!=0) {
			return true;
		}
	}
	return false;
}