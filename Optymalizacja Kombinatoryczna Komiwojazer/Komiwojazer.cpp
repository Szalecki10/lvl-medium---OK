
#include <random>
#include <fstream>
#include <iostream>
#include <limits.h>
#include <vector>
#include <stdlib.h>
#include <string>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <algorithm>

#define POP_SIZE 10

typedef std::vector< std::vector<double> > Matrix;
typedef std::vector<double> Row;
typedef std::vector<int> Dna;


//struktura jednej odpowiedzi
struct individual {
	Dna gnome;
	double fitness;
};


//randomowa liczba
int rand_num(int start, int end)
{
	int r = end - start;
	int rnum = start + rand() % r;
	return rnum;
}

//sprawdzenie czy sie powtarza
bool repeat(Dna s, int nowa)
{
	for (int i = 0; i < s.size(); i++) {
		if (s[i] == nowa)
			return true;
	}
	return false;
}
Dna mate(individual par1, individual par2) {
	int len = par2.gnome.size();
	// std::cout << len;
	Dna child_gnome;
	//for (int i = 0; i < len; i++) {
		//std::cout << par1.gnome[i];
	//}
	for (int i = 0; i < len; i++) {
		float p = rand_num(0, 100) / 100;
		if (p < 0.5) {
			child_gnome.push_back(par1.gnome[i]);
		}
		else {
			child_gnome.push_back(par2.gnome[i]);
		}
		//std::cout << child_gnome[i] << "-" << std::endl;
	}
	
	return child_gnome;
}
//zamiana dwóch genów miejscami
Dna mutatedGene(Dna gnome, int V)
{
	while (true) {
		int r = rand_num(1, V);
		int r1 = rand_num(1, V);
		if (r1 != r) {
			int temp = gnome[r];
			gnome[r] = gnome[r1];
			gnome[r1] = temp;
			break;
		}
	}
	return gnome;
}

//tworzenie genomu
Dna create_gnome(int V)
{
	Dna gnome;
	gnome.push_back(0);
	while (true) {
		if (gnome.size() == V) {
			gnome.push_back(0);
			break;
		}
		int temp = rand_num(1, V);
		if (!repeat(gnome, temp)) {
			gnome.push_back(temp);
		}

	}
	return gnome;
}


double cal_fitness(Dna gnome, int V, Matrix map)
{
	double f = 0;
	for (int i = 0; i < gnome.size() - 1; i++) {
		//std::cout << f << "+" << map[gnome[i]][gnome[i + 1]] << std::endl;
		f += map[gnome[i]][gnome[i + 1]];

	}
	return f;
}


//cooling element
int cooldown(int temp)
{
	return (90 * temp) / 100;
}

//porównywanie, okazalo sie useless
bool lessthan(struct individual t1,
	struct individual t2)
{
	return t1.fitness < t2.fitness;
}

individual newlessthan(struct individual t1, struct individual t2) {
	if (t1.fitness < t2.fitness) {

		return t1;
	}
	else {
		std::cout << "progress-> " << t2.fitness << std::endl;
		return t2;
	}
}

