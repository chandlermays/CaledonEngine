namespace CE
{
	class IUpdatable
	{
		virtual ~IUpdatable() = default;
		virtual void Update(float deltaTime) = 0;
	};
}