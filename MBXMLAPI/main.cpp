/*
-------------------Mafia Bar XML-------------------
-----------------------main.cpp-----------------------
--For Testing The XML API
*/
#include "MBXMLWRITER.h"

int main()
{
    MBXMLWRITER xml("test.xml");
    xml.AddComment("Employee Information");
    xml.AddTag("Profiles");
    xml.AddElement("name");
    xml.AddValue("Alex");
    xml.EndElement();
    xml.AddElement("company");
    xml.AddValue("Hello");
    xml.EndElement();
    xml.AddElement("phone");
    xml.AddValue("(091) 123-4567");
    xml.EndElement();
    xml.EndTag();
    return 0;
}