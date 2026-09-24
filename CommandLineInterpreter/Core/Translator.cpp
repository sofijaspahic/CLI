#include "Translator.h"
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
#include "../Commands/BatchCommands/Batch.h"
#include "../Errors/Exception.h"
#include <iostream>

using namespace std;

namespace {
    bool isAllowedBareChar(char c) {
        if (isalnum(static_cast<unsigned char>(c))) return true;
        if (c == ' ' || c == '\t') return true;
        if (c == '"' || c == '|' || c == '<' || c == '>') return true;
        static const string extra = "-._/\\:";
        return extra.find(c) != string::npos;
    }

    std::string trim(const std::string& text) {
        size_t begin = text.find_first_not_of(" \t");
        if (begin == std::string::npos) return "";
        size_t end = text.find_last_not_of(" \t");
        return text.substr(begin, end - begin + 1);
    }

    bool isNumber(const std::string& text) {
        if (text.empty()) return false;
        for (char c : text) {
            if (!isdigit(static_cast<unsigned char>(c))) return false;
        }
        return true;
    }
}

void Translator::validateLexical(const string& line) {
    bool inQuotes = false;
    vector<size_t> badPositions; 

    for (size_t i = 0; i < line.size(); ++i) {
        char c = line[i];
        if (c == '"') {
            inQuotes = !inQuotes;
            continue;
        }
        if (inQuotes) {
            continue;
        }
        if (!isAllowedBareChar(c)) {
            badPositions.push_back(i);
        }
    }

    if (!badPositions.empty()) {
        throw UnexpectedCharacterError(line,badPositions);
    }

    if (inQuotes) {
        throw MissingClosingQuoteError(line);
    }
}

vector<Arg> Translator::parseArgs(istringstream& ss) {
    vector<Arg> args;

    while (ss >> ws && ss.peek() != EOF) {
        string arg;
        if (ss.peek() == '-') {
            arg += (char)ss.get();
            if (ss.peek() == '"') {
                ss.get();
                string quoted;
                getline(ss, quoted, '"');
                if (ss.eof()) {
                    throw MissingClosingQuoteError(ss.str());
                }
                arg += '"' + quoted + '"';      
            }
            else {
                string rest;
                ss >> rest;
                arg += rest;
            }
            args.push_back(Arg{ arg,false });
        }
        else if (ss.peek() == '"') {
            ss.get();
            getline(ss, arg, '"');
            if (ss.eof()) {
                throw MissingClosingQuoteError(ss.str());
            }
            args.push_back(Arg{ arg,true });
        }
        else {
            ss >> arg;
            if (!arg.empty()) {
                args.push_back(Arg{ arg,false });
            }
        }
    }

    return args;
}

size_t Translator::findRedirectionStart(const string& line) {
    bool inQuotes = false;
    for (size_t i = 0; i < line.size(); ++i) {
        char c = line[i];
        if (c == '"') {
            inQuotes = !inQuotes;
        }
        else if (!inQuotes && (c == '<' || c == '>')) {
            return i;
        }
    }
    return string::npos; 
}

Redirection Translator::parseRedirections(const string& text) {
    Redirection redir;
    size_t i = 0;
    size_t n = text.size();

    auto skipSpaces = [&]() {
        while (i < n && (text[i] == ' ' || text[i] == '\t'))
            ++i;
    };

    auto readFilename = [&]() -> string {
        skipSpaces();
        size_t start = i;
        while (i < n && text[i] != ' ' && text[i] != '\t' && text[i] != '<' && text[i] != '>') {
            ++i;
        }
        return text.substr(start, i - start);
    };

    while (i < n) {
        skipSpaces();
        if (i >= n) break;

        if (text[i] == '<') {
            ++i;
            string filename = readFilename();
            if (filename.empty()) {
                throw MissingRedirectionFilenameError('<');
            }
            if (redir.hasInput) {
                throw DuplicateInputRedirectionError();
            }
            redir.hasInput = true;
            redir.inputFile = filename;
        }
        else if (text[i] == '>') {
            ++i;
            bool append = false;
            if (i < n && text[i] == '>') {
                append = true;
                ++i;
            }
            string filename = readFilename();
            if (filename.empty()) {
                throw MissingRedirectionFilenameError('>');
            }
            if (redir.hasOutput) {
                throw DuplicateOutputRedirectionError();
            }
            redir.hasOutput = true;
            redir.outputFile = filename;
            redir.append = append;
        }
        else {
            throw InvalidRedirectionSyntaxError(text[i]);
        }
    }
    return redir;
}