//główna funkcja genetycznego
void TSP_GEN(Matrix map, int V)
{
	// generacja 1
	int gen = 1;
	// ilosc iteracji
	int gen_thres = 15;

	//odp
	Dna smieci;
	individual syn_kolezanki_mamy = { smieci,std::numeric_limits < double >::max() };


	//populacja
	std::vector<struct individual> population;
	struct individual temp;

	// zapełnianie populacji
	for (int i = 0; i < POP_SIZE; i++) {
		temp.gnome = create_gnome(V);
		temp.fitness = cal_fitness(temp.gnome, V, map);
		population.push_back(temp);
	}

	std::cout << "\nInitial population: " << std::endl
		<< "GNOME     FITNESS VALUE\n";
	for (int i = 0; i < POP_SIZE; i++) {
		for (int j = 0; j < V + 1; j++) {
			std::cout << population[i].gnome[j] << "-";
		}
		std::cout << ">     " << population[i].fitness << std::endl;
		syn_kolezanki_mamy = newlessthan(syn_kolezanki_mamy, population[i]);
	}


	bool found = false;
	int temperature = 10000;

	//krzyzowanie i mutacje
	while (temperature > 1000 && gen <= gen_thres) {
		sort(population.begin(), population.end(), lessthan);
		std::cout << "\nCurrent temp: " << temperature << "\n";
		std::vector<struct individual> new_population;

		int ss = 2;
		for (int i = 0; i < ss; i++) {
			new_population.push_back(population[i]);
		}

		int sss = 9;
		for (int i = 0; i < sss; i++) {
			int len = population.size();
			individual parent1 = population[0];
			individual parent2 = population[1];
			individual offspring;
			offspring.gnome = mate(parent1, parent2);
			offspring.fitness = cal_fitness(offspring.gnome, V, map);
			new_population.push_back(offspring);
			
		}
		

		temperature = cooldown(temperature);
		
		population = new_population;
		std::cout << "Generation " << gen << " \n";
		std::cout << "GNOME     FITNESS VALUE\n";
		
		for (int i = 0; i < POP_SIZE; i++) {
			for (int j = 0; j < V + 1; j++) {
				std::cout << population[i].gnome[j] << "-";
			}
			std::cout << ">     " << population[i].fitness << std::endl;
			syn_kolezanki_mamy = newlessthan(syn_kolezanki_mamy, population[i]);
		}
		gen++;
	}
	std::cout << "\n\n\n\n najlepszy gen to: ";
	for (int j = 0; j < V + 1; j++) {
		std::cout << syn_kolezanki_mamy.gnome[j] << "-";
	}
	std::cout << "  jego fitness to: " << syn_kolezanki_mamy.fitness;
}

//koniec algorytmu genetycznego

void generate() {
	std::srand((std::time(nullptr)));
	int n;
	std::cout << "wpisz ile chcesz wierzchołków " << std::endl;
	std::cin >> n;
	std::cout << std::endl;
	std::ofstream zapis("in.txt");
	zapis << n << std::endl;
	for (int i = 1; i < n + 1; i++) {
		zapis << i << " " << (std::rand() % 100) * 1 << " " << (std::rand() % 100) * 1 << std::endl;
	}
}

double calculate(int x1, int y1, int x2, int y2)
{
	double wynik;
	//std::cout << "x1: " << x1 << "  x2: " << x2 << "  y1: " << y1 << "  y2: " << y2 << std::endl;
	wynik = std::sqrt(std::pow(x2 - x1, 2) + std::pow(y2 - y1, 2));
	//std::cout << "wynik: " << wynik << std::endl;
	return wynik;
}

//stała ilość wierzchołków
//#define V 5

double dystans(int n, Matrix graph, int vertex1, int vertex2) {
	return graph[vertex1][vertex2];
}

void TSP_choose_shortest(int n, Matrix matrix_graph) {
	//tworze dwa wektory, pierwszy to trasa, drugi to flaga na wierzcholki
	std::vector<int> trasa;
	std::vector<bool> visited(n, false);
	//wybieram miasto poczatkowe
	int currentVertex = 0;
	visited[currentVertex] = true;
	trasa.push_back(currentVertex);
	double dystans_value = 0.0;

	for (int i = 1; i < n; i++) {
		int nextVertex = -1;
		//to jest dziwne, ustawiam aktualny najmniejszy dystans na najwieksza wartosc inta
		int minimalnyDystans = std::numeric_limits<int>::max();
		for (int j = 0; j < n; j++) {
			if (matrix_graph[currentVertex][j] < minimalnyDystans && !visited[j]) {
				minimalnyDystans = matrix_graph[currentVertex][j];
				nextVertex = j;
			}
		}
		if (nextVertex != -1) {
			trasa.push_back(nextVertex);
			visited[nextVertex] = true;
			currentVertex = nextVertex;
		}

	}
	currentVertex = 0;
	trasa.push_back(currentVertex);
	for (int vertex : trasa) {
		std::cout << vertex << " -> ";
	}
	//std::cout << "0" << std::endl;
	for (int i = 0; i < trasa.size() - 1; i++) {
		//std::cout << "krok: " << dystans(matrix_graph, trasa[i], trasa[i + 1]) << std::endl;
		dystans_value += dystans(n, matrix_graph, trasa[i], trasa[i + 1]);
		//std::cout <<"teraz: "<< dystans(n, matrix_graph, trasa[i], trasa[i + 1]) << std::endl;
		//std::cout << "w sumie "<< dystans_value << std::endl;
	}


	std::cout << "dystans w sumie to: " << dystans_value << std::endl;

}

