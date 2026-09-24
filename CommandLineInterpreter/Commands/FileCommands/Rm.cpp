#include "Rm.h"
#include "../../Errors/Exception.h"
#include <cstdio>
#include <fstream>

using namespace std;

void Rm::execute() {

	if (args.empty()) {
		throw MissingArgumentError("rm");
	}

	if (args.size() > 1) {
		throw TooManyArgumentsError("rm");
	}

	const string& filename = args[0].value;

	ifstream checkFile(filename);
	if (!checkFile.is_open()) {
		throw FileNotFoundError(filename);
	}
	checkFile.close();

	if (remove(filename.c_str()) != 0) {
		throw FileAccessError(filename);
	}

}
