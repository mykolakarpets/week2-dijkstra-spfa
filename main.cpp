#include <iostream>
#include <list>
#include <map>

using namespace std;

namespace {
    class Vertice {
        map <const Vertice&, unsigned> neighbours;

        friend class Graph;
    };

    class Graph {
        list<Vertice> v;
    public:
//TODO: private class for algorithm
        unsigned dijkstra_spf(Vertice& start, Vertice& destination) {
            //TODO: we can't store references in std::map, pointer or hash
            //string view
            //gsl span
            typedef map <const Vertice*, unsigned> vertice_t;

            vertice_t unvisited {};
            vertice_t visited {};
            //TODO: boost range, boost filter, multiindex(database)

            for (auto& vertice : v) {
                unvisited[vertice] = UINT_MAX;
            }

            Vertice& current = start;
            //TODO: 1
            unvisited[current] = 0;

            for (auto& _neighbour : current.neighbours) {
                auto& neighbour = _neighbour.first;

                //TODO: may be too consuming
                if (not unvisited.find(neighbour))
                    continue;

                unvisited[neighbour] = min(unvisited[neighbour], unvisited[current] + _neighbour.second);
            }

            visited.insert(std::move(unvisited.extract(current)));
        }
    private:
        class _Vertice_D {//Vertice for dijkstra algorithm


        };
    };




}

int main() {
    std::cout << "Hello, World!" << std::endl;
    return 0;
}