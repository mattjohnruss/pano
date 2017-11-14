#include <node.h>
#include <pano_image.h>

#include <iostream>

namespace Pano
{
    Node::Node()
        : image_(PanoImage())
    {
        std::cout << "node constructor\n";
    }
}
