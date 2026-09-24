#include "../Core/Interpreter.h"
#include "../Commands/SimpleCommands/Time.h"
#include "../Commands/SimpleCommands/Date.h"
#include "../Commands/SimpleCommands/Prompt.h"
#include "../Commands/StreamCommands/Echo.h"
#include "../Commands/StreamCommands/Wc.h"
#include "../Commands/StreamCommands/Head.h"
#include "../Commands/StreamCommands/Tr.h"
#include "../Commands/FileCommands/Rm.h"
#include "../Commands/FileCommands/Truncate.h"
#include "../Commands/FileCommands/Touch.h"
#include "../Errors/Exception.h"
#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

Interpreter::Interpreter(shared_ptr<Reader> reader)
    : reader(reader) {}

void Interpreter::run() {

    string line;

    while (true) {
        cout << promptString;  

        auto out = make_shared<ConsoleOutputStream>(cout);
        auto err = make_shared<ConsoleOutputStream>(cerr);

        if (!reader->readLine(line)) break;  

        if (line.empty()) continue; 
        
        processLine(line, out, err);   
    }
}

void Interpreter::setPrompt(const string& newPrompt) {
    promptString = newPrompt;
    if (promptString.empty() || promptString.back() != ' ') {
        promptString += " ";
    }
}

void Interpreter::processLine(const string& line, shared_ptr<OutputStream> out, shared_ptr<OutputStream> err) {
    try {
        vector<unique_ptr<Command>> pipeline = translator.translate(line, this, out, err);
        if (pipeline.empty()) return;
        executePipeline(pipeline);

        Command* last = pipeline.back().get();
        if (last->needsNewline()) {
            if (ostream* console = last->consoleOutput()) {
                *console << "\n";
            }
        }
    } catch (const LexicalError& e) {
        cerr << e.what() << endl;
    } catch (const Exception& e) {
        cerr << "Error: " << e.what() << endl;
    } catch (const exception& e) {
        cerr << "Unexpected error: " << e.what() << endl;
    }

}

void Interpreter::executePipeline(vector<unique_ptr<Command>>& pipeline) {
    for (size_t i = 0; i + 1 < pipeline.size(); ++i) {
        auto buffer = make_shared<PipeStream>();
        pipeline[i]->setOutStream(buffer);
        pipeline[i]->execute();

        if (!pipeline[i + 1]->acceptsInput()) {
            throw PipeInputNotSupportedError();
        }
        StreamCommand* next = static_cast<StreamCommand*>(pipeline[i + 1].get());
        next->setInStream(make_shared<StringInputStream>(buffer->str()));
    }

    pipeline.back()->execute();
}

