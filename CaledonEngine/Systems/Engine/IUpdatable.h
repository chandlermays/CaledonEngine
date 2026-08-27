#pragma once

namespace CE
{
	class IUpdatable
	{
	public:
		virtual ~IUpdatable() = default;
		virtual void Update(float deltaTime) = 0;
	};
}