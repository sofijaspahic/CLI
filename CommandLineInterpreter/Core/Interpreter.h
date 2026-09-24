#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <string>
#include "Reader.h"
#include "Translator.h"

class Interpreter {

public:
    Interpreter(std::shared_ptr<Reader> reader = std::make_shared<ConsoleReader>());
    void run();
    void setPrompt(const std::string& newPrompt);
    void processLine(const std::string& line, std::shared_ptr<OutputStream> out, std::shared_ptr<OutputStream> err);

private:
    std::string promptString = "$ "; 
    std::shared_ptr<Reader> reader; 
    Translator translator; // tekst linije -> Command objekat

    void executePipeline(std::vector<std::unique_ptr<Command>>& pipeline);
};

#endif // !INTERPRETER_H