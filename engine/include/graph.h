#pragma once

#include <node.h>

#include <utility>
#include <vector>

namespace Pano
{
    typedef std::pair<Node*,Node*> Edge;

    class Graph
    {
        public:
            Graph();
            ~Graph();

            void add_node();

        private:
            std::vector<Node> nodes_;
            std::vector<Edge> edges_;
    };
}
