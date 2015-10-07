#include <vector>
#include <set>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <locale>
//#include <io.h>

using namespace std;

vector <string> introductoryWords;
vector <string> unions;
vector <string> marks;
vector <string> suffics;
vector <string> pronouns;
vector <string> marks_end;
vector <string> classes;
vector <string> addUnions;
vector <string> addClasses;

fstream foutput, fd;

const unsigned char code1 = 128;
const unsigned char code2 = 175;
const unsigned char code3 = 224;
const unsigned char code4 = 239;


void initializeIntroductoryWords();
void initializeUnionWords();
void initializePunctuation();
void initializeSuffix();
void initializePronouns();
void initializeMarksEnd();
void initializeClasses();
void initializeAddConjunctions();
void initializeAddClasses();

int iPr = 0, iWord = 1;

string rus(string &strIn)  //Russian language
{
    string strOut = strIn;
    for (string::size_type i = 0; i < strOut.length(); ++i)
    {
        unsigned char code = (unsigned char)(strOut[i]);
        if(code>=code1 && code<=code2 )
            strOut[i]+=64;
        if(code>=code3 && code<=code4)
            strOut[i]+=16;
        if(code==240)
            strOut[i] = 'š';
        if(code==241)
            strOut[i] = 'ž';
    }
    return strOut;
}


int find_mark(string s)
{
    vector <int> v;
    int min = 0,j;

    for (int i = 0; i < marks.size(); i++)
    {
        j = s.find(marks[i][0]);
        v.push_back(j);
    }

    for (int j = 0; j < v.size(); j++)
        if ((v[j] < min | min==0)&&(v[j] != -1))
           min = v[j];

    return min;
}


int find_mark_end(string s)
{
    vector <int> v;
    int min = 0,j;

    for (int i = 0; i < marks_end.size(); i++)
    {
        j = s.find(marks_end[i][0]);
        v.push_back(j);
    }

    for (int j = 0; j < v.size(); j++)
        if ((v[j] < min | min==0)&&(v[j] != -1))
           min = v[j];

    return min;
}

bool find_introductory(string s, bool flag)
{
     string str;
     int intWordsSize = introductoryWords.size();

     while (!(s.substr(0,1).compare(" ")))
     {
        s = s.substr(1,s.size()-1);
     }
     str = s;
     if (flag)
        str[0] = str[0] - 'À' + 'à';
     str = str.substr(0,str.size()-1);

     for (int i = 0; i < intWordsSize; i++)
     {
         if (!introductoryWords[i].compare(str))
            return true;
     }
     return false;;
}

bool verb_suffics(string s)
{
    int sizeSuffics = suffics.size();

    for (int i = 0; i < sizeSuffics; i++)
    {
        if (s==suffics[i])
        return true;
    }
    return false;
}

bool contain_verb(string s, bool flag)
{
    stringstream ss(s);
    bool t = false;

    while (!ss.fail())
    {
        string word,endWord;
        getline(ss,word,' ');

        endWord = "";

        if (word[word.size()-1]==','||word[word.size()-1]==':'||word[word.size()-1]=='.'||word[word.size()-1]==';')
            word = word.substr(0,word.size()-1);
        if (word.size()>=2)
            endWord = word.substr(word.size()-2,2);
        if (endWord=="ñÿ" || endWord=="ñü")
            if (word.size()>=4)
            endWord = word.substr(word.size()-4,2);

        t = verb_suffics(endWord);

        if (t)
        {
            if (flag)
                fd << word << " verb\n";
            return t;
        }
        if ((word.size()>=5))
            if ((endWord = word.substr(word.size()-2,2))=="ñÿ" || (endWord = word.substr(word.size()-2,2))=="ñü")
                endWord = word.substr(word.size()-5,3);
            else
                endWord = word.substr(word.size()-3,3);
        else
            if (word.size()>=3)
                endWord = word.substr(word.size()-3,3);

         t = verb_suffics(endWord);

        if (t)
        {
            if (flag)
                fd << word << " verb\n";
            return t;
        }
    }
    return false;
}

