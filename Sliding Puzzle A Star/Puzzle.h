#pragma once
#include <chrono>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include <queue>

struct Timer
{
	std::chrono::time_point<std::chrono::steady_clock> start, end;
	std::chrono::duration<float> duration;

	Timer()
	{
		start = std::chrono::high_resolution_clock::now();
	}

	~Timer()
	{
		end = std::chrono::high_resolution_clock::now();
		duration = end - start;

		float s = duration.count();
		float ms = duration.count() * 1000.0f;
		std::cout << "Timer : " << s << " s" << std::endl;
		std::cout << "Timer : " << ms << " ms" << std::endl;
	}
};

#define sus 8
#define jos 2
#define stanga 4
#define dreapta 6

std::fstream file("puzzle.txt");

struct blank
{
	int row;
	int col;
};

struct point
{
	int row;
	int col;
};

struct node
{
	int** stare;
	node* parinte;
	int actiune;
	int cost;
	int adancime;
	blank pozitia_golului;
	int fitness;
};

struct OrderByFitness
{
	bool operator() (node* const &a, node* const &b)
	{
		return a->fitness > b->fitness;
	}
};

typedef std::priority_queue<node*, std::vector<node*>, OrderByFitness> priority_fringe;

struct expandInstructions
{
	int cnumber;
	std::queue<int> actions;
};

class Puzzle
{
public:
	Puzzle();
	Puzzle(const Puzzle& other) = delete;
	Puzzle operator=(const Puzzle& other) = delete;
	Puzzle(Puzzle&& other) = delete;
	Puzzle operator=(Puzzle&& other) = delete;
	~Puzzle();
	bool validare_puzzle(int** stare_initiala, int marime, int** stare_finala);
	bool readFromFile();
	bool readFromKeyboard();
	bool FindSolution(int** stare_initiala, std::set<std::string> &closed, priority_fringe &fringe, int marime, int** stare_finala);
private:
	int m_marime;
	int** m_stare_initiala;
	int** m_stare_finala;
	std::set<std::string> closed;
	priority_fringe pfringe;
	int** allocSquareMatrix(int size);
	void deleteMatrix(int** matrix, int row);
	node* newNode();
	node* newNode(node* parent);
	node* initialNode(int** stare_initiala, int marime, int** stare_finala);
	void getTarget(int** stare_finala, int marime);
	int calc_fitness(int** stare_initiala, int** stare_finala, int marime);
	std::string& hash(int** matrix, int n);
	blank& findBlank(int** matrix, int marime);
	point& getPoint(int** matrix, int marime, int element);
	void printSquareMatrix(int** matrix, int n) const noexcept;
	void printArray(int* matrix, int n) const noexcept;
	int* matrixToArray(int** matrix, int n);
	void insertQ(int arr[], int n, std::queue<int> &actions);
	expandInstructions getInstructions(node* parent, int marime);
	void printSet(std::set<std::string> set);
	bool findSet(std::set<std::string> set, const std::string &hash);
	bool compareTarget(node* node, int** stare_finala, int marime);
	void copyMatrix(int** firstMatrix, int** secondMatrix, int marime);
	int** result(node* parent, int action, int marime);
	node** expand(node* parent, int marime, int** stare_finala, int &cn);
	int insertAllSuccesors(node* parent, int marime, int** stare_finala, priority_fringe &fringe);
	void solution(node* sol);
};

