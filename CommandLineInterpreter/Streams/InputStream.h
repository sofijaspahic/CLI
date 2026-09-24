#ifndef INPUTSTREAM_H
#define INPUTSTREAM_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

class InputStream {
public:
	virtual ~InputStream() = default;
	virtual std::istream& stream() = 0;

	virtual bool readLine(std::string& line) {
		return static_cast<bool>(std::getline(stream(), line));
	}

	virtual bool getChar(char& ch) {
		return static_cast<bool>(stream().get(ch));
	}

	virtual bool eof() {
		return stream().eof();
	}

	virtual bool isConsole() const { return false; }

};

class ConsoleInputStream : public InputStream {
public:
	explicit ConsoleInputStream(std::istream& in = std::cin);
	std::istream& stream() override;
	bool isConsole() const override { return true; }

private:
	std::istream& in;
};

class FileInputStream : public InputStream {
public:
	explicit FileInputStream(const std::string& filename);
	std::istream& stream() override;
private:
	std::ifstream file;
};

class StringInputStream : public InputStream {
public:
	explicit StringInputStream(const std::string& content);
	std::istream& stream() override;
private:
	std::istringstream ss;
};


#endif // !INPUTSTREAM_H


