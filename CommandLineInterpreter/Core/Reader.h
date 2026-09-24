#ifndef READER_H
#define READER_H

#include <string>
#include <iostream>
#include <fstream>

constexpr size_t MAX_LINE_LENGTH = 512;

class Reader {
public:
	virtual ~Reader() = default;
	virtual bool readLine(std::string& line) = 0;
};

class ConsoleReader : public Reader {

public:
	ConsoleReader(std::istream& in = std::cin);
	bool readLine(std::string& line) override;

private:
	std::istream& in;
};

class FileReader : public Reader {

public:
	explicit FileReader(const std::string& filename);
	bool readLine(std::string& line) override;

private:
	std::ifstream file;
};

#endif // !READER_H



