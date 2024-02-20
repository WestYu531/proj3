# Project 3 README
Zhontian Yu(919551080)
# partner
Xinbang Yuan 920853308

## Project Status
the code part is finished and debugged.

## Known Issues
 there is some some with output in the terminal. It mey be caused by the googletest

 root@2dc163c7edd1:/workspace/proj2# git push origin main
error: src refspec main does not match any
error: failed to push some refs to 'https://github.com/WestYu531/project2.git'


## Code References
* [Some other link](https://en.wikipedia.org/wiki/Delimiter-separated_values#:~:text=Formats%20that%20use%20delimiter%2Dseparated,data%20in%20a%20delimited%20format.)

## Generative AI Use
Input:
  User
makefile:17: *** missing separator.  Stop.

AI ouput:
  The error message makefile:17: *** missing separator. Stop. typically occurs in a Makefile when there is an incorrect indentation or a missing tab character. In Makefiles, commands under a rule must be preceded by a single tab character, not spaces. Here's how you can address this issue:

Open your Makefile and go to line 17.
Check if the line is part of a command under a rule. A rule looks something like this:
make
Copy code
target: dependencies
    command
Ensure that the command line (which should be right after the dependencies line) starts with a tab character and not spaces. If it starts with spaces, replace those spaces with a single tab.
If the line is not meant to be a command (for example, if it's part of a variable assignment or another declaration), make sure it is formatted correctly according to Makefile syntax.
If your text editor replaces tabs with spaces automatically, you may need to change its settings to keep tabs for Makefiles.
After making the changes, save your Makefile and try running make again.
If you're using a text editor like Vim, you can set it to show tabs and spaces differently to help spot these issues. In Vim, for example, you can use :set list to show whitespace characters. In other editors, there might be similar features or plugins available.

Input：
How to solve type conversion errors

AI output:

ChatGPT
Sure, here's how to address type conversion errors in your code:

Mismatched Data Types:
If your TNodeID or TWayID is a specific integer type such as long, int, or long long, you need to ensure that you are using the correct conversion function. For example:
If TNodeID is an int type, you should use stoi (string to integer).
If TNodeID is a long type, you should use stol (string to long integer).
If TNodeID is a long long type, you should use stoll (string to long long integer).
Example Code (assuming TNodeID is long type):
cpp
Copy code
NewNode->id = stol(Entity.AttributeValue("id"));
Exception Handling:
The string to number conversion functions (such as stoi, stol, stoll) might throw std::invalid_argument or std::out_of_range exceptions. You should catch and properly handle these exceptions to prevent your program from crashing unexpectedly:
cpp
Copy code
try {
    NewNode->id = std::stol(Entity.AttributeValue("id"));
} catch (const std::invalid_argument& e) {
    // Handle the invalid argument exception, e.g., by setting a default value or logging the error
    NewNode->id = 0; // Or some other default value
} catch (const std::out_of_range& e) {
    // Handle the out-of-range exception
    NewNode->id = 0; // Or some other default value
}
Data Validation:
Before performing the type conversion, validate whether the data is valid. For example, you can check if the string contains only digits before attempting to convert it to a number. This can be done using regular expressions or other string validation methods.
Use Appropriate Types:
Ensure that your data types match the expected range and precision of the data. If the numbers you are dealing with could be very large, it might be safer to use long long instead of int or long.
Code Consistency:
Ensure consistent use of the same data types and conversion methods throughout your codebase. This helps prevent mismatch errors and makes your code easier to understand and maintain.
By following these steps, you should be able to resolve type conversion errors in your code. If you encounter specific conversion issues, please provide details so I can offer more targeted advice.

Input:
make: *** No rule to make target 'testsrc/OSMTest.cpp', needed by 'obj/OSMTest.o'.  Stop.

AI output:
The error message from the make tool indicates that in your Makefile, there is a rule to build the target obj/OSMTest.o, but this target depends on the source file testsrc/OSMTest.cpp, which either does not exist in your project directory or cannot be found by make.

To solve this issue, you can follow these steps:

Check File Existence: Ensure that the testsrc/OSMTest.cpp file actually exists in your project directory. Check the path to the file to make sure there are no typos.
Check the Makefile: Look into your Makefile to find the rule that generates obj/OSMTest.o. Ensure that the dependency path correctly points to the testsrc/OSMTest.cpp file. If the file path is incorrect, correct it.
Directory Structure: Make sure your working directory is correct, i.e., you are running the make command in the directory that contains the Makefile. If you are in the wrong directory, make will not be able to find the correct files.
Clean and Rebuild: If a previous build process was interrupted, it might leave incomplete or corrupted files. You can run make clean (assuming your Makefile defines a clean rule) to remove old object files and targets, and then rerun make.
If you have followed these steps and still encounter issues, you may need to review the path and rule definitions in your Makefile to ensure they are correct. If you have the content of the Makefile and need specific help, please provide the relevant parts, and I can assist you further in resolving the issue.

input:
g++ -o ./obj/OpenStreetMap.o -c -std=c++17 -I ./include -I /opt/homebrew/include ./src/OpenStreetMap.cpp
./src/OpenStreetMap.cpp: In member function ‘std::shared_ptr<CStreetMap::SWay> COpenStreetMap::SImplementation::WayByID(CStreetMap::TWayID) const’:
./src/OpenStreetMap.cpp:233:52: error: passing ‘const std::unordered_map<long unsigned int, std::shared_ptr<COpenStreetMap::SImplementation::SWay> >’ as ‘this’ argument discards qualifiers [-fpermissive]
  233 |         std::cout<<"Mappedways[0] = "<<MappedWays[0]<<std::endl;
      |                                                    ^
In file included from /usr/include/c++/12/unordered_map:47,
                 from ./src/OpenStreetMap.cpp:

ouput:
The error you're encountering in your C++ code is due to trying to access a member of std::unordered_map in a const context. Specifically, when you're trying to output MappedWays[0] using std::cout, the issue arises because your function WayByID is marked as const, meaning it promises not to modify any member variables of the COpenStreetMap::SImplementation class.

However, the operator[] of std::unordered_map is a non-const function because it potentially modifies the map: if the key doesn't exist, it inserts a new element with that key. This is contradictory to what the const qualifier of your function promises, hence the compiler error.

To solve this issue, you should use the at() method instead of operator[] when accessing elements in a const context. Unlike operator[], at() does not insert a new element if the key does not exist; instead, it throws an exception of type std::out_of_range. This behavior makes it suitable for use in const contexts.