#include <model.h>

#include <iostream>
#include <stack>

Model::Model(const std::string &path)
{
    // create and importer object
    Assimp::Importer importer;

    // import the scene from the file
    // the importer retains ownership of all data and will destroy the scene in
    // its destructor
    const aiScene *scene =
        importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals);

    // check if the scene was loaded correctly
    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cerr << "Model::Model - Assimp error:" << importer.GetErrorString() << '\n';
        return;
    }

    // get the directory from the filename
    directory_ = path.substr(0, path.find_last_of('/'));

    // traverse and process the scene
    traverse_and_process(scene);
}

Model::~Model()
{
    // get the number of meshes
    unsigned n_mesh = meshes_.size();

    // loop over the meshes and delete them all
    for(unsigned i = 0; i < n_mesh; ++i)
    {
        delete meshes_[i];
    }
}

const std::unordered_map<std::string, MeshTexture>& Model::textures() const
{
    // return the textures map (const)
    return textures_;
}

std::unordered_map<std::string, MeshTexture>& Model::textures()
{
    // return the textures map
    return textures_;
}

const std::vector<Mesh *>& Model::meshes() const
{
    // return the vector of meshes (const)
    return meshes_;
}

void Model::draw(const Shader &shader, const GLenum mode) const
{
    // get the number of meshes
    unsigned n_mesh = meshes_.size();

    // loop over the meshes
    for(unsigned i = 0; i < n_mesh; ++i)
    {
        // draw the draw function on each mesh
        meshes_[i]->draw(shader, mode);
    }
}

void Model::traverse_and_process(const aiScene *const scene)
{
    // stack of nodes
    std::stack<aiNode*> node_stack;

    // push the root node onto the stack
    node_stack.push(scene->mRootNode);

    // while there are nodes left to proceses
    while(!node_stack.empty())
    {
        // get the top node and pop off the stack
        aiNode *top_node = node_stack.top();
        node_stack.pop();

        // get the number of children of the current node
        unsigned n_children = top_node->mNumChildren;

        // push the children of the top node onto the stack
        for(unsigned i = 0; i < n_children; ++i)
        {
            node_stack.push(top_node->mChildren[i]);
        }

        // process the top node
        process_node(top_node, scene);
    }
}

void Model::process_node(aiNode *node, const aiScene *const scene)
{
    // get the number of meshes at the node
    unsigned n_mesh = node->mNumMeshes;

    // loop over the meshes at the current node
    for(unsigned i = 0; i < n_mesh; ++i)
    {
        // get a pointer to the current mesh
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];

        // process the mesh into a Mesh object and store a pointer to it in meshes_
        meshes_.push_back(new Mesh(mesh, scene, directory_, *this));
    }
}
