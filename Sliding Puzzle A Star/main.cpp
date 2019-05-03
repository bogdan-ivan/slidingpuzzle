#include <iostream>
#include <queue>
#include <stack>
#include <set>
#include "Puzzle.h"

/*
4
7 2 1 4
0 13 3 9
12 10 5 8
14 6 11 15

4
9 2 3 4
5 14 1 8
6 15 11 0
13 10 7 12
*/

//void main()
//{
//	std::set<std::string> closed;
//	//std::priority_queue<node*> fringe;
//	priority_fringe pfringe;
//	std::stack<int> sstack;
//
//	int marime;
//	int** stare_initiala = nullptr;
//	int** stare_finala = nullptr;
//
//	if (prompt() == 2)
//	{
//		kread(marime);
//		stare_initiala = allocSquareMatrix(marime);
//		stare_finala = allocSquareMatrix(marime);
//		keyboardRead(stare_initiala, marime);
//	}
//	else
//	{
//		file >> marime;
//		stare_initiala = allocSquareMatrix(marime);
//		stare_finala = allocSquareMatrix(marime);
//		fread(stare_initiala, marime);
//	}
//
//	getTarget(stare_finala, marime);
//	std::cout << "STAREA INITIALA" << std::endl;
//	printSquareMatrix(stare_initiala, marime);
//	if (!validare_puzzle(stare_initiala, marime, stare_finala))
//	{
//		std::cout << "STAREA INITIALA NU ESTE VALIDA !!!" << std::endl;
//		return;
//	}
//
//	std::cout << "HASH: " << hash(stare_initiala, marime) << std::endl << std::endl;
//	std::cout << "FITNESS : " << calc_fitness(stare_initiala, stare_finala, marime) << std::endl;
//	std::cout << "TARGET " << std::endl;
//	printSquareMatrix(stare_finala, marime);
//
//	FindSolution(stare_initiala, closed, pfringe, marime, stare_finala, sstack);
//
//	delete stare_finala;
//	delete stare_initiala;
//
//}


void main()
{
	Puzzle myPuzzle;

	myPuzzle.readFromFile("puzzle.txt");
	myPuzzle.current();
	


}

//struct node
//{
//	int a;
//	float fitness;
//};
//
//auto fitnessOrder = [](node* const &a, node* const &b)
//{
//	return a->fitness > b->fitness;
//};
//
//using pq = std::priority_queue<node*, std::vector<node*>, decltype(fitnessOrder)>;
//pq lambdapq(fitnessOrder);
//
//bool operator<(const node first, const node second)
//{
//	return first.fitness > second.fitness;
//}
