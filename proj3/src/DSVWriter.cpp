#include "DSVWriter.h"
#include <memory>
#include <string>
#include "StringDataSink.h"
#include "DataSink.h"
#include "StringUtils.h"
#include <iostream>

struct CDSVWriter::SImplementation
{
    std::shared_ptr< CDataSink > sink;
    char delim;
    bool flag;
    bool firstrow;
};



CDSVWriter::CDSVWriter(std::shared_ptr< CDataSink > sink, char delimiter, bool quoteall)  //Constructor for DSV writer, sink specifies the data destination, delimiter specifies the delimiting character, and quoteall specifies if all values should be quoted or only those that contain the delimiter, a double quote, or a newline
{
    DImplementation = std::make_unique<SImplementation>();
    DImplementation->sink= sink;
    DImplementation->delim = delimiter;
    DImplementation->flag = quoteall;
    DImplementation->firstrow = true;


}

CDSVWriter::~CDSVWriter()   //Destructor for DSV writer
{
    
}

bool CDSVWriter::WriteRow(const std::vector<std::string> &row)  // Returns true if the row is successfully written, one string per column should be put in the row vector
{
//for each character in each string, put the character in the sink
//std::shared_ptr< CStringDataSink > sink = std::make_shared<CStringDataSink>();



std::vector<std::string> temp = row;
int count = 0;


 if (!CDSVWriter::DImplementation->firstrow)  //if not firstrow, add newline
{
    if(!CDSVWriter::DImplementation->sink->Put('\n')) 
        return false;
}  

for (int i = 0; i < temp.size(); i++)
{
     if (CDSVWriter::DImplementation->flag == true) 
    {
        if(!CDSVWriter::DImplementation->sink->Put('\"')) 
        return false;
    }

    if ( (CDSVWriter::DImplementation->flag == false) && ( (row[i].find(CDSVWriter::DImplementation->delim) != std::string::npos) || (temp[i].find('\"') != std::string::npos) || (temp[i].find('\n') != std::string::npos) ) ) //if delimeter, double quote, or newline is in the string, add double quotes
    {
        if(!CDSVWriter::DImplementation->sink->Put('\"')) 
        return false; 
    }
    
    if (temp[i].find('\"') != std::string::npos)   //Double quote character in the cell must be replaced with two double quotes. 

        {
            temp[i] = StringUtils::Replace(temp[i], "\"", "\"\"");
        }

    for (int j = 0; j < temp[i].length(); j++)
    {
        if(!CDSVWriter::DImplementation->sink->Put(temp[i][j])) 
        return false;
    }
 
    if (CDSVWriter::DImplementation->flag == true)  
    {
        if(!CDSVWriter::DImplementation->sink->Put('\"'))
        return false;
    }

    if ( (CDSVWriter::DImplementation->flag == false) && ( (temp[i].find(CDSVWriter::DImplementation->delim) != std::string::npos) || (temp[i].find('\"') != std::string::npos) || (temp[i].find('\n') != std::string::npos) ) ) //if delimeter, double quote, or newline is in the string, add double quotes
    {
        if(!CDSVWriter::DImplementation->sink->Put('\"'))
        return false; 
    }

    count = count+1;

    if (count < temp.size())
    {
    if(!CDSVWriter::DImplementation->sink->Put(DImplementation->delim))   //to separate the elems
    return false;
    }  

    
}
//check the string. if it is successfully written, return true
CDSVWriter::DImplementation->firstrow = false;
return true;
}



/* Values that have either the delimiter, double quote character, or newline must be quoted 
with double quotes.  
Double quote character in the cell must be replace with two double quotes. 
An empty line is a valid row where there are not values  */