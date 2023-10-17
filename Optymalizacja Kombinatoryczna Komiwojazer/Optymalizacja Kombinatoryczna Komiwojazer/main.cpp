#include <iostream>
#include <vector>
#include <algorithm>

#define V 5

double dystans(int graph[][V], int vertex1, int vertex2) {
	return graph[vertex1][vertex2];
}

void TSP_choose_shortest(int matrix_graph[][V]) {
	//tworze dwa wektory, pierwszy to trasa, drugi to flaga na wierzcholki
	std::vector<int> trasa;
	std::vector<bool> visited(V, false);
	//wybieram miasto poczatkowe
	int currentVertex = 0;
	visited[currentVertex] = true;
	trasa.push_back(currentVertex);
	
	for (int i = 1; i < V; i++) {
		int nextVertex = -1;
		//to jest dziwne, ustawiam aktualny najmniejszy dystans na najwieksza wartosc inta
		int minimalnyDystans = std::numeric_limits<int>::max();
		for (int j = 0; j < V; j++) {
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
	for (int vertex : trasa) {
		std::cout << vertex << " -> ";
	}
	std::cout << "0" << std::endl;
	double dystans_value = 0.0;
	for (int i = 0; i < trasa.size() - 1; i++) {
		//std::cout << "krok: " << dystans(matrix_graph, trasa[i], trasa[i + 1]) << std::endl;
		dystans_value += dystans(matrix_graph, trasa[i], trasa[i + 1]);
	}
	std::cout << "dystans w sumie to: " << dystans_value << std::endl;
}



int main() {
	
	/*reprezentacja w postaci macierzy grafu, ale zamiast zapisywaæ 0/1 zapisujemy odleg³oœæ miedzy wierzcholkami, jesli taka krawedz je laczaca istnieje*/
	int graph[][V] = {
		{0,304,304,403,280},
		{304,0,341,584,486},
		{304,341,0,299,341},
		{403,584,299,0,344},
		{280,486,341,344,0}
	};
	TSP_choose_shortest(graph);
	return 0;
}