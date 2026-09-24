#include "Truncate.h"
#include "../../Errors/Exception.h"
#include <fstream>

using namespace std;

void Truncate::execute() {
	if (args.empty()) {
		throw MissingArgumentError("truncate");
	}

	if (args.size() > 1) {
		throw TooManyArgumentsError("truncate");
	}

	const string& filename = args[0].value;

	ifstream checkFile(filename);
	if (!checkFile.is_open()) {
		throw FileNotFoundError(filename);
	}
	
	ofstream file(filename, ios::out | ios::trunc);
	if (!file.is_open()) {
		throw FileAccessError(filename);
	}

	file.close();
}