bool isunion(string s, bool flag, bool inout = false)
{
    for (int i = 0; i < unions.size();i++)
    {
        if (!unions[i].compare(s))
        {
            if (flag)
                if (inout)
                    foutput << classes[i];
                else
                    fd << classes[i];
            return true;
        }
    }
    return false;
}

bool analyzeConjuction(string s, bool flag, bool inout = false)
{
    stringstream ss(s);
    bool t = false;
    while (!ss.fail())
    {
        string word;
        getline(ss, word, ' ');

        if (word[word.size() - 1] == ',' || word[word.size() - 1] == '.' || word[word.size() - 1] == ':' || word[word.size()-1] == ';' || word[word.size() - 1] == '!' || word[word.size() - 1] == '?')
            word = word.substr(0, word.size() - 1);

        if (isunion(word, flag, inout))
            return true;
    }
    return false;
}

bool isAddunion(string s, bool flag)
{
    for (int i = 0; i < addUnions.size(); i++)
    {
        if (!addUnions[i].compare(s))
        {
            if (flag)
                fd << addClasses[i];
            return true;
        }
    }
    return false;
}

bool ispronoun(string s)
{
    for (int i = 0; i < pronouns.size();i++)
    {
        if (!pronouns[i].compare(s))
            return true;
    }
    return false;
}

bool contain_pronoun(string s, bool flag)
{
    stringstream ss(s);
    bool t = false;
    while (!ss.fail())
    {
        string word;
        getline(ss,word,' ');

        if (word[word.size()-1]==',' ||word[word.size()-1]=='.'||word[word.size()-1]==':'||word[word.size()-1]==';'||word[word.size()-1]=='!'||word[word.size()-1]=='?')
            word = word.substr(0,word.size()-1);

        if (ispronoun(word))
        {
            if (flag)
                fd << word << " pronoun\n";
            return true;
        }
    }
    return false;
}

void out_discourse(string s, bool isFirst, string s1)
{
    stringstream ss(s);
    string word1,word2 = "";
    int pos;
    if (isFirst)
    {
        fd << "Proposition " << iPr <<"\n";
        iWord = 1;
    }
    //getline(ss,word1,')');
    getline(ss,word1,' ');
    if (word1=="")
              getline(ss,word1,' ');
    fd << iWord++ << " " << word1 <<" start_clause=<"<< s1 <<">" ;
    if (isunion(word1,false))
    {
        fd <<" disc_rel=<";
        isunion(word1,true);
        fd <<">";
        if (s1=="A")
            fd << " disc_from =<" <<iPr-1<<">";
        else
        {
            s1[0]=s1[0]-1;
            fd << " disc_from=<"<< s1 <<">";
        }
    }
    //isAddunion(word1,true);

    word2 = "";
    while (!ss.fail())
    {
        getline(ss,word1,' ');
        if(word1=="")
            break;
        if (word1==word2)
            break;
        fd << "\n";
        fd << iWord++ << " " << word1 <<" ";
        if (isunion(word1,false))
        {
            fd<<"disc_rel=<";
            isunion(word1,true);
            fd<<">";
            if (s1=="A")
                fd << " disc_from =<" <<iPr-1<<">";
            else
            {
                s1[0]=s1[0]-1;
                fd << " disc_from=<"<< s1 <<">";
            }
        }
      //  isAddunion(word1,true);
        word2=word1;
    }
    fd << "end_clause\n";
    fd << "\n";
}

