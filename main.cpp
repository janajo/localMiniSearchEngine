
//  main.cpp
//
//  Created by Jana Fortney on 3/9/15.
//  Copyright (c) 2015 Jana Fortney. All rights reserved.
//


#include<iostream>
#include<cstdio>
#include<algorithm>
#include<vector>
#include<string>
#include<cmath>
#include<iterator>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <ctime>      /* clock_t, clock, CLOCKS_PER_SEC */

using namespace std;

int i = 1;

class WordRec  // word rec (each node of a linked list)
{
public:
    
    string wordS; //word string
    int wordC; ///word count
    string document; /// string storing name of document
    WordRec *next;
    
    
    
    
    
    WordRec()
    {
        next = NULL;
    }
    
    
    WordRec(string wordstring, int count, string doc)
    {
        wordS = wordstring;
        document = doc;
        wordC = count;
        next = NULL;
    }
    string toString()
    {
        stringstream ss;
        ss << wordS;
        ss << " " << wordC << "\n";
        return ss.str();
    }
};

class WordTab ///word table. a hash table of linked lists
{
private:
    WordRec* entries;
    int N = 9001;//size of table
    
    
    int hash(string s)
    {
        int hash = -1;
        for (int i = 0; i < s.length(); i++)
        {
            hash = (hash+ s[i]) ;
        }
        hash = hash % N;
        return hash;
    }
    
    
public:
    
    WordTab(int n) // constuctor
    {
        N = n;
        entries = new WordRec[N];
        
        for (int i = 0; i<N; i++)
        {
            entries[i].next = NULL;
            entries[i].wordS = "";
            entries[i].wordC = -1;
            entries[i].document = "";
        }
    }
    
    ~WordTab() ///deleter
    {
        for (int i = 0; i<N; i++)
        {
            if (entries[i].next != NULL)
            {
                WordRec *temp = entries[i].next, *old = &entries[i];
                while (temp->next != NULL)
                {
                    old = temp;
                    temp = temp->next;
                    delete old;
                }
                delete temp;
            }
        }
        delete[] entries;
    }
    
    
    
    
    void add(string s, string doc) // add a word to the table
    {
        
        int h = hash(s);// getting the hash value of the string 0-16383
        
        if (entries[h].wordC != -1) ///something already exists in the spot
        {
            
            WordRec *temp = &entries[h];
            
            while (temp->next != NULL /*&& temp->wordS == s*/)
            {
                if (temp->document == doc && temp->wordS == s)
                {
                    temp->wordC++;
                    break;
                }
                temp = temp->next;
            }
            
            if (temp->wordS == s && temp->document==doc) //accounts for last node in a linked list
            {
                temp->wordC++;
                
            }
            else
            {
                
                WordRec* old = temp;
                temp = new WordRec;
                temp->next = NULL;
                temp->wordS = s;
                temp->wordC = 1;
                temp->document = doc;
                old->next = temp;
                
               
            }
        }
        else
        {
            entries[h].wordC = 1;
            entries[h].wordS = s;
            entries[h].document = doc;
            
        }
        
      
        
        
    };
    
    
    
    string print()
    {
        stringstream ss;
        WordRec * temp;
        for (int i = 0; i <N; i++)
        {
            temp = &entries[i];
            if (entries[i].wordC != -1)
            {
                while (temp->next != NULL)
                {
                    ss << temp->toString();
                    temp = temp->next;
                }
                ss << temp->toString();
            }
        }
        
        return ss.str();
    }
    
    void finder(string wordInq, vector<string> &wordVec,vector<int> &countVec, vector<string> &docVec)
    {
        WordRec * temp;
        
        int indexNum= hash(wordInq);
        
        temp = &entries[indexNum];
        
        
        while(temp->next != NULL)
        {
            if(wordInq==temp->wordS)
            {
                wordVec.push_back(temp->wordS);
                countVec.push_back(temp->wordC);
                docVec.push_back(temp->document);
            }
            
            temp=temp->next;
            
        }
        if(wordInq==temp->wordS) // accounts for last  node since the while breaks
        {
            wordVec.push_back(temp->wordS);
            countVec.push_back(temp->wordC);
            docVec.push_back(temp->document);
        }
        
        
        
    }
};





