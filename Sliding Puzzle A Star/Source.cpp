#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <vector>
#include <Windows.h>
#include <set>
#include <queue>
#include <stack>
#include <unordered_map>
#include <chrono>


//4
//7 2 1 4
//0 13 3 9
//12 10 5 8
//14 6 11 15

/*
4
9 2 3 4
5 14 1 8
6 15 11 0
13 10 7 12
*/

#define sus 8
#define jos 2
#define stanga 4
#define dreapta 6


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


std::fstream file("puzzle.txt");

struct blank
{
	int row;
	int col;
};

int calc_fitness(int** stare_initiala, int** stare_finala, int marime);

blank findBlank(int** matrix, int marime);

int** allocSquareMatrix(int size)
{

	int** matrix = new int*[size];

	for (int i = 0; i < size; ++i)
	{
		matrix[i] = new int[size];
	}
	return matrix;
}

void deleteMatrix(int** matrix, int row)
{
	for (int i = 0; i < row; ++i)
	{
		delete[] matrix[i];
	}
	delete[] matrix;
}

int* matrixToArray(int** matrix, int n)
{
	int arraysize = n*n;
	int* array = new int[arraysize];
	int index = 0;
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			array[index] = matrix[i][j];
			index++;
		}
	}
	return array;
}

std::string hash(int** matrix, int n)
{
	std::string hash;
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			hash += std::to_string(matrix[i][j]);
		}
	}
	return hash;
}

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

node* newNode()
{
	node* node = new struct node;
	return node;
}

node* newNode(node* parent)
{
	node* node = new struct node;
	node->parinte = parent;
	return node;
}

node* initialNode(int** stare_initiala, int marime, int** stare_finala)
{
	node* node = new struct node;
	node->stare = stare_initiala;
	node->parinte = nullptr;
	node->actiune = 5;
	node->cost = 0;
	node->adancime = 0;
	node->pozitia_golului = findBlank(node->stare, marime);
	node->fitness = calc_fitness(stare_initiala, stare_finala, marime) + node->adancime;
	return node;
}

blank findBlank(int** matrix, int marime)
{
	blank blank;
	for (int i = 0; i < marime; i++)
	{
		for (int j = 0; j < marime; j++)
		{
			if (matrix[i][j] == 0)
			{
				blank.row = i;
				blank.col = j;
			}
		}
	}
	return blank;
}

bool validare_puzzle(int** stare_initiala, int marime, int** stare_finala)
{
	std::set<int> validating;
	blank blank = findBlank(stare_initiala, marime);
	int arraysize = marime*marime;
	int* marray = matrixToArray(stare_initiala, marime);
	int inversiuni = 0;
	int width = sqrt(arraysize);
	int corr = 0;

	for (int i = 0; i < marime; i++)
	{
		for (int j = 0; j < marime; j++)
		{
			if (stare_initiala[i][j] == stare_finala[i][j])
			{
				corr++;
			}
		}
	}

	if (corr == marime*marime)
	{
		std::cout << "STAREA INITIALA ESTE STAREA FINALA !!!" << std::endl;
		return true;
	}

	for (int i = 0; i < marime; i++)
	{
		for (int j = 0; j < marime; j++)
		{
			validating.insert(stare_initiala[i][j]);
		}
	}

	if (validating.size() != marime*marime)
	{
		std::cout << "NUMERELE DIN PUZZLE SE REPETA !!!" << std::endl;
		return false;
	}


	for (int i = 0; i < arraysize; i++)
	{
		for (int j = i; j < arraysize; j++)
		{
			if (marray[i] > marray[j] && marray[j] != 0)
			{
				inversiuni++;
			}
		}
	}

	std::cout << "NUMAR INVERSIUNI: " << inversiuni << std::endl;
	std::cout << "ROW: " << blank.row + 1 << std::endl;
	std::cout << "COL: " << blank.col + 1 << std::endl;

	if (((width % 2 == 1) && (inversiuni % 2 == 0)) || ((width % 2 == 0) && ((blank.row % 2 == 1) == (inversiuni % 2 == 0))))
	{
		std::cout << "STATUS : VALID" << std::endl;
		return true;
	}

	std::cout << "STATUS : INVALID" << std::endl;
	std::cout << "STAREA INITIALA ESTE CORUPTA" << std::endl;
	return false;
}

