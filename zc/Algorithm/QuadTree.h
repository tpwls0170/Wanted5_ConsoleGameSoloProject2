#pragma once
#include <vector>
#include <Math/Vector2.h>
#include <memory>
#include <Actor/Actor.h>

#include <Windows.h>
#include <iostream>
namespace Craft
{
	struct Rect
	{
		float x;
		float y;
		float width;
		float height;

		bool Contains(const Vector2& point)const
		{
			if (point.x >= x && point.x <= x + width
				&& point.y >= y && point.y <= y + height)
			{
				return true;
			}

			return false;
		}

		bool Intersects(const Rect& other) const
		{
			return x + width >= other.x &&
				x <= other.x + other.width &&
				y + height >= other.y &&
				y <= other.y + other.height;
		}
	};

	class QuadTree
	{
		struct Node
		{
			// 이 Node가 담당하는 공간
			Rect Bounds;

			// 나중에 이 공간 안에 들어있는 객체들
			std::vector<Actor*> actors;

			// 나중에 4개로 쪼개졌을때
			std::unique_ptr<Node> childrens[4];

			static constexpr int Capacity = 4;
			static constexpr int ChildCount = 4;

			void split()
			{
				childrens[0] = std::make_unique<Node>();
				childrens[1] = std::make_unique<Node>();
				childrens[2] = std::make_unique<Node>();
				childrens[3] = std::make_unique<Node>();

				float halfWidth = Bounds.width / 2;
				float halfHeight = Bounds.height / 2;
				float x = Bounds.x;
				float y = Bounds.y;
				childrens[0]->Bounds = {
						x ,
						y ,
						halfWidth,
						halfHeight
				};

				childrens[1]->Bounds = {
					x + halfWidth ,
					y ,
					halfWidth,
					halfHeight
				};

				childrens[2]->Bounds = {
					x ,
					y + halfHeight,
					halfWidth,
					halfHeight
				};

				childrens[3]->Bounds = {
					x + halfWidth,
					y + halfHeight,
					halfWidth,
					halfHeight
				};

				for (Actor* actor : actors)
				{
					InsertIntoChildren(actor);
				}

				actors.clear();
			}

			bool InsertIntoChildren(Actor* actor)
			{
				for (int i = 0; i < ChildCount; ++i)
				{
					if (childrens[i]->Bounds.Contains(actor->GetPosition()))
					{
						return childrens[i]->Insert(actor);
					}
				}

				return false;
			}

			bool Insert(Actor* actor)
			{
				// Bunds 안에 있는지 검사
				if (!Bounds.Contains(actor->GetPosition()))
				{
					return false;
				}
				
				if (childrens[0] != nullptr)
				{
					return InsertIntoChildren(actor);
				}

				if (actors.size() < Capacity)
				{
					// Capacity보다 적으면 actors에 추가
					actors.emplace_back(actor);
					// 추가 성공하면 true
					return true;
				}

				// 꽉 찼으면 분할한다.
				split();
				return InsertIntoChildren(actor);
			}

			void Query(const Rect& area, std::vector<Actor*>& result)
			{
				if (!Bounds.Intersects(area))
				{
					return;
				}

				for (Actor* actor : actors)
				{
					if (area.Contains(actor->GetPosition()))
					{
						result.emplace_back(actor);
					}
				}

				for (int i = 0; i < ChildCount; ++i)
				{
					if (childrens[i] != nullptr)
					{
						childrens[i]->Query(area, result);
					}
				}
			}
		};

	public:
		QuadTree(const Rect& bounds);
		bool Insert(Actor* actor);
		std::vector<Actor*> Query(const Rect& area);
	private:
		std::unique_ptr<Node> root;
	};
}