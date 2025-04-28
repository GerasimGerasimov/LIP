#pragma once

#include <string>
#include <vector>
#include <functional>
#include <map>

#define i5N "XN"
#define iSwitchStatus "SS8"

enum class ParcerResult
{
    COMMENT = -1,
    SECTION = -2,
    END = -3,
    NOTKEYVALUE = -4
};

typedef struct
{
    char* tag;
    int result;
} TSectionReadResult;

class IniParser
{
public:
    static IniParser& getInstance();

    void init(void);
    bool setSectionToRead(const char* SectionName);
    TSectionReadResult getNextTagChar();
    std::string getNextTagString();
    void setRoot(char* root, int size);
    std::vector<std::string> getListOfDelimitedString(char delimiter, char* src, int size);
    std::vector<std::string> getListOfDelimitedStrInclude(char delimiter, char* src, int size);
    std::string getElement(char delimiter, char** ptr, int& size);
    std::string getElement(char delimiter, char* ptr);
    char* getElementPtrByNumber(int number, char delimiter, char* src);
    int isDelimiterSizeLimited(char delimiter, char*& src, int& size);
    int getStringLenght(char** ptr);
private:
    IniParser();
    IniParser(const IniParser&) = delete;
    IniParser& operator=(const IniParser&) = delete;
    IniParser(const IniParser&&) = delete;
    IniParser& operator=(const IniParser&&) = delete;

    void resetFind(char* start);
    char* getSectionEntryPoint(const char* SectionName);
    int getTagString(char** position);
    int isDelimiter(char** ptr, char Delimiter);
    int getSectionLinesCount(char* SectionName);
    std::string getElementInclude(char delimiter, char** ptr, int& size);
    char* Root;
    char* SearchPointer;
    int RootSize;
};

namespace Parser
{
    std::vector<std::string> splitString(std::string delimiter, const std::string& text);
    void parseConfigurarion(std::vector<std::string>& Configuration, std::map<std::string, std::function<void(std::vector<std::string>& Type)>> &func);
}