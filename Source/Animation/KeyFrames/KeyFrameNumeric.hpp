#ifndef _NIMA_KEYFRAMENUMERIC_HPP_
#define _NIMA_KEYFRAMENUMERIC_HPP_

#include "KeyFrameWithInterpolation.hpp"

namespace nima
{
	class BlockReader;
	class ActorNode;

	class KeyFrameNumeric : public KeyFrameWithInterpolation
	{
		typedef KeyFrameWithInterpolation Base;
		private:
			float m_Value;

		public:
			KeyFrameNumeric();
			float value() const;

			bool read(BlockReader* reader, ActorNode* node) override;
			void apply(ActorNode* node, float mix) override;
			void applyInterpolation(ActorNode* node, float time, KeyFrame* toFrame, float mix) override;
			
		protected:
			virtual void setValue(ActorNode* node, float value, float mix) = 0;
	};
}

#endif