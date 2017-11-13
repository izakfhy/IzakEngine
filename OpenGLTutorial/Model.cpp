#include "Model.h"
#include "ResourceManager.h"
#include "MetaData.h"
#include "Texture.h"
#include "Globals.h"

DEFINE_META(Model)
{
  ADD_MEMBER(meshes, false);
  ADD_MEMBER(directory, true);
}

Model::~Model()
{
  meshes.clear();
}

void Model::Draw(std::shared_ptr<Shader> shader)
{
  for (unsigned int i = 0; i < meshes.size(); i++)
    meshes[i].Draw(shader);
}


std::vector<Mesh>& Model::GetMeshes()
{
  return meshes;
}

bool Model::Initialize(const std::string& path)
{
  return loadModel(path);
}

bool Model::loadModel(std::string path)
{
  m_path = path;
  Assimp::Importer import;
  const aiScene *scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

  if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
  {
    std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
    return false;
  }
  directory = path.substr(0, path.find_last_of('\\'));
  directory = directory.substr(directory.find_last_of('\\') + 1, directory.length());
  processNode(scene->mRootNode, scene);
  return true;
}


void Model::processNode(aiNode *node, const aiScene *scene)
{
  // process all the node's meshes (if any)
  for (unsigned int i = 0; i < node->mNumMeshes; i++)
  {
    aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
    meshes.push_back(processMesh(mesh, scene));
  }
  // then do the same for each of its children
  for (unsigned int i = 0; i < node->mNumChildren; i++)
  {
    processNode(node->mChildren[i], scene);
  }
}


Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene)
{
  std::vector<Vertex> vertices;
  std::vector<unsigned int> indices;
  std::vector<std::shared_ptr<IZE::Texture>> textures;

  for (unsigned int i = 0; i < mesh->mNumVertices; i++)
  {
    Vertex vertex;
    // process vertex positions, normals and texture coordinates
    Vector3 vec;
    vec.x = mesh->mVertices[i].x;
    vec.y = mesh->mVertices[i].y;
    vec.z = mesh->mVertices[i].z;
    vertex.Position = vec;

    vec.x = mesh->mNormals[i].x;
    vec.y = mesh->mNormals[i].y;
    vec.z = mesh->mNormals[i].z;
    vertex.Normal = vec;

    if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
    {
      Vector2 vec2;
      vec2.x = mesh->mTextureCoords[0][i].x;
      vec2.y = mesh->mTextureCoords[0][i].y;
      vertex.TexCoords = vec2;
    }
    else
      vertex.TexCoords = Vector2(0.0f, 0.0f);

    vertices.push_back(vertex);


  }
  // process indices
  for (unsigned int i = 0; i < mesh->mNumFaces; i++)
  {
    aiFace face = mesh->mFaces[i];
    for (unsigned int j = 0; j < face.mNumIndices; j++)
      indices.push_back(face.mIndices[j]);
  }

    // process material
    if (mesh->mMaterialIndex >= 0)
    {
      if (mesh->mMaterialIndex >= 0)
      {
        aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
        std::vector<std::shared_ptr<IZE::Texture>> diffuseMaps = loadMaterialTextures(material,
          aiTextureType_DIFFUSE, "diffuse");

        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

        std::vector<std::shared_ptr<IZE::Texture>> specularMaps = loadMaterialTextures(material,
          aiTextureType_SPECULAR, "specular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
      }
    }

  return Mesh(vertices, indices, textures);
}

std::vector< std::shared_ptr<IZE::Texture>> Model::loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName)
{
  std::vector<std::shared_ptr<IZE::Texture>> textures;
  for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
  {
    aiString str;
    mat->GetTexture(type, i, &str);

    std::string texturespath("./Resource/Textures/" + directory + "/");
    std::shared_ptr<IZE::Texture> texture = IZE::ResourceManager::GetInstance()->GetResource<IZE::Texture>(texturespath + str.C_Str());

    texture->m_type = typeName;
    texture->m_fileName = texturespath + str.C_Str();
    textures.push_back(texture);
  }
  return textures;
}

std::string Model::GetModelPath()
{
  return m_path;
}

std::string Model::GetModelDir()
{
  return directory;
}



DEFINE_META(ModelComponent)
{
  PARENT_META(Component);
  ADD_MEMBER(m_model,false);
  ADD_MEMBER(m_texture, true);
  ADD_MEMBER(m_shader, false);
  ADD_MEMBER(m_path, true);

}


ModelComponent::ModelComponent() : Component(ComponentID::ModelComponent,"ModelComponent",true)
{

}

ModelComponent::ModelComponent(const ModelComponent& rhs) : Component(ComponentID::ModelComponent, "ModelComponent", true),
m_model(rhs.m_model),
m_shader(rhs.m_shader),
m_path(rhs.m_path)
{
}

void ModelComponent::Init()
{
  LoadMesh(m_path);
  SetTexturePath(m_texture);
}

void ModelComponent::Deinit()
{

}

void ModelComponent::Update()
{
 // m_model->Draw(m_shader);
}

void ModelComponent::Draw()
{
  m_model->Draw(m_shader);
}


void ModelComponent::SetTexturePath(const std::string& path)
{
  if (path == "")
    return;
  m_texture = path;
  m_model->GetMeshes().front().textures.clear();
  
  m_model->GetMeshes().front().textures.push_back(g_resource->GetResource<IZE::Texture>(m_texture));
}

Component* ModelComponent::Clone()
{
  return new ModelComponent(*this);
}

void ModelComponent::LoadMesh(const std::string & path)
{
  m_path = path;
  m_model = IZE::ResourceManager::GetInstance()->GetResource<Model>(path);
}


void ModelComponent::SetShader(std::shared_ptr<Shader> shader)
{
  m_shader = shader;
}