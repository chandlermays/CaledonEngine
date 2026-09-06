/*------------------------------
| File: IRenderable.h
| Author: Chandler Mays
------------------------------*/
#pragma once

namespace CE
{
	class IRenderable
	{
	public:
		virtual ~IRenderable() = default;
		virtual void Render() = 0;
	};
}