vector<string> Translator::splitPipeline(const string& text) {
    vector<string> segments;
    bool inQuotes = false;
    size_t start = 0;

    for (size_t i = 0; i < text.size(); ++i) {
        char c = text[i];
        if (c == '"') {
            inQuotes = !inQuotes;
        }
        else if (!inQuotes && c == '|') {
            segments.push_back(text.substr(start, i - start)); 
            start = i + 1;
        }
    }
    segments.push_back(text.substr(start)); 
    
    return segments;
}

unique_ptr<Command> Translator::createCommand(const string& cmdName, Interpreter* interpreter) {
    if (cmdName == "time") return make_unique<Time>();
    if (cmdName == "date") return make_unique<Date>();
    if (cmdName == "echo") return make_unique<Echo>();
    if (cmdName == "touch") return make_unique<Touch>();
    if (cmdName == "wc") return make_unique<Wc>();
    if (cmdName == "rm") return make_unique<Rm>();
    if (cmdName == "truncate") return make_unique<Truncate>();
    if (cmdName == "head") return make_unique<Head>();
    if (cmdName == "tr") return make_unique<Tr>();
    if (cmdName == "prompt") return make_unique<Prompt>(interpreter);
    if (cmdName == "batch") return make_unique<Batch>(interpreter);
  
    throw UnknownCommandError(cmdName);
}

vector<unique_ptr<Command>> Translator::translate(const string& line, Interpreter* interpreter, shared_ptr<OutputStream> defaultOut, shared_ptr<OutputStream> defaultErr) {
    
    validateLexical(line);

    size_t redirPos = findRedirectionStart(line);
    string commandsPart = (redirPos == string::npos) ? line : line.substr(0, redirPos);
    string redirPart = (redirPos == string::npos) ? "" : line.substr(redirPos);
    
    Redirection redir = parseRedirections(redirPart);
    vector<string> segments = splitPipeline(commandsPart);
    vector<unique_ptr<Command>> pipeline;
    
    for (const string& segment : segments) {
        istringstream ss(segment);
        string cmdName;
        ss >> cmdName;

        if (cmdName.empty()) {
            throw EmptyCommandNameError();
        }
        
        vector<Arg> args = parseArgs(ss);
        unique_ptr<Command> command = createCommand(cmdName, interpreter);
        command->setArgs(args);
        pipeline.push_back(move(command));

    }
    
    // provera pravila cevovoda (samo ako stvarno ima vise od jedne komande)
    if (pipeline.size() > 1) {

        for (size_t i = 0; i < pipeline.size(); ++i) {
            bool isFirst = (i == 0);
            bool isLast = (i == pipeline.size() - 1);

            if (!isFirst) {
                if (!pipeline[i]->acceptsInput()) {
                    throw PipeInputNotSupportedError();
                }
                StreamCommand* streamCmd = static_cast<StreamCommand*>(pipeline[i].get());
                if (streamCmd->hasArgumentInput()) {  
                    throw ArgumentInputConflictError("receive piped input");
                }
            }

            if (!isLast && !pipeline[i]->producesOutput()) {
                throw PipeOutputNotSupportedError();
            }
        }
    }

    // podrazumevani tokovi
    for (auto& command : pipeline) {
        command->setErrStream(defaultErr);
    }
    pipeline.back()->setOutStream(defaultOut);

    // primena redirekcije
    if (redir.hasInput) {
        if (!pipeline.front()->acceptsInput()) {
            throw RedirectionInputNotSupportedError();
        }
        StreamCommand* streamCmd = static_cast<StreamCommand*>(pipeline.front().get());
        if (streamCmd->hasArgumentInput()) {
            throw ArgumentInputConflictError("redirect input");
        }
        streamCmd->setInStream(make_shared<FileInputStream>(redir.inputFile));
    }

    if (redir.hasOutput) {
        if (!pipeline.back()->producesOutput()) {
            throw RedirectionOutputNotSupportedError();
        }
        pipeline.back()->setOutStream(make_shared<FileOutputStream>(redir.outputFile, redir.append));
    }

    return pipeline;
}