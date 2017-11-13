/******************************************************************************/
/*!
\file	  SystemComponentNames.h
\author Izak Foong, foong.h, 440002413
\par    foong.h\@digipen.edu
\date   12/05/2014
\brief
The messanging system that stores function within a given event and calls this
function with the message send to the event

All content © 2015 DigiPen Institute of Technology Singapore. All Rights Reserved
*/
/******************************************************************************/

// Macro entry name MUST BE EXACTLY THE SAME AS class name!
// System Components will be constructed in the same order as declared here!

REGISTER_SYSTEM_COMPONENT_NAME(Input)
//REGISTER_SYSTEM_COMPONENT_NAME(Collision)
//REGISTER_SYSTEM_COMPONENT_NAME(Physics)
//REGISTER_SYSTEM_COMPONENT_NAME(Audio)
REGISTER_SYSTEM_COMPONENT_NAME(ComponentGM)
REGISTER_SYSTEM_COMPONENT_NAME(ResourceManager)
REGISTER_SYSTEM_COMPONENT_NAME(Factory)
REGISTER_SYSTEM_COMPONENT_NAME(GraphicsEngine)


/*REGISTER_SYSTEM_COMPONENT_NAME(GameStateManager)
REGISTER_SYSTEM_COMPONENT_NAME(ThreadPool)
REGISTER_SYSTEM_COMPONENT_NAME(LuaInterface)


REGISTER_SYSTEM_COMPONENT_NAME(AudioSystem)
REGISTER_SYSTEM_COMPONENT_NAME(Factory)
REGISTER_SYSTEM_COMPONENT_NAME(Editor)

REGISTER_SYSTEM_COMPONENT_NAME(TextBoxManager)
REGISTER_SYSTEM_COMPONENT_NAME(Profiler)*/
