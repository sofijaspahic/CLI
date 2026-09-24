#ifndef TIME_H
#define TIME_H

#include "../Command.h"

class Time:public SimpleCommand {
public:
	using SimpleCommand::SimpleCommand; 
	void execute() override; 
};


#endif // !TIME_H
