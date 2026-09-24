#ifndef DATE_H
#define DATE_H

#include "../Command.h"

class Date :public SimpleCommand {
public:
	using SimpleCommand::SimpleCommand;
	void execute() override;
};


#endif // !DATE_H
