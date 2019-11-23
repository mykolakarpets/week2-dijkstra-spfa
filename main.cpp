#include <iostream>
#include <list>
#include <map>
#include <set>

using namespace std;

namespace {
    class Vertice {
    public:
        string name {"default_name"};

        Vertice() = default;

        Vertice(string _name): name(_name) {};

        Vertice(map<Vertice *, unsigned> &neighbours) : neighbours(neighbours) {};

        //TODO: can automatically choose whether passed by reference or value?
        void add_neighbours(map<Vertice *, unsigned> neighbours) {
            this->neighbours = std::move(neighbours);
        }

    private:
        map<Vertice *, unsigned> neighbours{};

        unsigned tentative_dst{UINT_MAX};

        friend class Graph;
    };

    class Graph {
        //TODO: structures on heap
    public:
        static unsigned dijkstra_spf(list<Vertice *> &v, Vertice &start, Vertice &destination) {
            set<Vertice *, bool (*)(const Vertice *, const Vertice *)> unvisited(
                    [](const Vertice *v1, const Vertice *v2) { return v1->tentative_dst < v2->tentative_dst; }
            );
            set<Vertice *> visited{};
            //TODO: boost range, boost filter, multiindex(database)

            for (Vertice *vertice : v) {
                unvisited.insert(vertice);
            }

            auto current = find(unvisited.begin(), unvisited.end(), &start);
            if (current == unvisited.end()) {
                cout << " start is not a part of the graph" << endl;
                return UINT_MAX;
                //TODO: throw an exception
            }

            auto temp = unvisited.extract(current);
            temp.value()->tentative_dst = 0;
            unvisited.insert(std::move(temp));


            while (true) {
                cout << "Current node: " << (*current)->name << endl;
                for (auto &_neighbour : (*current)->neighbours) {
                    cout << "Current neightbour: " << _neighbour.first->name <<endl;
                    //TODO: just add field "visited"
                    auto _vertice = find(unvisited.begin(), unvisited.end(), _neighbour.first);
                    if (_vertice == unvisited.end())
                        continue;

                    auto node = unvisited.extract(_vertice);

                    node.value()->tentative_dst = min(_neighbour.first->tentative_dst,
                                                      (*current)->tentative_dst + _neighbour.second);

                    //TODO: find a way to re-sort without re-placing
                    unvisited.insert(std::move(node));
                }

                visited.insert(std::move(unvisited.extract(current)));

                if (*current == &destination)
                    break;

                if (unvisited.empty())
                    break;
                else {
                    current = unvisited.begin();
                    if ((*current)->tentative_dst == UINT_MAX) {
                        cout << "No connection between nodes!" << endl;
                        return UINT_MAX;
                    }
                }

            }

            return (*current)->tentative_dst;
        }
    };
}

int main() {
    Vertice v1("v1"), v2("v2"), v3("v3"), v4("v4");
    v1.add_neighbours({
                              {&v2, 1},
                              {&v3, 3}
                      });
    v2.add_neighbours({
                              {&v3, 5},
                              {&v4, 2}
                      });
    v3.add_neighbours({
                              {&v4, 4}
                      });

    std::list<Vertice *> test1{&v1, &v2, &v3, &v4};

    cout << Graph::dijkstra_spf(test1, v1, v4) << endl;

    return 0;
}