#include "Date.h"
#include "../../Errors/Exception.h"
#include <ctime>
#include <iomanip>

using namespace std;

void Date::execute() {

	if (!args.empty()) {
		throw TooManyArgumentsError("date");
	}

	tm local = localNow();
	outStream->stream() << put_time(&local, "%d.%m.%Y.");
}
