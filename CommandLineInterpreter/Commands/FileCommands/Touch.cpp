#include "Touch.h"
#include "../../Errors/Exception.h"

using namespace std;

void Touch::execute() {

	if (args.empty()) {
		throw MissingArgumentError("touch");
	}

	if (args.size() > 1) {
		throw TooManyArgumentsError("touch");
	}

	const string& filename = args[0].value;

	if (fileExists(filename)) {
		throw FileAlreadyExistsError(filename);
	}
	
	ofstream newFile(filename);
	if (!newFile.is_open()) {
		throw FileAccessError(filename);
	}

	newFile.close();
}
