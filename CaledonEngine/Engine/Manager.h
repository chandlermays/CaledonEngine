#pragma once

namespace CE
{
	class Manager
	{
	public:
		virtual ~Manager() {}

		virtual bool Initialize() = 0;

		virtual void Update(float) {}
		virtual void Render() {}

		virtual void Shutdown() = 0;
	};
}