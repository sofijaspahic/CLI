#ifndef PROMPT_H
#define PROMPT_H

#include "../Command.h"

class Prompt :public SimpleCommand {
public:
	Prompt(Interpreter* interp=nullptr,
		std::shared_ptr<OutputStream> out = std::make_shared<ConsoleOutputStream>(std::cout),
		std::shared_ptr<OutputStream> err = std::make_shared<ConsoleOutputStream>(std::cerr));

	void setInterpreter(Interpreter* interp);
	void execute() override;
	bool producesOutput() const override { return false; }


private:
	Interpreter* interpreter;

	
};

#endif // !PROMPT_H
