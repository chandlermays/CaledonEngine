#pragma once

namespace CE
{
	class Window;

	class Renderer
	{
	public:
		virtual ~Renderer() = default;								// Destructor
		Renderer(const Renderer&) = delete;							// Prevent copy-construction
		Renderer& operator=(const Renderer&) = delete;				// Prevent copy-assignment
		Renderer(Renderer&&) = delete;								// Prevent move-construction
		Renderer& operator=(Renderer&&) = delete;					// Prevent move-assignment

		virtual bool Initialize(Window* pWindow) = 0;				// Prepares the Renderer for use
		virtual void Shutdown() = 0;								// Cleans up the Renderer
		virtual void Render() = 0;									// Executes the rendering process

		virtual void* GetNativeHandle() const = 0;					// Retrieves the native Renderer handle
	};
}