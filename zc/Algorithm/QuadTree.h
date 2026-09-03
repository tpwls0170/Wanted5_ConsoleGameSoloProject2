#pragma oncermse
#include <vector>
#include <Math/Vector2.h>
#include <memory>

class Actor;
namespace Craft
{
	class QuadTree
	{
		struct Rect
		{
			float x;
			float y;
			float width;
			float height;
		};

		struct Node
		{
			// 이 Node가 담당하는 공간
			Rect Bounds;

			// 나중에 이 공간 안에 들어있는 객체들
			std::vector<Actor*> actors;

			// 나중에 4개로 쪼개졌을때
			std::unique_ptr<Node> children[4];
		};

	public:
	private:
		std::unique_ptr<QuadTree> root;
	};
}


