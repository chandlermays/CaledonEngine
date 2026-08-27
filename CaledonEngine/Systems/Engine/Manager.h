/*------------------------------
| File: Manager.h
| Author: Chandler Mays
------------------------------*/
#pragma once

namespace CE
{
	class Manager
	{
	public:
		virtual ~Manager() {}

		virtual bool Initialize() = 0;
		virtual void Shutdown() = 0;
	};
}