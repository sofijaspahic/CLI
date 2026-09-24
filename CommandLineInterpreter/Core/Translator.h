#ifndef TRANSLATOR_H
#define TRANSLATOR_H

#include <string>
#include <vector>
#include <sstream>
#include <memory>
#include "../Commands/Command.h"

class Interpreter;

struct Redirection {
	bool hasInput = false;
	std::string inputFile;
	bool hasOutput = false;
	std::string outputFile;
	bool append = false;
};

class Translator {
public:
	// vraca ceo cevovod, kad nema | to je vektor sa jednim elementom
	std::vector<std::unique_ptr<Command>> translate(const std::string& line, Interpreter* interpreter,
		std::shared_ptr<OutputStream> defaultOut, std::shared_ptr<OutputStream> defaultErr);

private:
	static void validateLexical(const std::string& line);
	static std::vector<Arg> parseArgs(std::istringstream& ss);
	static size_t findRedirectionStart(const std::string& line);
	static Redirection parseRedirections(const std::string& text);
	static std::vector<std::string> splitPipeline(const std::string& text);
	static std::unique_ptr<Command> createCommand(const std::string& cmdName, Interpreter* interpreter);
};

#endif // !TRANSLATOR_H
