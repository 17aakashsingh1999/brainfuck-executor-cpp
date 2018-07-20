#include <iostream>
#include <fstream>
#include <vector>
#include <stack>
#include <utility>

using namespace std;

void help()
{
    cout << "Usage:\n\tbf.exe file_name.bf";
}

bool verify(vector<char> source)
{
    //stack<bool> brackets;
    // 0 for [, 1 for ]
    /*for (int i = 0; i < source.size(); i++)
    {
        if (source[i] != '[' || source[i] != ']')
            continue;
        if (source[i] == '[')
            brackets.push(0);
        else if (source[i] == ']')
        {
            if (brackets.top() == 0)
                brackets.pop();
            else
                return 0;
        }
    }
    return 1;*/
    int st = 0;  // st stands for stack
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
    char chars[] = {'<', '>', '+', '-', ',', '.', '[', ']'};
    vector<char>::iterator it,it2;
    //removes comments from source
    for(it=source.begin();it+1 != source.end();it++)
    {
        if(*it == '/' && *(it+1) == '*')
        {
            //search for comment end
            for(it2 = it+2; it2+1 != source.end(); it2++)
            {
                if(*it2 == '*' && *(it2+1) == '/')break;
            }
            if(it2+1 == source.end())
            {
                cout<<"Comment not closed error.\n";
                return;
            }
            source.erase(it,it2+1);
            it--;
        }
    }
    //removes extra chars from source
    for(it=source.begin(); it != source.end(); it++)
    {
        for(i=0;i<8;i++)
        {
            if(*it == chars[i]) break;
        }
        if(i == 8)
        {
            cout<<"erased "<<*it<<endl;
            source.erase(it);
            it--;
        }
    }
}

void execute(vector<char> source)
{
    int ptr = 0;
    stack< pair<int, char> > brackets;
    vector<char> buffer;
    buffer.push_back(0);
    int i = 0;
    while (i < source.size())
    {
        if (source[i] == '>')
        {
            if (ptr == buffer.size() - 1)
            {
                buffer.push_back(0);
            }
            ptr++;
        }
        else if (source[i] == '<')
        {
            if (ptr == 0)
            {
                cout << "Runtime error.\n";
                return;
            }
            ptr--;
        }
        else if (source[i] == '+')
        {
            buffer[ptr]++;
        }
        else if (source[i] == '-')
        {
            buffer[ptr]--;
        }
        else if (source[i] == '.')
        {
            if (buffer[ptr] >= 0)
                cout << buffer[ptr];
        }
        else if (source[i] == ',')
        {
            cout << "Enter a character: ";
            cin >> buffer[ptr];
        }
        else if (source[i] == '[')
        {
            brackets.push(pair<int, char>(i, '['));
        }
        else if (source[i] == ']')
        {
            if (buffer[ptr] == 0)
            {
                brackets.pop();
                i++;
                continue;
            }
            else
            {
                i = brackets.top().first + 1;
                continue;
            }
        }
        i++;
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
    if (!fin)
    {
        cout << "File " << argv[1] << " not found.\n";
        help();
        return 0;
    }
    fin.seekg(0);
    char reader;
    vector<char> source;
    while (fin >> reader)
    {
        source.push_back(reader);
    }
    fin.close();

    if (!verify(source))
    {
        cout << "Syntax Error.\n";
        help();
        return 0;
    }
    filter(source); //filter out comments and other characters

    execute(source);
}