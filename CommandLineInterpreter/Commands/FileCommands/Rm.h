#ifndef RM_H
#define RM_H

#include "../Command.h"

class Rm : public FileCommand {
public:
	using FileCommand::FileCommand;
	void execute() override;
	bool producesOutput() const override { return false; }
};

#endif // !RM_H

