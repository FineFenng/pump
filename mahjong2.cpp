//
// Created by finefenng on 2018/7/16.
//

#include <algorithm>
#include <array>
#include <iostream>
#include <string>
#include <vector>
#include <map>

using namespace std;

string tiao = "123456789";
string wan  = "abcdefghi";
string tong = "ABCDEFGHI";

map<char, int> CardTree;
bool isHu = false;

bool IsCard(const char& card)
{
  if(tiao.find(card) != string::npos || wan.find(card) != string::npos || tong.find(card) != string::npos)
  {
    return true;
  }
  return false;
}


bool IsPong(const char& card)
{
    int& value = CardTree[card];
    if (value >= 2 && value < 4)
    {
        return true;
    }
    return false;
}


bool IsGang(const char& card)
{
    int& value = CardTree[card];
    if (value >= 3 && value < 4)
    {
        return true;
    }
    return false;

}

bool  IsChi(const char& card)
{
    char first_card, second_card, third_card;

    // first card
    first_card = card;
    second_card = static_cast<char>(first_card + 1);
    third_card  = static_cast<char>(second_card + 1);

    if (IsCard(second_card) && IsCard(third_card))
    {
        if (CardTree[third_card] > 0 && CardTree[second_card] > 0)
        {
            return true;
        }
    }

    // second card
    second_card = card;
    first_card = static_cast<char>(second_card - 1);
    third_card = static_cast<char>(second_card - 1);

    if (IsCard(first_card) && IsCard(third_card))
    {
        if( CardTree[first_card] > 0 && CardTree[third_card] > 0)
        {
            return true;
        }
    }

    // third card
    third_card = card;
    second_card = static_cast<char>(third_card - 1);
    first_card = static_cast<char>(third_card - 2);

    if (IsCard(second_card) && IsCard(first_card))
    {
        if (CardTree[second_card] > 0 && CardTree[first_card] > 0)
        {
            return true;
        }
    }
    return  false;
}

void CheckOut()
{
    if (all_of(CardTree.begin(), CardTree.end(),[] (pair<char, int> it) {return it.second > 0;}))
    {
        isHu = true;
        return;
    }


    char key = '\0';

    for (auto it = CardTree.begin(); it != CardTree.end(); it++)
    {
        if (it->second > 0)
        {
            key = it->first;
            break;
        }
    }

    if ((CardTree[key+1] > 0 && CardTree[key+2] > 0 && key <= '7') ||
        (CardTree[key+1] > 0 && CardTree[key+2] > 0 && key <= 'g') ||
        (CardTree[key+1] > 0 && CardTree[key+2] > 0 && key <= 'G'))
    {
        --CardTree[key];
        --CardTree[key+1];
        --CardTree[key+2];
        CheckOut();
        ++CardTree[key];
        ++CardTree[key+1];
        ++CardTree[key+2];
        if (isHu) return;
    }

    if (CardTree[key] >= 3)
    {
       CardTree[key] -= 3;
       CheckOut();
       CardTree[key] += 3;
       if (isHu) return;
    }

}




int main()
{
    for_each(tiao.begin(), tiao.end(), [](char it) {CardTree[it] = 0 ;});
    for_each(wan.begin(), tiao.end(), [](char it) {CardTree[it] = 0 ;});
    for_each(tong.begin(), tiao.end(), [](char it) {CardTree[it] = 0 ;});


    int inputCardsCount;
    char key;

    while(cin >> inputCardsCount)
    {

        for (int i = 0; i < inputCardsCount; i++)
        {
            cin >> key;
            ++CardTree[key];
        }
    }

    :q




}