void Bubble(vector<string> &minusOneVec, vector<int> &minusOneCount, int size)
{
    
    int i, j;
    bool flag = true;    // set flag to 1 to start first pass
    int temp;             // holding variable
    string tempString;
    for(i = 1; i <= size && flag== true; i++)
    {
        flag = false;
        for (j=0; j < (size-1); j++)
        {
            if (minusOneCount[j+1] > minusOneCount[j])      // ascending order simply changes to <
            {
                temp = minusOneCount[j];             // swap elements
                minusOneCount[j] = minusOneCount[j+1];
                minusOneCount[j+1] = temp;
                flag = true;// indicates that a swap occurred.
                
                tempString = minusOneVec[j];             // swap elements
                minusOneVec[j] = minusOneVec[j+1];
                minusOneVec[j+1] = tempString;
                
                
                
            }
            
        }
        
    }
    
    return;   //arrays are passed to functions by address; nothing is returned
}


void WordReader(WordTab &object, string document)
{
    
    ifstream inFile(document);
    
    

    
    if (!inFile)
    {
        cout << "Could not open file!" << endl;
        
    }
    
    while (!inFile.eof())
    {
        
        
        string lineString;
        getline(inFile, lineString);
        
        
     
       
        for(int i=0; i<lineString.length(); i++)
        {
            lineString[i]=tolower(lineString[i]);
        
        }
        
        string subLine; //sub string of inquires
        vector<string> lineStringVec;
        
        for( int i=0; i<lineString.length(); i++)
        {
            if (lineString[0] != '<')
            {
                if (lineString[i] != ' '  && lineString[i] != ')' && lineString[i] != '(' &&  lineString[i] != ','&& lineString[i]!='-' )
                {
                    subLine.push_back(lineString[i]);
                }
                if (lineString[i] == ' ' || i == lineString.length()-1 ||  lineString[i] == ')' || lineString[i] == '(' || lineString[i] == ',' || lineString[i]=='-' )
                {
                    string subLineWord = subLine;
                    lineStringVec.push_back(subLineWord);
                    subLine.clear();
                }
            }
        }

      
        for(int j=0; j<lineStringVec.size(); j++)
        {
      if (lineStringVec[j] != "a" &&  lineStringVec[j] != "all" && lineStringVec[j] != "an"  && lineStringVec[j] != "and" && lineStringVec[j] != "any" &&  lineStringVec[j] != "are" && lineStringVec[j] != "as" && lineStringVec[j] != "be" && lineStringVec[j] != "been" && lineStringVec[j] != "but" && lineStringVec[j] != "by" && lineStringVec[j] != "few" && lineStringVec[j] != "for" && lineStringVec[j] != "have" && lineStringVec[j] != "he" && lineStringVec[j] != "her" && lineStringVec[j] != "here" && lineStringVec[j] != "him" && lineStringVec[j] != "his" && lineStringVec[j] != "how" && lineStringVec[j] != "i" && lineStringVec[j] != "in" && lineStringVec[j] != "is" && lineStringVec[j] != "it" && lineStringVec[j] != "its" && lineStringVec[j] != "many" && lineStringVec[j] != "me" && lineStringVec[j] != "my" && lineStringVec[j] != "none" && lineStringVec[j] != "of" && lineStringVec[j] != "on" && lineStringVec[j] != "or" && lineStringVec[j] != "our" && lineStringVec[j] != "she" && lineStringVec[j] != "some" && lineStringVec[j] != "the" && lineStringVec[j] != "their" && lineStringVec[j] != "them" && lineStringVec[j] != "there" && lineStringVec[j] != "they" && lineStringVec[j] != "that" && lineStringVec[j] != "this" && lineStringVec[j] != "us" && lineStringVec[j] != "was" && lineStringVec[j] != "what" && lineStringVec[j] != "when" && lineStringVec[j] != "where" && lineStringVec[j] != "which" && lineStringVec[j] != "who" && lineStringVec[j] != "why" && lineStringVec[j] != "will" && lineStringVec[j] != "with" && lineStringVec[j] != "you"  && lineStringVec[j] != "your" && lineStringVec[j] !="." && lineStringVec[j]!="") // if lineString[j] is not equal to all of these, add it
            {
                //cout<< lineStringVec[j]<<endl;
                object.add(lineStringVec[j], document);
            }
        
        }//end of for vector loop
        
    
    }// end of while
     inFile.close();
}

void DocReader(WordTab &object)
{
    
    string cranfield1 = "cranfield000";
    string cranfield2 = "cranfield00";
    string doc = "";
    string input="";
    
    
    for(int i =1; i<10; i++)
    {
        // cout << "Reading next doc:" <<i<< endl;
        cranfield1 += to_string(i);
        doc = ("/Users/jana/Desktop/cranfieldfolder/"+cranfield1+".txt");
        WordReader(object, doc);
        cranfield1 = "cranfield000"; //reset cranfield
        
    }
    for (int i = 10; i<=50; i++)
    {
        //  cout << "Reading next doc:" << i << endl;
        cranfield2 += to_string(i);
        doc = ("/Users/jana/Desktop/cranfieldfolder/"+cranfield2+".txt");
        WordReader(object, doc);
        //reset cranfield
        cranfield2 = "cranfield00";
    }
    
}


