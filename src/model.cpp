#include <model.h>

#include <iostream>

Model::Model(const std::string &path)
{
    // create and importer object
    Assimp::Importer importer;

    // import the scene from the file
    // the importer retains ownership of all data and will destroy the scene in
    // its destructor
    const aiScene *scene =
        importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

    // check if the scene was loaded correctly
    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cerr << "ERROR::MODEL::ASSIMP::" << importer.GetErrorString() << '\n';
        return;
    }

    directory_ = path.substr(0, path.find_last_of('/'));

    // start processing the scene from the root node
    process_node(scene->mRootNode, scene);
}

Model::~Model()
{
    unsigned n_mesh = meshes_.size();

    for(unsigned i = 0; i < n_mesh; ++i)
    {
        delete meshes_[i];
    }
}

void Model::draw(Shader &shader) const
{
    // get the number of meshes
    unsigned n_mesh = meshes_.size();

    // loop over the meshes
    for(unsigned i = 0; i < n_mesh; ++i)
    {
        // draw the draw function on each mesh
        meshes_[i]->draw(shader);
    }
}

void Model::process_node(aiNode *node, const aiScene * const scene)
{
    //std::cout << "Model::process_node\n";

    // get the number of meshes at the node
    unsigned n_mesh = node->mNumMeshes;

    // loop over the meshes at the current node
    for(unsigned i = 0; i < n_mesh; ++i)
    {
        // get a pointer to the current mesh
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];

        // process the mesh into a Mesh object and store a pointer to it in meshes_
        meshes_.push_back(new Mesh(mesh, scene, directory_, *this));
        //std::cout << "Model::process_node - new mesh created\n";
    }

    // get the number of children of the current node
    unsigned n_children = node->mNumChildren;

    // loop over the children
    for(unsigned i = 0; i < n_children; ++i)
    {
        //std::cout << "Model::process_node - child\n";
        // recursively processes the children of the current node
        // (awful - what if the tree is very deep?)
        process_node(node->mChildren[i], scene);
    }
}
