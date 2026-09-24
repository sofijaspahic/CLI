#ifndef TRUNCATE_H
#define TRUNCATE_H

#include "../Command.h"

class Truncate : public FileCommand {
public:
	using FileCommand::FileCommand;
	void execute() override;
	bool producesOutput() const override { return false; }
};

#endif // !TRUNCATE_H

