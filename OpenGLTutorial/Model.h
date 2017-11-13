#pragma once
#include <vector>
#include <string>

#include "Shader.h"
#include "Mesh.h"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "MetaData.h"
#include "Component.h"

class aiNode;
class aiScene;
class aiMaterial;
class aiMesh;

class Model : public IZE::Resource
{
public:
  Model(){}
  /*  Functions   */
  Model(const std::string& path) : m_path(path)
  {
    loadModel(path);
  }
  ~Model();

  void Draw(std::shared_ptr<Shader> shader);
  
  //inherited virtuals
  virtual bool Initialize(const std::string& path) override final;

  std::vector<Mesh>& GetMeshes();
  std::string GetModelPath();
  std::string GetModelDir();

  META_DATA(Model);

private:
  /*  Model Data  */
  std::vector<Mesh> meshes;
  std::string directory = "";
  std::string m_path = "";
  /*  Functions   */
  bool loadModel(std::string path);
  void processNode(aiNode *node, const aiScene *scene);
  Mesh processMesh(aiMesh *mesh, const aiScene *scene);
  std::vector<std::shared_ptr<IZE::Texture>> loadMaterialTextures(aiMaterial *mat, aiTextureType type,
    std::string typeName);
};

class ModelComponent : public Component
{
  std::shared_ptr<Model> m_model;
  std::shared_ptr<Shader> m_shader;
  std::string m_path = "";
  std::string m_texture = ""; //single texture path
public:
  ModelComponent();
  ModelComponent(std::shared_ptr<Shader> shader);
  ModelComponent(const ModelComponent& rhs);
  void LoadMesh(const std::string & path);
  void SetShader(std::shared_ptr<Shader> shader);
  void Draw();
  void SetTexturePath(const std::string& path);
  //virtuals
  virtual void Init() override;
  virtual void Deinit() override;
  virtual void Update() override;
  virtual Component* Clone() override;

  META_DATA(ModelComponent);
};