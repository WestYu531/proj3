#include "DSVReader.h"
#include "DSVWriter.h"
#include <memory>
#include <string>
#include "StringDataSource.h"
#include "DataSource.h"
#include "StringUtils.h"
#include <iostream>
#include <cctype>
struct CDSVReader::SImplementation         //only read in a double quote as part of the cell if it is two consecutive double quotes.
{
    std::shared_ptr< CDataSource > source;
    char delim;

};

CDSVReader::CDSVReader(std::shared_ptr< CDataSource > src, char delimiter)  //Constructor for DSV reader, src specifies the data source and delimiter specifies the delimiting character
{
    DImplementation = std::make_unique<SImplementation>();
    DImplementation->source = src;
    DImplementation->delim = delimiter;
}

CDSVReader::~CDSVReader()   // Destructor for DSV reader
{

}

bool CDSVReader::End() const  //Returns true if all rows have been read from the DSV
{
    return CDSVReader::DImplementation->source->End();

}

bool CDSVReader::ReadRow(std::vector<std::string> &row)   //Returns true if the row is successfully read, one string will be put in the row per column. 
{

    //if tmp (char) == \n and !flag, don't add char to string and end the row

    //clear whatever is in the row before you write to it.
    //read in strings from the source, separate them by delimiter, put them in the row vector. The source is a CStringDataSource while the row should be a vector of strings.

    if (row.size() != 0)
    {
        row.clear();
    }

    int flag = 0;
    int firstchar = 1;
    std::string rowelem = "";
    while(true)                  //iterate through every character in the source string
    {
    char tmp;
    if (!CDSVReader::DImplementation->source->Get(tmp))
    return false;


    

    if ( (tmp != CDSVReader::DImplementation->delim) && (tmp != '\n') && (tmp != '\"')) //if not delimeter or newline or double quote, add to row elem.    
    {
        rowelem = rowelem + tmp;

        /* if ( (tmp == '\"') && (!flag))   
        {
        flag = 1;
        }

        else if ( (tmp == '\"') && (flag))
        {
        flag = 0;
        }
 */
    }

    else if ( (tmp == '\"') && (!flag) && (firstchar))    //starting quotes
        {
        std::cout << "current elem (should be starting quotes): " << tmp << std::endl;
        flag = 1;
        firstchar = 0;

        }

    else if ( (tmp == '\"') && (!firstchar))  //ending quotes or escaped double quote
        {
            //std::cout << "current elem (should be ending or escaped): " << tmp << std::endl;

        /* char prevtmp = tmp;
        std::cout << "current elem: " << prevtmp << std::endl;
 */
        if (!CDSVReader::DImplementation->source->Peek(tmp))
        return false;

        //std::cout << "next elem by peek: " << tmp << std::endl;

        if (tmp == '\"')
        {
            //std::cout<<"the previous quote was an escaped quote" << std::endl;
/*             if (!CDSVReader::DImplementation->source->Get(tmp))
            return false; */
            rowelem = rowelem + '\"';
            flag = 1;

        }

        else if (tmp != '\"')
        {
            //std::cout<< "the previous quote was an ending quote\n";
            flag = 0;
        }

        //check if next elem is double quote. 
        //if double quote, that means it is an escaped double quote. add only one double quote to the rowelem and don't turn the flag off.
        //if next elem is not a double quote, that means current elem is closing quotes. turn the flag off and don't add double quotes to rowelem
        }

    else if ((tmp == CDSVReader::DImplementation->delim) && flag) //if delim is part of string
    {
        rowelem = rowelem + tmp;

    }

    else if ((tmp == CDSVReader::DImplementation->delim) && !flag)  //if delim is not part of string
    {
        //if char is the delimeter and its not part of the string, don't add to string, end the vector element (cell). make a new string
        row.push_back(rowelem);
        rowelem = "";
    }

    else if ((tmp == '\n') && flag) //if newline is part of string
    {
        rowelem = rowelem + tmp;

    }

    else if ((tmp == '\n') && !flag)  //if newline is not part of string-->end the row
    {
        //if char is the delimeter and its not part of the string, don't add to string, end the vector 
        row.push_back(rowelem);
        break;
    }


    if (CDSVReader::DImplementation->source->End())
    {
        row.push_back(rowelem);
        break;
    }

    }
    return true;

}