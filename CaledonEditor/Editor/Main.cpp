/*------------------------------
| File: Main.cpp
| Author: Chandler Mays
------------------------------*/
#include "Editor.h"

int main()
{
	Editor editor;

	if (!editor.Initialize())
		return -1;

	editor.Run();

	return 0;
}