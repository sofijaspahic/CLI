#include "Batch.h"
#include "../../Core/Interpreter.h"
#include "../../Core/Reader.h"
#include "../../Errors/Exception.h"

using namespace std;

void Batch::execute() {
	if (args.empty()) {
		throw MissingArgumentError("batch");
	}

	if (args.size() > 1) {
		throw TooManyArgumentsError("batch");
	}

	if (!interpreter) {
		throw InterpreterError("batch");
	}

	const string& filename = args[0].value;
	FileReader fileReader(filename);

	string line;
	while (fileReader.readLine(line)) {
		if (line.empty()) continue;
		interpreter->processLine(line, outStream, errStream);
	}
}
