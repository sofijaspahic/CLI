#ifndef WC_H
#define WC_H

#include "../Command.h"

class Wc : public StreamCommand {
public:
	using StreamCommand::StreamCommand;
	void execute() override;
	bool hasArgumentInput() const override;
};



#endif // !WC_H