void fread(int** matrix, int marime)
{
	for (int i = 0; i < marime; i++)
	{
		for (int j = 0; j < marime; j++)
		{
			file >> matrix[i][j];
		}
	}
}

void printSquareMatrix(int** matrix, int n)
{
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			std::cout << matrix[i][j] << " ";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

void printArray(int* matrix, int n)
{
	for (int i = 0; i < n; i++)
	{
		std::cout << matrix[i] << " ";
	}
	std::cout << std::endl;
}

void getTarget(int** stare_finala, int marime)
{
	int index = 1;
	for (int i = 0; i < marime; i++)
	{
		for (int j = 0; j < marime; j++)
		{
			stare_finala[i][j] = index;
			index++;
		}
	}
	stare_finala[marime - 1][marime - 1] = 0;
}

point getPoint(int** matrix, int marime, int element)
{
	point coordonate;
	for (int i = 0; i < marime; i++)
	{
		for (int j = 0; j < marime; j++)
		{
			if (matrix[i][j] == element)
			{
				coordonate.row = i;
				coordonate.col = j;
			}
		}
	}
	return coordonate;
}

int calc_fitness(int** stare_initiala, int** stare_finala, int marime)
{
	int sumaDistantelor = 0;
	for (int i = 0; i < marime; i++)
	{
		for (int j = 0; j < marime; j++)
		{
			point trgt = getPoint(stare_finala, marime, stare_initiala[i][j]);
			sumaDistantelor += abs(i - trgt.row) + abs(j - trgt.col);
		}
	}
	return sumaDistantelor;
}

struct OrderByFitness
{
	bool operator() (node* const &a, node* const &b)
	{
		return a->fitness > b->fitness;
	}
};

typedef std::priority_queue<node*, std::vector<node*>, OrderByFitness> priority_fringe;

/*
bool operator<(const node first, const node second)
{
return first.fitness > second.fitness;
}
*/

void printSet(std::set<std::string> set)
{
	std::set<std::string>::iterator itr;
	std::cout << "\nThe set closed is : ";
	for (itr = set.begin(); itr != set.end(); itr++)
	{
		std::cout << *itr << " ";
	}
	std::cout << std::endl;
}

bool findSet(std::set<std::string> set, const std::string &hash)
{
	return set.find(hash) != set.end();
}

bool compareTarget(node* node, int** stare_finala, int marime)
{
	int** matrix = node->stare;
	for (int i = 0; i < marime; i++)
	{
		for (int j = 0; j < marime; j++)
		{
			if (matrix[i][j] != stare_finala[i][j])
			{
				return false;
			}
		}
	}
	return true;
}

struct expandInstructions
{
	int cnumber;
	std::queue<int> actions;
};

void insertQ(int arr[], int n, std::queue<int> &actions)
{
	for (int i = 0; i < n; i++)
	{
		actions.push(arr[i]);
	}
}

expandInstructions getInstructions(node* parent, int marime)
{
	blank blank = parent->pozitia_golului;
	expandInstructions instructions;

	if ((blank.row != 0) && (blank.row != marime - 1) && (blank.col != 0) && (blank.col != marime - 1))
	{
		instructions.cnumber = 4;
		int arr[4] = { sus,jos,stanga,dreapta };
		insertQ(arr, 4, instructions.actions);
	}
	if ((blank.row == 0 && blank.col != 0) && (blank.row == 0 && blank.col != marime - 1) || (blank.row == marime - 1 && blank.col != 0) && (blank.row == marime - 1 && blank.col != marime - 1))
	{
		instructions.cnumber = 3;
		if (blank.row == 0)
		{
			int arr[3] = { jos,stanga,dreapta };
			insertQ(arr, 3, instructions.actions);
		}
		if (blank.row == marime - 1)
		{
			int arr[3] = { sus,stanga,dreapta };
			insertQ(arr, 3, instructions.actions);
		}
	}
	if ((blank.col == 0 && blank.row != 0) && (blank.col == 0 && blank.row != marime - 1) || (blank.col == marime - 1 && blank.row != 0) && (blank.col == marime - 1 && blank.row != marime - 1))
	{
		instructions.cnumber = 3;
		if (blank.col == 0)
		{
			int arr[3] = { sus,jos,dreapta };
			insertQ(arr, 3, instructions.actions);
		}
		if (blank.col == marime - 1)
		{
			int arr[3] = { sus,jos,stanga };
			insertQ(arr, 3, instructions.actions);
		}
	}
	if ((blank.row == 0 && blank.col == 0) || (blank.row == 0 && blank.col == marime - 1) || (blank.row == marime - 1 && blank.col == 0) || (blank.row == marime - 1 && blank.col == marime - 1))
	{
		instructions.cnumber = 2;
		if (blank.row == 0 && blank.col == 0)
		{
			int arr[2] = { dreapta,jos };
			insertQ(arr, 2, instructions.actions);
		}
		if (blank.row == 0 && blank.col == marime - 1)
		{
			int arr[2] = { stanga,jos };
			insertQ(arr, 2, instructions.actions);
		}
		if (blank.row == marime - 1 && blank.col == 0)
		{
			int arr[2] = { sus,dreapta };
			insertQ(arr, 2, instructions.actions);
		}
		if (blank.row == marime - 1 && blank.col == marime - 1)
		{
			int arr[2] = { sus,stanga };
			insertQ(arr, 2, instructions.actions);
		}
	}
	return instructions;
}

void copyMatrix(int** firstMatrix, int** secondMatrix, int marime)
{
	for (int i = 0; i < marime; i++)
	{
		for (int j = 0; j < marime; j++)
		{
			firstMatrix[i][j] = secondMatrix[i][j];
		}
	}
}

int** result(node* parent, int action, int marime)
{
	int** resultm = allocSquareMatrix(marime);
	copyMatrix(resultm, parent->stare, marime);
	blank blank = parent->pozitia_golului;
	int i = blank.row;
	int j = blank.col;
	switch (action)
	{
	case sus:
	{
		std::swap(resultm[i][j], resultm[i - 1][j]);
		break;
	}
	case jos:
	{
		std::swap(resultm[i][j], resultm[i + 1][j]);
		break;
	}
	case dreapta:
	{
		std::swap(resultm[i][j], resultm[i][j + 1]);
		break;
	}
	case stanga:
	{
		std::swap(resultm[i][j], resultm[i][j - 1]);
		break;
	}
	default:
	{
		break;
	}
	}
	return resultm;
}

node** expand(node* parent, int marime, int** stare_finala, int &cn)
{
	int children_number = 0;
	expandInstructions instructions;
	instructions = getInstructions(parent, marime);
	children_number = instructions.cnumber;
	node** succesors = new struct node*[children_number];
	std::queue<int> actions = instructions.actions;

	for (int i = 0; i < children_number; i++)
	{
		node* child = newNode(parent);
		child->actiune = actions.front();
		child->stare = result(parent, child->actiune, marime);
		actions.pop();
		child->adancime = parent->adancime + 1;
		child->cost = parent->cost + 1;
		child->fitness = calc_fitness(child->stare, stare_finala, marime) + child->adancime;
		child->pozitia_golului = findBlank(child->stare, marime);
		succesors[i] = child;
	}
	cn = children_number;
	return succesors;
}

int insertAllSuccesors(node* parent, int marime, int** stare_finala, priority_fringe &fringe)
{
	int cn = 0;
	node** succ = expand(parent, marime, stare_finala, cn);
	for (int i = 0; i < cn; i++)
	{
		fringe.push(succ[i]);
	}
	return cn;
}

void solution(node* sol)
{
	if (sol->parinte != nullptr)
	{
		solution(sol->parinte);
	}
	if (sol->adancime != 0)
	{
		std::cout << "Pasul " << sol->adancime << " =";
	}
	switch (sol->actiune)
	{
	case sus:
	{
		std::cout << " sus " << std::endl;
		delete sol->stare;
		delete sol;
		break;
	}
	case jos:
	{
		std::cout << " jos " << std::endl;
		delete sol->stare;
		delete sol;
		break;
	}
	case dreapta:
	{
		std::cout << " dreapta " << std::endl;
		delete sol->stare;
		delete sol;
		break;
	}
	case stanga:
	{
		std::cout << " stanga " << std::endl;
		delete sol->stare;
		delete sol;
		break;
	}
	default:
	{
		break;
	}
	}
}

std::stack<int> solution2(node* sol, std::stack<int> &sstack)
{
	if (sol->parinte != nullptr)
	{
		solution2(sol->parinte, sstack);
	}
	sstack.push(sol->actiune);
	return sstack;
}

bool FindSolution(int** stare_initiala, std::set<std::string> &closed, priority_fringe &fringe, int marime, int** stare_finala, std::stack<int> &sstack)
{
	fringe.push(initialNode(stare_initiala, marime, stare_finala));
	int noduri_expandate = 0;
	int noduri_vizitate = 0;
	int echo = 0;
	Timer timer;

	while (1)
	{
		if (fringe.empty() == true)
		{
			return false;
		}

		//echo++;
		//std::cout << "WORKING" << echo << std::endl;
		node* testing = fringe.top();
		noduri_vizitate++;

		if (compareTarget(testing, stare_finala, marime) == true)
		{

			solution(testing);
			//solution2(testing, sstack);
			std::cout << "NODURI VIZITATE : " << noduri_vizitate << std::endl;
			std::cout << "NODURI EXPANDATE : " << noduri_expandate << std::endl;
			return true;
		}
		else
		{
			fringe.pop();
			std::string hashid = hash(testing->stare, marime);
			if (findSet(closed, hashid) == false)
			{
				closed.insert(hashid);
				noduri_expandate += insertAllSuccesors(testing, marime, stare_finala, fringe);
			}
		}
	}
}

int prompt()
{
	int opt = 0;
	std::cout << "SELECTATI MODUL DE INTRODUCERE AL DATELOR: " << std::endl;
	std::cout << "1. Citire din fisier " << std::endl;
	std::cout << "2. Citire de la tastatura " << std::endl;
	while (opt == 0)
	{
		std::cin >> opt;
	}
	return opt;
}

void keyboardRead(int** stare_initiala, int marime)
{
	std::cout << "Introduceti configuratia puzzle-ului :" << std::endl;
	for (int i = 0; i < marime; i++)
	{
		for (int j = 0; j < marime; j++)
		{
			std::cin >> stare_initiala[i][j];
		}
	}
}

void kread(int &marime)
{
	std::cout << "Care este marimea puzzle-ului ?" << std::endl;
	std::cin >> marime;
}

void main()
{
	std::set<std::string> closed;
	//std::priority_queue<node*> fringe;
	priority_fringe pfringe;
	std::stack<int> sstack;

	int marime;
	int** stare_initiala = nullptr;
	int** stare_finala = nullptr;

	if (prompt() == 2)
	{
		kread(marime);
		stare_initiala = allocSquareMatrix(marime);
		stare_finala = allocSquareMatrix(marime);
		keyboardRead(stare_initiala, marime);
	}
	else
	{
		file >> marime;
		stare_initiala = allocSquareMatrix(marime);
		stare_finala = allocSquareMatrix(marime);
		fread(stare_initiala, marime);
	}

	getTarget(stare_finala, marime);
	std::cout << "STAREA INITIALA" << std::endl;
	printSquareMatrix(stare_initiala, marime);
	if (!validare_puzzle(stare_initiala, marime, stare_finala))
	{
		std::cout << "STAREA INITIALA NU ESTE VALIDA !!!" << std::endl;
		return;
	}

	std::cout << "HASH: " << hash(stare_initiala, marime) << std::endl << std::endl;
	std::cout << "FITNESS : " << calc_fitness(stare_initiala, stare_finala, marime) << std::endl;
	std::cout << "TARGET " << std::endl;
	printSquareMatrix(stare_finala, marime);

	FindSolution(stare_initiala, closed, pfringe, marime, stare_finala, sstack);

	delete stare_finala;
	delete stare_initiala;

}