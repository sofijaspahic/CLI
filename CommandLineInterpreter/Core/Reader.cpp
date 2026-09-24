#include "Reader.h"
#include "../Errors/Exception.h"

using namespace std;

namespace {
	void applyMaxLength(string& line) {
		if (line.size() > MAX_LINE_LENGTH) {
			line.resize(MAX_LINE_LENGTH);
		}
	}
}

ConsoleReader::ConsoleReader(istream& in) : in(in) {}

bool ConsoleReader::readLine(string& line) {
	in.clear();
	if (!getline(in, line)) {
		return false;
	} 
	applyMaxLength(line);
	return true;
}

FileReader::FileReader(const string& filename) : file(filename) {
	if (!file.is_open()) {
		throw FileNotFoundError(filename);
	}
}

bool FileReader::readLine(string& line) {
	if (!getline(file, line)) {
		return false;
	}
	applyMaxLength(line);
	return true;
}
