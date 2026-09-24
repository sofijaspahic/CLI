#ifndef BATCH_H
#define BATCH_H

#include "../Command.h"

class Batch : public BatchCommand {
public:
	using BatchCommand::BatchCommand;
	void execute() override;
	bool needsNewline() const override { return false; }
};

#endif // !BATCH_H

