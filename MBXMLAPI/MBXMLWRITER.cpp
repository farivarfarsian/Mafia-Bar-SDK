#include "MBXMLWRITER.h"
MBXMLWRITER::MBXMLWRITER(const char* filename)
{
    struct stat buffer;
    if ((stat(filename, &buffer) == 0) == true)
    {
        printf_s("The File: %s Exists Already\n", filename);
    }
    else
    {
        FileXML.open(filename, std::ios::out);
        if (FileXML.is_open())
        {
            printf_s("XML File: %s Created!\n", filename);
            FileXML << "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n";
        }
        else
        {
            printf_s("Unable to Open The File\n");
        }
    }
}
MBXMLWRITER::~MBXMLWRITER(void) { FileXML.close(); }
bool MBXMLWRITER::OpenFile(const char* filename)
{
    struct stat buffer;
    if ((stat(filename, &buffer) == 0) == true)
    {
        printf_s("The File: %s Exists Already\n", filename);
        return false;
    }
    FileXML.open(filename, std::ios::out);
    if (FileXML.is_open())
    {
        printf_s("XML File: %s Created!\n", filename);
        FileXML << "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n";
        return true;
    }
    else { return false; }
}

void MBXMLWRITER::AddTag(const char* Tag)
{
    FileXML << std::string(Tabs, '\t');
    TagNames.Resize(Tags + 1);
    FileXML << "<" << Tag << ">\n";
    TagNames[Tags] = Tag;
    Tabs += 1;
    Tags += 1;
}

void MBXMLWRITER::EndTag()
{
    Tabs--;
    FileXML << std::string(Tabs, '\t');
    FileXML << "</" << TagNames[Tags - 1] << ">\n";
    TagNames.PopBack();
    Tags--;
}

void MBXMLWRITER::AddElement(const char* element)
{
    FileXML << std::string(Tabs, '\t');
    ElementNames.Resize(Elements + 1);
    ElementNames[Elements] = element;
    Elements += 1;
    FileXML << "<" << element << ">";
}

void MBXMLWRITER::EndElement()
{
    FileXML << "</" << ElementNames[Elements - 1] << ">\n";
    ElementNames.PopBack();
    Elements--;
}

void MBXMLWRITER::AddAttribute(const char* attrib)
{
}

void MBXMLWRITER::AddComment(const char* comment) { FileXML << "<!--" << comment << "-->\n"; }
