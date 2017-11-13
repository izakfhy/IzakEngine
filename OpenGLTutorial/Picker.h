#pragma once

class GameObject;
class Picker
{
  GameObject* m_curObj;
  static Picker* m_instance;

public:
  Picker();
  ~Picker();
  static Picker* GetInstance();

  GameObject* GetPickedObj();
  void ReadPixelPicking(unsigned FrameBuffer);
};