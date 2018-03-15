/***************************************************/
/* Nom:	ActionTable.h
/* Description: ActionTable
/* Auteur: BRUNEAU Colin
/***************************************************/
#ifndef __ActionTable_H_
#define __ActionTable_H_

#include "Core\Component.h"

namespace crea
{
#define MERGE2CONDITIONS(condition1, condition2) (condition1 << 8) | condition2 // A condition is given as 8 bits (char)
#define CREATE_KEY(condition, action) (condition << 16) | action	// Then the condition and action (short) are combined

	class CREAENGINE_API ActionAnimInfo
	{
	public:
		string	szAnimFileName;
		string	szActionDesc;
	};

	class CREAENGINE_API MapConditionAction : public std::multimap<long, ActionAnimInfo*> {};

	class CREAENGINE_API ActionTable : public Component
	{
		crea::GameManager*	m_pGM;

	protected:
		MapConditionAction	m_condActionMap;

	public:
		ActionTable();
		virtual ~ActionTable();

		void		read();
		string*		getAnimation(
			unsigned char _ucAnimCond1,
			unsigned char _ucAnimCond2,
			unsigned short _unAction,
			string* _pszActionDesc = nullptr);
		string*		getAnimation(
			unsigned short _unAnimCond,
			unsigned short _unAction,
			string* _pszActionDesc = nullptr);
		bool		addAnimation(
			unsigned short _unAnimCond,
			unsigned short _unAction,
			string* _pszAnimFileName,
			string* _pszActionDesc = nullptr);

		bool		loadFromFileJSON(string& _filename);

		virtual bool init();
		virtual bool update();
		virtual bool draw();
		virtual bool quit();

		virtual Component* clone() { return this; } // CB: no need to clone the ActionTable!
	};

} // namespace crea

#endif
