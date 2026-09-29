/*------------------------------
| File: BodySleepMode2D.h
| Author: Chandler Mays
------------------------------*/
#pragma once

// Settings for a RigidBody2D's initial sleep state

namespace CE
{
	enum class BodySleepMode2D
	{
		NeverSleep,			//
		StartAwake,			// 
		StartAsleep			// 
	};
}