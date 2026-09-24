#ifndef TR_H
#define TR_H

#include "../Command.h"

class Tr : public StreamCommand {
public:
	using StreamCommand::StreamCommand;
	void execute() override;
	bool hasArgumentInput() const override;
};

#endif // !TR_H

