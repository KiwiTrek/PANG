#ifndef __MODULE_H__
#define __MODULE_H__

#include "Globals.h"

class Module
{
public:

	//Called at the beginning of the application execution
	virtual bool Init();

	//Called when the module is activated
	//By now we will consider all modules to be permanently active
	virtual bool Start();

	//Called at the beginning of each application loop
	virtual UPDATE_STATUS PreUpdate();

	//Called at the middle of each application loop
	virtual UPDATE_STATUS Update();

	//Called at the end of each application loop
	virtual UPDATE_STATUS PostUpdate();

	//Called at the end of the application
	virtual bool CleanUp();
};

#endif // __MODULE_H__