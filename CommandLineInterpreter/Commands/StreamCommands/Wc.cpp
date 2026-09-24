#include "Wc.h"
#include "../../Errors/Exception.h"
#include <fstream>
#include <sstream>

using namespace std;

void Wc::execute() {
	ostream& out = outStream->stream();

	bool countWords = false;
	bool countChars = false;
	const Arg* filename = nullptr;

	for (const Arg& arg : args) {
		if (arg.value == "-w") {
			countWords = true;
		} else if (arg.value == "-c") {
			countChars = true;
		} else if (!arg.value.empty() && arg.value[0] == '-') {
			throw InvalidOptionError("wc", arg.value);
		} else if (!filename) {
			filename = &arg;
		} else {
			throw TooManyArgumentsError("wc");
		}
	}

	if (!countWords && !countChars) {
		throw MissingArgumentError("wc");
	}

	size_t words = 0;
	size_t chars = 0;
	bool inWord = false;

	auto processStream = [&](istream& is) {
		char ch;
		while (is.get(ch)) {
			chars++;
			if (isspace(static_cast<unsigned char>(ch))) {
				inWord = false;
			}
			else if (!inWord) {
				inWord = true;
				words++;
			}
		}
	};

	withResolvedInput(filename, processStream);

	if (countWords && countChars) {
		out << words << " " << chars;
	} else if (countWords) {
		out << words;
	} else if (countChars) {
		out << chars;
	}

}

bool Wc::hasArgumentInput() const {
	for (const Arg& arg : args) {
		if (arg.value != "-w" && arg.value != "-c") {
			return true;
		}
	}
	return false;
}
