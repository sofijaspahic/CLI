#ifndef HEAD_H
#define HEAD_H

#include "../Command.h"

class Head : public StreamCommand {
public:
	using StreamCommand::StreamCommand;
	void execute() override;
	bool hasArgumentInput() const override;
};

#endif // !HEAD_H

