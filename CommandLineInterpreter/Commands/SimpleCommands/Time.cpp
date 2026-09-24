#include "Time.h"
#include "../../Errors/Exception.h"
#include <ctime>
#include <iomanip>
#include <chrono>

using namespace std;

void Time::execute() {

	if (!args.empty()) {
		throw TooManyArgumentsError("time");
	}

	tm local = localNow();
	outStream->stream() << put_time(&local, "%H:%M:%S");
}


