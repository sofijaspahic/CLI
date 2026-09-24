#ifndef OUTPUTSTREAM_H
#define OUTPUTSTREAM_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

class OutputStream {
public:
	virtual ~OutputStream() = default;
	virtual std::ostream& stream() = 0;
	virtual bool isConsole() const { return false; }
};

class ConsoleOutputStream : public OutputStream {
public:
	ConsoleOutputStream(std::ostream& out);
	std::ostream& stream() override;
	bool isConsole() const override { return true; }

private:
	std::ostream& out;
};

class FileOutputStream : public OutputStream {
public:
	FileOutputStream(const std::string& filename, bool append);
	std::ostream& stream() override;
private:
	std::ofstream file;
};

class PipeStream : public OutputStream {
public:
	std::ostream& stream() override;
	std::string str() const;
private:
	std::ostringstream ss;
};

#endif // !OUTPUTSTREAM_H

