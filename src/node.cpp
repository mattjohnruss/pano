#include <node.h>

namespace Pano
{
    Node::Node()
        : image_(PanoImage())
    {
        std::cout << "node constructor\n";
    }
}
