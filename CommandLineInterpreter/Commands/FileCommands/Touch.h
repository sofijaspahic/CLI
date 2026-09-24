#ifndef TOUCH_H
#define TOUCH_H

#include "../Command.h"

class Touch : public FileCommand {
public:
	using FileCommand::FileCommand;
	void execute() override;
	bool producesOutput() const override { return false; }
};


#endif // !TOUCH_H

