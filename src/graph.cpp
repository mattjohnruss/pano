#include <graph.h>

namespace Pano
{
    Graph::Graph()
    {
        std::cout << "graph constructor\n";
    }

    Graph::~Graph()
    {
    }

    void Graph::add_node()
    {
        nodes_.push_back(Node());
    }
}
