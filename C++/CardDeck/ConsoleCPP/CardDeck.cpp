// ConsoleCPP.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <map>
#include <set>

using namespace std;

class Deck
{
private:
	set<int> m_Deck;
	map<int,int> m_Count;

public:
	Deck(){};

	void AddCard(int nVal)
	{
		m_Deck.insert(nVal);
		m_Count[nVal]++;
	}

	void RemoveCard(int nVal)
	{
		if (m_Count[nVal] > 0)
		{
			m_Deck.erase(nVal);
			m_Count[nVal]--;
		}
	}

	void ShowHand()
	{
		set<int>::iterator pCard = m_Deck.begin();

		while (pCard != m_Deck.end())
		{
			for (int iCount = 0; iCount < m_Count[*pCard]; iCount++)
			{
				cout << *pCard << endl;
			}
			pCard++;
		}
	}

	int ShowMinVal()
	{
		return *(m_Deck.begin());
	}

	int ShowMaxVal()
	{
		return *(--m_Deck.end());
	}
};

int main()
{
	Deck myDeck;

	myDeck.AddCard(1);
	myDeck.AddCard(3);
	myDeck.AddCard(5);
	myDeck.AddCard(3);
	myDeck.AddCard(1);

	myDeck.ShowHand();
	myDeck.RemoveCard(5);
	myDeck.ShowHand();

	cout << myDeck.ShowMinVal() << endl;
	cout << myDeck.ShowMaxVal() << endl;

	cin.get();
}