int main()
{
    
    int n = 9001; ////prime
    WordTab myHasher(n);
    DocReader(myHasher);
   
    
    string cranfield1 = "cranfield0001";
    string cranfield2 = "cranfield00";
    string doc = "";
    
    vector<string> wordVec;
    vector<int> countVec;
    vector<string> docVec;
    vector<string>inquiresVec;
    
    
    string inquires;
    string continueUser = "yes";
    
    int ignoreCounter=0;
    
    while( continueUser == "yes")
    {
        
        inquiresVec.clear();
        wordVec.clear();
        countVec.clear();
        docVec.clear();
        
        
        
        
        cout<<"Input terms you would like to search for"<<endl;
        
        if(ignoreCounter!=0)
        {
            cin.ignore(); //allows us to reuse getline and ignore the whitespace after the first "enter"
        }
        getline(cin, inquires);
        ignoreCounter++;
        
        
        for( int i=0; i<inquires.length(); i++) //makes all lower
        {
            inquires[i]=tolower(inquires[i]);
        }
        string subInq; //sub string of inquires
        cout<<inquires<<endl;
        for( int i=0; i<inquires.length(); i++)
        {
            if (inquires[i] != ' ')
            {
                subInq.push_back(inquires[i]);
            }
            if (inquires[i] == ' ' || i == inquires.length()-1 )
            {
                inquiresVec.push_back(subInq);
                subInq.clear();
            }
            
        }
        
        
        for(int i=0; i<inquiresVec.size(); i++)
        {
            myHasher.finder(inquiresVec[i], wordVec, countVec, docVec);
        }
        

        
        vector<string> minusOneVec;
        vector<string> plusOneVec;
        vector<int> minusOneCount;
        vector<int> plusOneCount;
        
        minusOneVec.clear();
        plusOneVec.clear();
        minusOneVec.clear();
        plusOneCount.clear();
        
        
        for(int i=0; i<inquiresVec.size(); i++)
        {
            bool filled=false;
            
            if (inquiresVec[i] == "and")
            {
                // fill minusOneVec w/ docs that contain the word that is in inquiresVec[i-1]
                // fill plusOneVec w/ docs that contain the word that is in inquiresVec[i+1]
                //compare minusOneVec and plusOnceVec and deletes items not in both
                //change inquires[i-1] to "inquires[i-1] AND inquires[i+1]"
                // delete inquires[i] and inquires[i+1]
                
                
                for(int j=0; j<wordVec.size(); j++)
                {
                    if(wordVec[j] == inquiresVec[i-1])
                    {
                        minusOneVec.push_back(docVec[j]);
                        minusOneCount.push_back(countVec[j]);
                        filled=true;
                    }
                    
                }
                
                
                for(int j=0; j<wordVec.size(); j++)
                {
                    if(wordVec[j] == inquiresVec[i+1])
                    {
                        plusOneVec.push_back(docVec[j]);
                        plusOneCount.push_back(countVec[j]);
                        filled=true;
                    }
                    
                }
                
                
                bool found=false;
                for(int j=0; j<minusOneVec.size(); j++)
                {
                    for( int k=0; k<plusOneVec.size(); k++)
                    {
                        if(minusOneVec[j] == plusOneVec[k])
                        {
                            found=true;
                            minusOneCount[j]=minusOneCount[j]+plusOneCount[k];
                            
                        }
                    }
                    
                    if(found==false)
                    {
                        minusOneVec.erase(minusOneVec.begin()+j);
                        minusOneCount.erase(minusOneCount.begin()+j);
                        j= j-1; //accounts for adding j when the element has just been erased and moved up
                    }
                    found=false;//reset found
                }//end of for loop (comparing the two arrays
                
                string newWord=inquiresVec[i-1]+" and "+inquiresVec[i+1];
                
                for(int g=0; g<minusOneVec.size(); g++)
                {
                    docVec.push_back(minusOneVec[g]);
                    countVec.push_back(minusOneCount[g]);
                    wordVec.push_back(newWord);
                }
                
                
                inquiresVec[i-1]=newWord;
                for(int q=i; q<inquiresVec.size()-2; q++)
                {
                    inquiresVec[q]=inquiresVec[q+2];
                    
                }
                inquiresVec.pop_back();
                inquiresVec.pop_back();
                i--;
                
            }/// end of dealing with "AND"
            
            else if (inquiresVec[i]=="or")
            {
                for(int j=0; j<wordVec.size(); j++)
                {
                    if(wordVec[j] == inquiresVec[i-1])
                    {
                        minusOneVec.push_back(docVec[j]);
                        minusOneCount.push_back(countVec[j]);
                        filled=true;
                        
                    }
                    
                }
                
                for(int j=0; j<wordVec.size(); j++)
                {
                    if(wordVec[j] == inquiresVec[i+1])
                    {
                        plusOneVec.push_back(docVec[j]);
                        plusOneCount.push_back(countVec[j]);
                        filled=true;
                        
                    }
                    
                }
                
                bool found=false;
                for(int j=0; j<plusOneVec.size(); j++)
                {
                    for( int k=0; k<minusOneVec.size(); k++)
                    {
                        if(minusOneVec[k] == plusOneVec[j])
                        {
                            found=true;
                            minusOneCount[k]=minusOneCount[k]+plusOneCount[j];
                        }
                    }
                    
                    if(found==false)
                    {
                        minusOneVec.push_back(plusOneVec[j]);
                        minusOneCount.push_back(plusOneCount[j]);
                        
                    }
                    found=false;
                    
                }//end of for loop (comparing the two arrays
                
                string newWord=inquiresVec[i-1]+" or "+inquiresVec[i+1];
                
                for(int g=0; g<minusOneVec.size(); g++)
                {
                    docVec.push_back(minusOneVec[g]);
                    countVec.push_back(minusOneCount[g]);
                    wordVec.push_back(newWord);
                }
                
                
                inquiresVec[i-1]=newWord;
                for(int q=i; q<inquiresVec.size()-2; q++)
                {
                    inquiresVec[q]=inquiresVec[q+2];
                    
                }
                inquiresVec.pop_back();
                inquiresVec.pop_back();
                i--;
            }//end of OR loop
            
            else
            {
                if (i != inquiresVec.size()-1) //accounts for last word
                {
                    if(inquiresVec[i+1] != "and" || inquiresVec[i+1] != "or")
                    { // saves time since this list/word woudn't get printed anyways
                        for(int j=0; j<wordVec.size(); j++)
                        {
                            if(wordVec[j] == inquiresVec[i])
                            {
                                minusOneVec.push_back(docVec[j]);
                                minusOneCount.push_back(countVec[j]);
                                filled=true;
                            }
                        }
                    }
                }
                
                else
                {
                    for(int j=0; j<wordVec.size(); j++)
                    {
                        if(wordVec[j] == inquiresVec[i])
                        {
                            minusOneVec.push_back(docVec[j]);
                            minusOneCount.push_back(countVec[j]);
                            filled=true;
                            
                        }
                        
                        
                    }
                }
                
            }
            
            
            
            if (i != inquiresVec.size()-1  && inquiresVec[i] !="") //accounts for last word
            {
                if(inquiresVec[i+1] != "and" && inquiresVec[i+1] != "or")
                    //makes sure that we don't print something to only reprint those docs again when we process the AND or OR
                {
                    
                    Bubble( minusOneVec, minusOneCount, minusOneCount.size() );
                    
                    
                    if (minusOneCount.size()!=0)
                    {
                        cout<<"The word(s) '"<<inquiresVec[i]<<"' appear(s) in "<<endl;
                        
                        for(int l=0; l<minusOneVec.size(); l++)
                        {
                            cout<<minusOneVec[l]<<" with frequency of "<<minusOneCount[l]<<endl;
                        }
                        
                        
                    }
                    else
                    {
                        cout<<"The word or combination of words '"<<inquiresVec[i]<<"' does not appear in any documents"<<endl;
                    }
                    
                }
            }
            else
            {
                if (inquiresVec[i] != "")
                {
                    Bubble(minusOneVec,minusOneCount,minusOneCount.size() );
                    
                    if (minusOneCount.size() !=0)
                    {
                        cout<<"The word(s) "<<inquiresVec[i]<<" appear(s) in "<<endl;
                        
                        for(int l=0; l<minusOneVec.size(); l++)
                        {
                            cout<<minusOneVec[l]<<endl;
                        }
                        
                        
                    }
                    else
                    {
                        cout<<"The word or combination of words '"<<inquiresVec[i]<<"' does not appear in any documents"<<endl;
                    }
                    
                }
            }
            
            
            
            minusOneVec.clear();
            plusOneVec.clear();
            minusOneCount.clear();
            plusOneCount.clear();
            
        }
        cout<<"Would you like to search again? If so, enter 'yes'. Other wise press any button"<<endl;
        cin>>continueUser;
        
    }
    return 0;
    
};

