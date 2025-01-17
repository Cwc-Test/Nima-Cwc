#ifndef _NIMA_GAMEACTOR_HPP_
#define _NIMA_GAMEACTOR_HPP_

#include <nima/Actor.hpp>
//#include "Graphics/Renderer2D.hpp"
typedef  void Texture;
typedef  void GraphicsBuffer;
typedef  void Renderer2D;


namespace nima
{
	class GameActor;
	class GameActorInstance;

	class GameActorImage : public ActorImage
	{
		friend class GameActor;
		friend class GameActorInstance;
		typedef ActorImage Base;
		private:
			// The deform vertex buffer will contain only the deform translation of the vertices.
			//GraphicsBuffer* m_DeformVertexBuffer;
			//GraphicsBuffer* m_VertexBuffer;
			void* m_DeformVertexBuffer;
			void* m_VertexBuffer;
			
			
			int m_IndexOffset;

		public:
			GameActorImage();
			~GameActorImage();
			void copy(GameActorImage* node, Actor* resetActor);

			ActorNode* makeInstance(Actor* resetActor);
			void render(GameActorInstance* gameActorInstance, void* renderer);
	};

	class GameActorController
	{
		public:
			virtual ~GameActorController(){}
			virtual void onAdded(GameActorInstance* actorInstance) = 0;
			virtual void onRemoved(GameActorInstance* actorInstance) = 0;
			virtual void advance(GameActorInstance* actorInstance, float seconds) = 0;
	};

	class GameActor : public Actor
	{
		friend class GameActorImage;
		typedef Actor Base;
		private:
			Texture** m_Textures;
			GraphicsBuffer* m_VertexBuffer;
			GraphicsBuffer* m_SkinnedVertexBuffer;
			GraphicsBuffer* m_IndexBuffer;
			GraphicsBuffer* m_SequenceUVBuffer;

		protected:
			ActorImage* makeImageNode() override;
			void dispose() override;
		public:
			GameActor();
			~GameActor();
			void initializeGraphics(void* renderer);
			Actor* makeInstance() const override;
	};

	class GameActorInstance : public Actor
	{
		typedef Actor Base;
		friend class GameActor;
		private:
			GameActorInstance(const GameActor* gameActor);
			const GameActor* m_GameActor;
			std::vector<GameActorController*> m_Controllers;

		public:
			~GameActorInstance();
			void initializeGraphics(void* renderer);
			void render(void* renderer);
			const GameActor* gameActor();
			
			template<class ControllerType, typename... ControllerArguments>
			ControllerType* addController(ControllerArguments... args)
			{
				static_assert(std::is_base_of<GameActorController, ControllerType>::value, "ControllerType not derived from GameActorController");
				ControllerType* controller = new ControllerType(args...);
				m_Controllers.push_back(controller);
                controller->onAdded(this);
				return controller;
			}
			void removeController(GameActorController* controller);
			
			void advance(float elapsedSeconds) override;

		protected:
			void updateVertexDeform(ActorImage* image) override;
	};
}

#endif
