#ifndef COMMAND_H
#define COMMAND_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <memory>
#include <functional>
#include "../Streams/OutputStream.h"
#include "../Streams/InputStream.h"

class Interpreter;

// "doslovan tekst"/ime fajla
struct Arg {
	std::string value;
	bool quoted = false;
};

class Command {

public:
	Command(std::shared_ptr<OutputStream> out = std::make_shared<ConsoleOutputStream>(std::cout),
		std::shared_ptr<OutputStream> err = std::make_shared<ConsoleOutputStream>(std::cerr));
	virtual ~Command() = default;
	virtual void execute() = 0;
	// postavljanje argumenata i tokova
	void setArgs(const std::vector<Arg>& newArgs);
	void setOutStream(std::shared_ptr<OutputStream> out);
	void setErrStream(std::shared_ptr<OutputStream> err);
	// komanda koja ne poseduje izlazni tok moze biti samo kao poslednja u cevovodu
	virtual bool producesOutput() const { return true; }
	virtual bool acceptsInput() const { return false; }
	std::ostream* consoleOutput() const;
	virtual bool needsNewline() const { return producesOutput(); }

protected:
	std::shared_ptr<OutputStream> outStream;
	std::shared_ptr<OutputStream> errStream;
	std::vector<Arg> args;

	const Arg& requireSingleArg(const std::string& cmdName) const;
	static std::size_t parseCountOption(const std::string& arg, const std::string& prefix,
		const std::string& cmdName);

};

// komande bez obrade ulaznog toka - time,date,prompt
class SimpleCommand : public Command {
public:
	using Command::Command;
	virtual void execute() = 0;
protected:
	static std::tm localNow();
};

// komande koje transformisu ulazni tekst - echo,wc,tr,head
class StreamCommand : public Command {
public:
	StreamCommand(std::shared_ptr<InputStream> in = std::make_shared<ConsoleInputStream>(std::cin),
		std::shared_ptr<OutputStream> out = std::make_shared<ConsoleOutputStream>(std::cout),
		std::shared_ptr<OutputStream> err = std::make_shared<ConsoleOutputStream>(std::cerr));
	
	void setInStream(std::shared_ptr<InputStream> in);
	virtual bool hasArgumentInput() const;
	bool acceptsInput() const override { return true; }

protected:
	std::shared_ptr<InputStream> inStream;
	void withResolvedInput(const Arg* filename, const std::function<void(std::istream&)>& consume);

	std::string readAllInput(const Arg* filename);
	std::vector<std::string> readInputLines(const Arg* filename);
	static std::string joinLines(const std::vector<std::string>& lines);
};

// komande koje rade sa file sistemom - touch,truncate,rm
class FileCommand : public Command {
public:
	using Command::Command;
	virtual void execute() = 0;

protected:
	bool fileExists(const std::string& filename) const;
	void requireExistingFile(const std::string& filename) const;
};

// komande sa paketnom obradom - batch
class BatchCommand : public Command {
public:
	BatchCommand(Interpreter* interp=nullptr,
		std::shared_ptr<OutputStream> out = std::make_shared<ConsoleOutputStream>(std::cout),
		std::shared_ptr<OutputStream> err = std::make_shared<ConsoleOutputStream>(std::cerr));

	void setInterpreter(Interpreter* interp);

protected:
	Interpreter* interpreter;

};


#endif // !COMMAND_H

