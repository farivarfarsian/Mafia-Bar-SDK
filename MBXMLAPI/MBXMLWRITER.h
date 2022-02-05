#pragma once
#include <fstream>
#include "Vector.h"
#include <string> //only getline Function
/*
TODO:
    Creating Attributes
    Storing Elements Into a Custom Variable Type
    Deleting Elements
    Error Handling
*/
static enum MBXMLSTATUS
{
    FILE_ALREADY_EXISTS_W = 20091,
    UNABLE_TO_OPEN_FILE_W = 20092
};
class MBXMLWRITER
{
public:
    MBXMLWRITER() = default;
    explicit MBXMLWRITER(const char* filename);
    ~MBXMLWRITER(void);
    bool OpenFile(const char* filename);
public:
    void AddTag(const char* Tag);
    void EndTag();
public:
    void AddElement(const char* element);
    void EndElement();
public:
    void AddAttribute(const char* attrib);
    void AddComment(const char* comment);
    template<typename T>
    inline void AddValue(const T& value) { FileXML << value; }
public: //IN PROGRESS
    std::string Temp_ReadEntierFile()
    {
        FileXML.open("test.xml", std::ios::in);
        std::string s;
        getline(FileXML, s, '\0');
        FileXML.close();
        return s;
    }
private:
    std::fstream FileXML;
    int Tabs = 0, Tags = 0, Elements = 0;
    MafiaBar::SDK::Vector<const char*> TagNames;
    MafiaBar::SDK::Vector<const char*> ElementNames;
};