void analyze(vector <string> sentenceFragments, vector <int> sentencePart, bool flag, int ibeg)
{
    bool isFirst = true;
    int size = sentenceFragments.size(),j;
    string str;
    bool t,t1 = false;
    string iClause = "A";

    ++iPr;
    foutput << iPr;
    //Introductory words at the begining
    while (sentencePart[ibeg]==0 & !flag)
    {
        foutput << sentenceFragments[ibeg];
        ++ibeg;
        if (ibeg==size)
            return;
    }

   str = "";
   //++iClause[0];
   while (ibeg < size)
    {
        //clause with verb or pronoun
        if (sentencePart[ibeg]==1)
        {
            j = ibeg+1;
            str = str + sentenceFragments[ibeg];
            while (j < size)
            {
                if (sentencePart[j] == -1)
                {
                    str = str + sentenceFragments[j];
                    j++;
                }
                if (sentencePart[j] == 1 || sentencePart[j] == 2)
                    break;
                if (sentencePart[j] == 0)
                {
                    if (j + 1 == size)
                    {
                        flag = true;
                        break;
                    }
                    if (sentencePart[j+1] == 1)
                    {
                        break;
                        flag=true;
                    }
                    if (sentencePart[j+1] == -1)
                    {
                        str = str + sentenceFragments[j];
                        j = j + 1;
                    }
                }
            }
            out_discourse(str, isFirst, iClause);
            isFirst = false;
            foutput << "[";
            if (analyzeConjuction(str,false,false))
            {
                analyzeConjuction(str,true,true);
                foutput << "(";
                if (iClause == "A")
                    foutput << iPr-1;
                else
                {
                    string str1=iClause;
                    str1[0]= str1[0]-1;
                    foutput <<str1;
                }
                foutput << ")";
            }
            foutput << str<< "]";
            if (flag)
            {
                foutput << sentenceFragments[ibeg+1];
                //fd << sentenceFragments[ibeg+1] << " introductory_word\n";
            }
            ibeg = j;
            str = "";
            ++iClause[0];
        }
        else if (sentencePart[ibeg]==-1 | sentencePart[ibeg]==2)
        {
            str = str+sentenceFragments[ibeg];
            j=ibeg+1;
            if (j==size)
            {
                foutput << "[";
                if (analyzeConjuction(str,false,false))
                {
                    analyzeConjuction(str,true,true);
                    foutput << "(";
                    if (iClause=="A")
                        foutput << iPr-1;
                    else
                    {
                        string str1=iClause;
                        str1[0]= str1[0]-1;
                        foutput << str1;
                    }
                    foutput << ") ";
                }
                foutput << str<< "]";
                break;
            }
            while(sentencePart[j]==-1 | sentencePart[j]==0 | sentencePart[j]==2)
            {
                str = str+sentenceFragments[j];
                ++j;
                if (j==size)
                {
                    t1=true;
                    break;
                }
            }
            if (t1)
            {
                out_discourse(str,isFirst,iClause);
                isFirst = false;
                foutput << "[";
                if (analyzeConjuction(str,false,false))
                {
                    analyzeConjuction(str, true, true);
                    foutput << "(";
                    if (iClause == "A")
                        foutput << iPr - 1;
                    else
                    {
                        string str1=iClause;
                        str1[0]= str1[0]-1;
                        foutput << str1 <<" ";
                    }

                    foutput << ") ";
                }
                foutput << str<< "]";
                break;
            }
            else ibeg = j;
        }
    }
    foutput << "\n";
}



void processString(string &s)
{
    string s1;
    bool flag = true, t;
    int pos = find_mark(s);
    vector <string> sentenceFragments;
    vector <int> sentencePart;

    while (pos)
    {
        s1 = s.substr(0,pos + 1);

        if (find_introductory(s1,flag))
        {
            sentenceFragments.push_back(s1);
            sentencePart.push_back(-1);
        }
        else
        {
            t = contain_verb(s1,false) || contain_pronoun(s1,false);
            sentenceFragments.push_back(s1);

            if (t)
                sentencePart.push_back(1);
            else
                if (analyzeConjuction(s1,false))
                    sentencePart.push_back(2);
                else
                    sentencePart.push_back(-1);
        }

        flag = false;
        s = s.substr(pos + 1, s.size());
        pos = find_mark(s);
    }
    if (find_introductory(s, flag))
    {
        sentenceFragments.push_back(s);
        sentencePart.push_back(-1);
    }
    else
    {
        t = contain_verb(s, false) || contain_pronoun(s, false);
        sentenceFragments.push_back(s);

        if (t)
            sentencePart.push_back(1);
        else
            if (analyzeConjuction(s, false))
                    sentencePart.push_back(2);
            else
                sentencePart.push_back(-1);
    }
    analyze(sentenceFragments, sentencePart, false, 0);
}

