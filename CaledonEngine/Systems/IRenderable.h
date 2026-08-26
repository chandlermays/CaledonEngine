namespace CE
{
	class IRenderable
	{
		virtual ~IRenderable() = default;
		virtual void Render() = 0;
	};
}