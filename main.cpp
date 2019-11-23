#include <iostream>
#include <list>
#include <map>
#include <vector>

#include <cstdio>

using namespace std;

namespace {
    static void log(const char *prefix, const char *file, int line, const char *format, ...) {
#ifdef BUILD_DEBUG
        static constexpr size_t print_bufsize = 255;
        char buf[print_bufsize];
        va_list args;

        va_start(args, format);
        vsnprintf(buf, print_bufsize, format, args);
        va_end(args);

        fprintf(stdout, "%s %s:%d %s\n", prefix, file, line, buf);
#endif
    }

#define LOG_DEBUG(format, ...) \
    log("[DEBUG]", __FILE__, __LINE__, format, ##__VA_ARGS__)


    class Vertice {
    public:
        string name{"default_name"};

        Vertice() = default;

        Vertice(string _name) : name(_name) {};

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
            vector<Vertice *> unvisited{};
            vector<Vertice *> visited{};
            //TODO: boost range, boost filter, multiindex(database)

            for (Vertice *vertice : v) {
                unvisited.push_back(vertice);
            }

            auto current = find(unvisited.begin(), unvisited.end(), &start);
            if (current == unvisited.end()) {
                cout << "The start is not a part of the graph" << endl;
                return UINT_MAX;
            }

            (*current)->tentative_dst = 0;

            auto v_comp = [](const Vertice *v1, const Vertice *v2) { return v1->tentative_dst < v2->tentative_dst; };

            while (true) {
                LOG_DEBUG("current vertice = %s", (*current)->name.c_str());
                for (auto &_neighbour : (*current)->neighbours) {
                    LOG_DEBUG("\tneighbour = %s", _neighbour.first->name.c_str());
                    //TODO: just add field "visited"
                    auto _vertice = find(unvisited.begin(), unvisited.end(), _neighbour.first);
                    if (_vertice == unvisited.end())
                        continue;

                    (*_vertice)->tentative_dst = min(_neighbour.first->tentative_dst,
                                                     (*current)->tentative_dst + _neighbour.second);
                }

                if (*current == &destination)
                    break;

                unvisited.erase(current);

                if (unvisited.empty()) {
                    break;
                } else {
                    make_heap(unvisited.begin(), unvisited.end(), v_comp);
                    current = unvisited.end() - 1;
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