void processFile()
{
    fstream f;
    int j;

    f.open("text.txt",ios::in);
    string s1 = "", s2;

    while (!f.fail())
    {
        if (f.fail())
            break;

        f >> s2;
        s1 = s1 + " " + s2;
    }
    for (;;)
    {
        j = find_mark_end(s1);
        if (j == -1)
            break;

        if (j + 1 > 0 & s1.length() == 0)
            break;
        s2 = s1.substr(0, j + 1);
        if (s1.size() - j == 1)
            break;
        if (s1.size() < j + 2)
            break;
        s1 = s1.substr(j + 2, s1.size());
        processString(s2);
    }
    f.close();
}


void initializeIntroductoryWords()
{
    fstream f1;
    string s;
    int j;

    f1.open("introductory_words.txt",ios::in);

    while (!f1.eof())
    {
        if (f1.eof())
            break;

        f1 >> s;
        do
        {
            j = s.find(',');
            if (j != -1)
                s[j]=' ';
        }
        while (j != -1);

        if (s.length() == 0)
            break;
        introductoryWords.push_back(s);
    }
}

void initializeUnionWords()
{
    fstream fs;
    string s;
    int j;

    fs.open("unions.txt",ios::in);
    while(!fs.fail())
    {
        fs >> s;
        do
        {
            j = s.find(',');
            if (j != -1)
                s[j] = ' ';
        }
        while (j != -1);
        unions.push_back(s);
    }
}

void initializeClasses()
{
    fstream fs;
    string s;

    fs.open("classes.txt",ios::in);
    while(!fs.fail())
    {
        fs >> s;
        classes.push_back(s);
    }
}

void initializePunctuation()
{
    marks.push_back(",");
    marks.push_back(";");
    marks.push_back(":");
    marks.push_back("(");
    marks.push_back(")");
}

void initializeSuffix()
{
    fstream fs;
    string s;

    fs.open("suffics.txt",ios::in);
    while(!fs.fail())
    {
        fs >> s;
        suffics.push_back(s);
    }
}

void initializePronouns()
{
    fstream fs;
    string s;

    fs.open("pronouns.txt",ios::in);
    while(!fs.fail())
    {
        fs >> s;
        pronouns.push_back(s);
    }
}

void initializeMarksEnd()
{
    marks_end.push_back(".");
    marks_end.push_back("?");
    marks_end.push_back("!");
}

void initializeAddConjunctions()
{
    fstream fs;
    string s;

    fs.open("addUnions.txt",ios::in);
    while(!fs.fail())
    {
        fs >> s;
        addUnions.push_back(s);
    }
}

void initializeAddClasses()
{
    fstream fs;
    string s;

    fs.open("addClasses.txt",ios::in);
    while(!fs.fail())
    {
        fs >> s;
        addClasses.push_back(s);
    }
}

int main(void)
{
    initializeUnionWords();
    initializePunctuation();
    initializeIntroductoryWords();
    initializeSuffix();
    initializePronouns();
    initializeMarksEnd();
    initializeClasses();
    initializeAddConjunctions();
    initializeAddClasses();
    foutput.open("output1.txt", ios::out);
    fd.open("discourseMarkers.txt", ios::out);
    processFile();
    fd.close();
    foutput.close();
    return 0;
}
