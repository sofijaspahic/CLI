#include "Prompt.h"
#include "../../Core/Interpreter.h"
#include "../../Errors/Exception.h"

using namespace std;

Prompt::Prompt(Interpreter* interp, shared_ptr<OutputStream> out, shared_ptr<OutputStream> err) 
	: SimpleCommand(out,err),interpreter(interp) {}

void Prompt::setInterpreter(Interpreter* interp) {
	interpreter = interp;
}

void Prompt::execute() {
	if (args.empty()) {
		throw MissingArgumentError("prompt");
	}

	if (args.size() > 1) {
		throw TooManyArgumentsError("prompt");
	}

	if (!interpreter) {
		throw InterpreterError("prompt");
	}

	if (!args[0].quoted) {
		throw ArgumentMustBeQuotedError("prompt");
	}

	interpreter->setPrompt(args[0].value);
}