int main() {

	int wybor;
	std::cout << "Jesli masz plik, wybierz 1, jesli nie, wybierz 2, skonczyc 3 " << std::endl;
	std::cin >> wybor;
	std::cout << std::endl;
	bool koniec = true;
	while (koniec != false)
	{

		switch (wybor)
		{
		case 1:
		{

			//to jest otwarcie pliku
			std::ifstream odczyt("in.txt");
			std::string linia;
			//tu chodzi tylko o to, że ta pierwsza linia jest inna, więc chce ją od razu przerzucić do zmiennej i zapomnieć o jej iostnieniu
			getline(odczyt, linia);
			int V = stoi(linia);
			if (V < 1) {
				break;
			}
			//tu robię sobie vektor vektorów -> czyli macierz


			const size_t N = 3; //macierz jest zawsze szeroka na 3 i wysoka na 2
			Matrix matrix;
			std::string s, dump;
			std::vector<int> ciag;
			getline(odczyt, dump, ' ');
			//tu zaczynają się jaja
			//tworze vector intów do którego wpycham wszystkie wartości po pierwszej linii z pliku
			int integer;
			//to slice'uje mi wszystko po spacji, ale nie ogarnia do końca konceptu entera, później to się ogarnie
			while (getline(odczyt, s, ' ')) {
				integer = stoi(s);
				//std::cout << integer << "  ";
				ciag.push_back(integer);
			}
			/*
			//nie ma po co tego drukować
			for (size_t i = 0; i < ciag.size(); i++) {
			}*/
			//std::cout << ciag.size();

			//tu zaczynają się jaja^2
			//counter - liczy mi który rzecz z vectora ciag powinna byc teraz dodana do macierzy
			int counter = 0;
			//dodawać będziemy całe wiersze od razu
			Row row(V);
			for (int i = 0; i < V; ++i)
			{


				//konstrukcja wiersza to LP, X, Y, jako że nasza funkcja slice'owania nie wie jak działa enter, wpiszemy LP ręcznie XD
				row[0] = i + 1;
				//tutaj dodaje X i Y, po to jest mi counter żeby wiedział które one mają miejsca w vektorze ciag
				for (size_t j = 1; j < N; ++j)
				{
					//row[j] = odczyt.get();

					int temp = ciag[counter];
					row[j] = temp;
					counter++;

				}
				/*for (int i = 0; i < 3; i++)
				{
					std::cout << row[i] << "  ";
				}
				std::cout << std::endl;*/
				matrix.push_back(row); //pushuje wiersze do tabeli
			}
			//no i mam macierz, która działa, uwaga uwaga, jak macierz

			//zaczynamy liczyć 
			//potrzebujemy macierzy sąsiedztwa
			//uwaga uwaga - nie mamy macierzy sąsiedztwa
			Matrix neighbour_matrix;
			Row n_row;
			for (int i = 0; i < V; ++i)
			{
				Row n_row(V);
				for (size_t j = 0; j < V; ++j)
				{
					if (i != j) {
						n_row[j] = calculate(matrix[i][1], matrix[i][2], matrix[j][1], matrix[j][2]);
					}
					else
					{
						n_row[j] = 0;
					}
				}

				{

				}

				neighbour_matrix.push_back(n_row);
			}
			/*
			for (size_t i = 0; i < V; ++i)
			{
				for (size_t j = 0; j < V; ++j)
				{
					std::cout << neighbour_matrix[i][j] << " ";
				}

				std::cout << std::endl;
			}
			*/
			TSP_choose_shortest(V, neighbour_matrix);
			TSP_GEN(neighbour_matrix, V);
			std::cin>>wybor;
			
			break;
		}
		case 2:
		{
			generate();
			wybor = 3;
			break;
		}
		case 3:
		{
			koniec = false;
			break;
		}
		default:
		{
			wybor = 3;
			break;
		}
		}
	}
	return 0;
}