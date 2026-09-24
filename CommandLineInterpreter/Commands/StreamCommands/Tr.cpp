#include "Tr.h"
#include "../../Errors/Exception.h"
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

void Tr::execute() {
	ostream& out = outStream->stream();

	string whatStr = "";
	string withStr = "";
	const Arg* filename = nullptr;
	bool whatFound = false;

	for (const Arg& arg : args) {
		// ako pocinje sa - to je sigurno -what
		if (!arg.value.empty() && arg.value[0] == '-') {
			whatStr = arg.value.substr(1);
			if (whatStr.length() >= 2 && whatStr.front() == '"' && whatStr.back() == '"') {
				whatStr = whatStr.substr(1, whatStr.length() - 2);
			}
			whatFound = true;
		}
		// nasli what, sledeci je with ako vec ne postoji
		else if (whatFound && withStr.empty()) {
			withStr = arg.value;
			if (withStr.length() >= 2 && withStr.front() == '"' && withStr.back() == '"') {
				withStr = withStr.substr(1, withStr.length() - 2);
			}
		}
		// nismo nasli -what, a nemamo filename onda je prvi arg fajl
		else if (!whatFound && !filename) {
			filename = &arg;
		}
		else {
			throw TooManyArgumentsError("tr");
		}
	}

	if (!whatFound) {
		throw MissingArgumentError("tr");
	}

	auto processStream = [&](istream& is) {
		string result;
		string line;
		bool firstLine = true;
		while (getline(is, line)) {
			if (!firstLine) {
				result += "\n";
			}
			firstLine = false;
			if (whatStr.empty()) {
				result += line;
				continue;
			}
			string newLine;
			size_t currentPos = 0;
			size_t foundPos = 0;

			while ((foundPos = line.find(whatStr, currentPos)) != string::npos) {
				newLine.append(line, currentPos, foundPos - currentPos);
				if (!withStr.empty()) {
					newLine.append(withStr);
				}
				currentPos = foundPos + whatStr.length();
			}

			newLine.append(line, currentPos, string::npos); 
			result += newLine;
		}
		out << result;
	};

	withResolvedInput(filename, processStream);
}

// arg(fajl) ce biti uvek pre "-what" ako ga ima
bool Tr::hasArgumentInput() const {
	return !args.empty() && !args[0].value.empty() && args[0].value[0] != '-';
}
