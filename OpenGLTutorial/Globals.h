#pragma once

#define g_input IZE::Input::SingletonPtr
#define g_graphics GraphicsEngine::SingletonPtr
#define g_camera GraphicsEngine::SingletonPtr->m_camera
#define g_resource IZE::ResourceManager::GetInstance()
#define g_CompGM ComponentGM::SingletonPtr
#define g_factory Factory::SingletonPtr
#define g_engine IZE::IZEngine::SingletonPtr