#include <algorithm>
#include <regex>
#include <vector>
#include <stdlib.h>
#include <iostream>

using namespace std;


/*
 *  -------------------------------------
 *  |     |  1  2  3  4  5  6  7  8  9  |
 *  -------------------------------------
 *  |tiao:|  1  2  3  4  5  6  7  8  9  |
 *  |tong:|  A  B  C  D  E  F  G  H  I  |
 *  |wan :|  a  b  c  d  e  f  g  h  i  |
 *  -------------------------------------
 *
 */


string regex_pong = "(([1-9]|[a-i]|[A-I])\\1{1,2})";
string regex_gang = "(([1-9]|[a-i]|[A-I])\\1{2})";
string regex_chi  = "((?:1(?=2)|2(?=3)|3(?=4)|4(?=5)|5(?=6)|6(?=7)|7(?=8)|8(?=9)){2}\\d)|"
                    "((?:a(?=b)|b(?=c)|c(?=d)|d(?=e)|e(?=f)|f(?=g)|g(?=h)|h(?=i)){2}\\d)|"
                    "((?:A(?=B)|B(?=C)|C(?=D)|D(?=E)|E(?=F)|F(?=G)|G(?=H)|H(?=I)){2}\\d)";

void GetPongCards(const string& inputCards)
{
    regex re(regex_pong);
    smatch result;
    if (regex_search(inputCards, result, re))
    {
        cout << result.str(1) << '\n';
    }
    else
    {
        cout << "No matches"<< '\n';
    }
}

void GetGangCards(const string& inputCards)
{
    regex re(regex_gang);
    smatch result;
    if (regex_search(inputCards, result, re))
    {
        cout << result.str(1) << '\n';
    }
    else
    {
        cout << "No matches" << '\n';
    }

}

void GetChiCards(const string& inputCards)
{
    regex re(regex_chi);
    smatch result;
    if (regex_search(inputCards, result, re))
    {
        cout << result.str(1) << '\n';

    }
    else
    {
        cout << "No matches" << '\n';

    }


}




int main(int argc, char* argv[])
{
    int inputCardsCount;


    string inputCards;
    while(cin >> inputCardsCount)
    {

        inputCards.resize(inputCardsCount);
        for (int i = 0; i < inputCardsCount; i++)
        {
            cin >> inputCards[i];
        }

        sort(inputCards.begin(), inputCards.end());

        GetChiCards(inputCards);

    }

    return 0;

}
