/***************************************************/
/* Nom:	BehaviorTree.h
/* Description: BehaviorTree
/* Auteur: BRUNEAU Colin
/***************************************************/
#ifndef __BehaviorTree_H_
#define __BehaviorTree_H_

#include "Core\Component.h"

namespace crea
{
	// Predefinitions
	class Behavior;

	class CREAENGINE_API BehaviorTree : public Component
	{
	protected:
		Behavior* m_pRoot;

	public:
		BehaviorTree();
		virtual ~BehaviorTree();

		void setRootBehavior(Behavior* _pRoot);

		virtual bool init();
		virtual bool update();
		virtual bool draw();
		virtual bool quit();

		virtual Component* clone()
		{
			BehaviorTree* p = new BehaviorTree(*this);
			// CB: copy tree structure...
			p->m_pRoot = m_pRoot->clone();
			return p;
		}
	};

} // namespace crea

#endif
