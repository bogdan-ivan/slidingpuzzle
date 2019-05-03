#include "Puzzle.h"

static constexpr int unitialized = -1;

Puzzle::Puzzle()
	:m_marime(unitialized), m_stare_initiala(nullptr), m_stare_finala(nullptr), m_valid(false)
{

}

Puzzle::~Puzzle()
{
	if (m_stare_initiala != nullptr)
	{
		deleteMatrix(m_stare_initiala, m_marime);
	}
	if (m_stare_finala != nullptr)
	{
		deleteMatrix(m_stare_finala, m_marime);
	}
	m_file.close();
}

bool Puzzle::validate()
{
	std::set<int> validating;
	blank blank = findBlank(m_stare_initiala);
	int arraysize = m_marime*m_marime;
	int* marray = matrixToArray(m_stare_initiala);
	int inversiuni = 0;
	int width = sqrt(arraysize);
	int corr = 0;

	for (int i = 0; i < m_marime; i++)
	{
		for (int j = 0; j < m_marime; j++)
		{
			if (m_stare_initiala[i][j] == m_stare_finala[i][j])
			{
				corr++;
			}
		}
	}

	if (corr == m_marime*m_marime)
	{
		std::cout << "STAREA INITIALA ESTE STAREA FINALA !!!" << std::endl;
		return true;
	}

	for (int i = 0; i < m_marime; i++)
	{
		for (int j = 0; j < m_marime; j++)
		{
			validating.insert(m_stare_initiala[i][j]);
		}
	}

	if (validating.size() != m_marime*m_marime)
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

bool Puzzle::readFromFile(const char* fileName)
{

	m_file.open(fileName);
	if (!m_file)
		return false;
	m_file >> m_marime;
	if (m_marime < 1)
		return false;
	m_stare_initiala = allocSquareMatrix(m_marime);
	m_stare_finala = allocSquareMatrix(m_marime);

	for (int i = 0; i < m_marime; i++)
	{
		for (int j = 0; j < m_marime; j++)
		{
			m_file >> m_stare_initiala[i][j];
		}
	}

	getTarget();
	m_valid = validate();

	return true;
}

bool Puzzle::readFromKeyboard()
{
	while (m_marime < 1)
	{
		std::cout << "Care este marimea puzzle-ului ?" << std::endl;
		std::cin >> m_marime;
	}

	std::cout << "Introduceti configuratia puzzle-ului :" << std::endl;
	for (int i = 0; i < m_marime; i++)
	{
		for (int j = 0; j < m_marime; j++)
		{
			std::cin >> m_stare_initiala[i][j];
		}
	}
	
	getTarget();
	m_valid = validate();
	
	return false;
}

bool Puzzle::findSolution()
{
	if (!m_valid)
		return false;

	m_fringe.push(initialNode());
	int noduri_expandate = 0;
	int noduri_vizitate = 0;
	int echo = 0;
	Timer timer;

	while (1)
	{
		if (m_fringe.empty() == true)
		{
			return false;
		}

		node* testing = m_fringe.top();
		noduri_vizitate++;

		if (compareTarget(testing) == true)
		{

			solution(testing);
			std::cout << "NODURI VIZITATE : " << noduri_vizitate << std::endl;
			std::cout << "NODURI EXPANDATE : " << noduri_expandate << std::endl;
			return true;
		}
		else
		{
			m_fringe.pop();
			std::string hashid(std::move(hash(testing->stare)));
			if (findSet(hashid) == false)
			{
				m_closed.insert(hashid);
				noduri_expandate += insertAllSuccesors(testing, m_stare_finala, m_fringe);
			}
		}
	}
}

void Puzzle::current() const noexcept
{
	for (int i = 0; i < m_marime; i++)
	{
		for (int j = 0; j < m_marime; j++)
		{
			std::cout << m_stare_initiala[i][j] << " ";
		}
		std::cout << std::endl;
	}
}

int ** Puzzle::allocSquareMatrix(int size)
{
	int** matrix = new int*[size];

	for (int i = 0; i < size; ++i)
	{
		matrix[i] = new int[size];
	}
	return matrix;
}

void Puzzle::deleteMatrix(int ** matrix, int row)
{
	for (int i = 0; i < row; ++i)
	{
		delete[] matrix[i];
	}
	delete matrix;
}

node * Puzzle::newNode()
{
	node* node = new struct node;
	return node;
}

node * Puzzle::newNode(node * parent)
{
	node* node = new struct node;
	node->parinte = parent;
	return node;
}

node * Puzzle::initialNode()
{
	node* node = new struct node;
	node->stare = m_stare_initiala;
	node->parinte = nullptr;
	node->actiune = 5;
	node->cost = 0;
	node->adancime = 0;
	node->pozitia_golului = findBlank(node->stare);
	node->fitness = calc_fitness(m_stare_initiala, m_stare_finala) + node->adancime;
	return node;
}

void Puzzle::getTarget()
{
	int index = 1;
	for (int i = 0; i < m_marime; i++)
	{
		for (int j = 0; j < m_marime; j++)
		{
			m_stare_finala[i][j] = index;
			index++;
		}
	}
	m_stare_finala[m_marime - 1][m_marime - 1] = 0;
}

int Puzzle::calc_fitness(int ** stare_initiala, int ** stare_finala)
{
	int sumaDistantelor = 0;
	for (int i = 0; i < m_marime; i++)
	{
		for (int j = 0; j < m_marime; j++)
		{
			point trgt = getPoint(stare_finala, stare_initiala[i][j]);
			sumaDistantelor += abs(i - trgt.row) + abs(j - trgt.col);
		}
	}
	return sumaDistantelor;
}

std::string Puzzle::hash(int ** matrix)
{
	std::string hash;
	for (int i = 0; i < m_marime; i++)
	{
		for (int j = 0; j < m_marime; j++)
		{
			hash += std::to_string(matrix[i][j]);
		}
	}
	return hash;
}

blank& Puzzle::findBlank(int ** matrix)
{
	blank blank;
	for (int i = 0; i < m_marime; i++)
	{
		for (int j = 0; j < m_marime; j++)
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

point & Puzzle::getPoint(int ** matrix, int element)
{
	point coordonate;
	for (int i = 0; i < m_marime; i++)
	{
		for (int j = 0; j < m_marime; j++)
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

void Puzzle::printSquareMatrix(int ** matrix, int n) const noexcept
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

void Puzzle::printArray(int * matrix, int n) const noexcept
{
	for (int i = 0; i < n; i++)
	{
		std::cout << matrix[i] << " ";
	}
	std::cout << std::endl;
}

int * Puzzle::matrixToArray(int ** matrix)
{
	int arraysize = m_marime*m_marime;
	int* array = new int[arraysize];
	int index = 0;
	for (int i = 0; i < m_marime; i++)
	{
		for (int j = 0; j < m_marime; j++)
		{
			array[index] = matrix[i][j];
			index++;
		}
	}
	return array;
}

void Puzzle::insertQ(int arr[], int n, std::queue<int>& actions)
{
	for (int i = 0; i < n; i++)
	{
		actions.push(arr[i]);
	}
}

expandInstructions Puzzle::getInstructions(node * parent, int marime)
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

void Puzzle::printSet() const noexcept
{
	std::cout << "\nThe set closed is : ";
	for (auto itr = m_closed.begin(); itr != m_closed.end(); itr++)
	{
		std::cout << *itr << " ";
	}
	std::cout << std::endl;
}

bool Puzzle::findSet(const std::string & hash)
{
	return m_closed.find(hash) != m_closed.end();
}

bool Puzzle::compareTarget(node * node)
{
	int** matrix = node->stare;
	for (int i = 0; i < m_marime; i++)
	{
		for (int j = 0; j < m_marime; j++)
		{
			if (matrix[i][j] != m_stare_finala[i][j])
			{
				return false;
			}
		}
	}
	return true;
}

void Puzzle::copyMatrix(int ** firstMatrix, int ** secondMatrix)
{
	for (int i = 0; i < m_marime; i++)
	{
		for (int j = 0; j < m_marime; j++)
		{
			firstMatrix[i][j] = secondMatrix[i][j];
		}
	}
}

int ** Puzzle::result(node * parent, int action)
{
	int** resultm = allocSquareMatrix(m_marime);
	copyMatrix(resultm, parent->stare);
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

node ** Puzzle::expand(node * parent, int marime, int ** stare_finala, int & cn)
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
		child->stare = result(parent, child->actiune);
		actions.pop();
		child->adancime = parent->adancime + 1;
		child->cost = parent->cost + 1;
		child->fitness = calc_fitness(child->stare, stare_finala) + child->adancime;
		child->pozitia_golului = findBlank(child->stare);
		succesors[i] = child;
	}
	cn = children_number;
	return succesors;
}

int Puzzle::insertAllSuccesors(node * parent, int ** stare_finala, priority_fringe & fringe)
{
	int cn = 0;
	node** succ = expand(parent, m_marime, stare_finala, cn);
	for (int i = 0; i < cn; i++)
	{
		fringe.push(succ[i]);
	}
	return cn;
}

void Puzzle::solution(node * sol)
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
