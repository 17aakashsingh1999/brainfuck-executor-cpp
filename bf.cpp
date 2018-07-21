/* This is a small program that runs brainfuck code.
   Author: Aakash Singh
   Date: 20 July 2018
*/

#include <iostream>  //handles input and output to the console
#include <fstream>  //handles input and output to files
#include <vector>  //vector data structure
#include <stack>  //stack data structure

using namespace std;

void help()  //displays the help message
{
    cout << "Usage:\n\tbf.exe file_name.bf";
}

bool verify(vector<char> source)  //function to check for correct bracket sequence
{
    int st = 0;  // st stands for stack
    /* Increment st whenever [ is encountered and decrement st whenever ] is encountered.
       At every step check if st is less than 0. For a correct sequence of brackets st can never be 0
       as when reading character by character the number of [ can't exceed number of ] at any point in time.
       In the end st should be 0 as total number of [ is equal to total number of ] */
    for(int i=0;i < source.size(); i++)
    {
        if(source[i] == '[') st++;
        else if(source[i] == ']') st--;
        if(st < 0) return 0;
    }
    if(st == 0) return 1;
    return 0;
}

void filter(vector<char>& source)
{
    int i;
    char chars[] = {'<', '>', '+', '-', ',', '.', '[', ']'}; //chars defines a list of permitted characters
    vector<char>::iterator it,it2;
    //removes comments from source
    for(it=source.begin();it+1 != source.end();it++)
    {
        if(*it == '/' && *(it+1) == '*')  //search beginning of the comment
        {
            //search for ending of the comment
            for(it2 = it+2; it2+1 != source.end(); it2++)
            {
                if(*it2 == '*' && *(it2+1) == '/')break;
            }
            if(it2+1 == source.end()) //if the iterator goes out of bounds then the end is not found
            {
                cout<<"Comment not closed error.\n";
                return;
            }
            source.erase(it,it2+1);  //delete the found comment
            it--;  
            /* 'it--' is required as the current character at position 'it' has been changed and in the next iteration
               after 'it++' inside the for loop the new character at the current 'it' position will be skipped */
        }
    }
    //removes extra chars from source
    for(it=source.begin(); it != source.end(); it++)
    {
        for(i=0;i<8;i++)
        {
            if(*it == chars[i]) break;
        }
        if(i == 8)  //8 is the size of the chars array.
        {
            source.erase(it);
            it--; //prevents skipping the new character at the current position
        }
    }
}

void execute(vector<char> source)
{
    int ptr = 0; //defines the pointer for the buffer
    stack<int> brackets;  //stack to track position of the brackets
    vector<char> buffer;  //defines the buffer (memory cells in brainfuck)
    buffer.push_back(0);  //initialize the first memory cell
    int i = 0;  //iterator to read the source
    while (i < source.size())
    {
        if (source[i] == '>')  //moves pointer 1 position to the right
        {
            if (ptr == buffer.size() - 1) //creates new cell to the right if it doesn't exist
            {
                buffer.push_back(0);
            }
            ptr++; //move the pointer to the next cell
        }
        else if (source[i] == '<') //moves the pointer 1 position to the left
        {
            if (ptr == 0)  //if pointer is at 0 position then no exists to the left. So < is an illegal operation.
            {
                cout << "Runtime error.\n";  //throws Runtime error
                return;  //stops execution of the code
            }
            ptr--;  //move the pointer to the previous cell
        }
        else if (source[i] == '+')  //increment the current memory cell value by 1
        {
            buffer[ptr]++;
        }
        else if (source[i] == '-')  //decrement the current memory cell value by 1
        {
            buffer[ptr]--;
        }
        else if (source[i] == '.')  //print the character at the current memory cell
        {
            if (buffer[ptr] >= 0)
                cout << buffer[ptr];
        }
        else if (source[i] == ',')  //read a character to the current memory cell
        {
            cout << "Enter a character: ";
            cin >> buffer[ptr];
        }
        else if (source[i] == '[')  //track the opening bracket
        {
            brackets.push(i);
        }
        else if (source[i] == ']')  //track the closing bracket
        {
            //if current cell value is 0 exit loop or continue with the loop
            if (buffer[ptr] == 0) 
            {
                brackets.pop();
            }
            else
            {
                i = brackets.top();
            }
        }
        i++;  //read the next command in the source
    }
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        help();
        return 0;
    }
    ifstream fin;
    fin.open(argv[1]);
    if (!fin) //check if file exists
    {
        cout << "File " << argv[1] << " not found.\n";
        help();
        return 0;
    }
    fin.seekg(0);  //reset file pointer to beginning of file
    char reader;  //char variable to read characters from the file
    vector<char> source;
    while (fin >> reader)  //read characters from the file
    {
        source.push_back(reader);  //add the read characters to the vector
    }
    fin.close();  //close file after reading

    if (!verify(source)) //check syntax, correct brackets
    {
        cout << "Syntax Error.\n";
        help();
        return 0;
    }
    filter(source); //filter out comments and other characters

    execute(source); //runs the brainfuck source code
}