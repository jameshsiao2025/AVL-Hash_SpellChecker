// WordChecker.cpp
//
// ICS 46 Winter 2020
// Project #4: Set the Controls for the Heart of the Sun
//
// Replace and/or augment the implementations below as needed to meet
// the requirements.

#include "WordChecker.hpp"
//#include <iostream>



WordChecker::WordChecker(const Set<std::string>& words)
    : words{words}
{
}


bool WordChecker::wordExists(const std::string& word) const
{
    bool exist = false;
    exist = words.contains(word);
    return exist;
}

std::vector<std::string> WordChecker::findSuggestions(const std::string& word) const
{

    std::vector<std::string> suggestions;

    //swap adjacent
    for(int i=0; i < word.size()-1; ++i)
    {
        std::string swapped = word;
        char temp = swapped.at(i);
        swapped.at(i) = swapped.at(i+1);
        swapped.at(i+1) = temp;
        if(words.contains(swapped) == true && repeat(suggestions, swapped) == false)
        {
            suggestions.push_back(swapped);
        }
    }

    std::string alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    for(int i=0; i<=word.size(); ++i)
    {
        for(int x=0; x<26; ++x)
        {
            
            std::string inserted = word;
            std::string::iterator it;
            it = inserted.insert(inserted.begin()+i, alphabet.at(x));

            if(words.contains(inserted)==true && repeat(suggestions, inserted)==false)
            {
                suggestions.push_back(inserted);
            }
        }
    }

    for(int i=0; i<word.size(); ++i)
    {
        std::string erased = word;
        erased.erase(erased.begin()+i);
        if(words.contains(erased)==true && repeat(suggestions, erased) == false)
        {
            suggestions.push_back(erased);
        }
    }

    for(int i=0; i<word.size(); ++i)
    {
        for(int x=0; x<26; ++x)
        {
            
            std::string replaced = word;
            std::string temp(1, alphabet.at(x));
            replaced.replace(i,1, temp);


            if(words.contains(replaced)==true && repeat(suggestions, replaced)==false)
            {
                suggestions.push_back(replaced);
            }
        }
    }

    for(int i=1; i<word.size(); ++i)
    {
        std::string split = word;   
        std::string first = split.substr(0,i);
        std::string second = split.substr(i, split.size()-i);
        if(words.contains(first)==true && words.contains(second)==true)
        {
            std::string::iterator it;
            it = split.insert(split.begin()+i, ' ');
            //std::cout << split << std::endl;
            if(repeat(suggestions, split) == false)
            {
                suggestions.push_back(split);
            }    
        }

    }

    return suggestions;
}

bool WordChecker::repeat(std::vector<std::string> &suggestions, std::string word) const
{
        bool exists = false; 
        for(int i=0; i<suggestions.size(); ++i)
        {
            if(word == suggestions[i])
            {
                exists = true;
                return exists;
            }
        }
        return exists;
}

