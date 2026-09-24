#include "Echo.h"
#include "../../Errors/Exception.h"
#include <fstream>
#include <iterator>

using namespace std;

void Echo::execute() {
	ostream& out = outStream->stream();

	if (!args.empty()) {
		if (args.size() == 1) {
			if (args[0].quoted) { 
				out << args[0].value;
			} 
			else {
				ifstream file(args[0].value);
				if (!file.is_open()) {
					throw FileNotFoundError(args[0].value);
				}
				string content((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
				out << content;
				file.close();
			}
			return;
		}
		else {
			for (size_t i = 0; i < args.size(); ++i) {
				out << args[i].value;
				if (i + 1 < args.size()) {
					out << " ";
				}
			}
		}
	}
	else {
		string content;
		char ch;
		while (inStream->stream().get(ch)) {
			content.push_back(ch);
		}
		if (inStream->isConsole() && !content.empty()) {
			content.pop_back();
		}
		out << content;
	}

}


