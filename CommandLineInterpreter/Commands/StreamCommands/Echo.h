#ifndef ECHO_H
#define ECHO_H

#include "../Command.h"

class Echo : public StreamCommand {
public:
	using StreamCommand::StreamCommand;
	void execute() override;
};

#endif // !ECHO_H

