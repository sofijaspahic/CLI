#include "InputStream.h"
#include "../Errors/Exception.h"

using namespace std;

ConsoleInputStream::ConsoleInputStream(istream& in) : in(in) {}

istream& ConsoleInputStream::stream() {
	if (in.eof() || in.fail()) {
		in.clear();
	}
	return in;
}

FileInputStream::FileInputStream(const string& filename) : file(filename) {
	if (!file.is_open()) {
		throw FileNotFoundError(filename);
	}
}

istream& FileInputStream::stream() {
	return file;
}

StringInputStream::StringInputStream(const string& content) :ss(content) {}

istream& StringInputStream::stream() {
	return ss;
}
