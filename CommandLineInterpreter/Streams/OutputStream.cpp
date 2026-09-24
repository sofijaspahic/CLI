#include "OutputStream.h"
#include "../Errors/Exception.h"

using namespace std;

ConsoleOutputStream::ConsoleOutputStream(ostream& out) : out(out) {
}

ostream& ConsoleOutputStream::stream() {
    return out;
}

FileOutputStream::FileOutputStream(const string& filename, bool append)
    : file(filename, append ? (ios::out | ios::app) : (ios::out | ios::trunc)) {
    if (!file.is_open()) {
        throw FileAccessError(filename);
        }
}

ostream& FileOutputStream::stream() {
    return file;
}

ostream& PipeStream::stream() {
    return ss;
}

string PipeStream::str() const {
    return ss.str();
}
