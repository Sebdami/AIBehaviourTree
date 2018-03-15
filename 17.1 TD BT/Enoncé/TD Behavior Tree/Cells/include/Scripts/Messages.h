/***************************************************/
/* Nom:	Messages.h
/* Description: Messages
/* Auteur: BRUNEAU Colin
/***************************************************/
#ifndef __Messages_H_
#define __Messages_H_

//Add new messages here
typedef enum {
	MSG_NULL,
	MSG_Randomize,
	MSG_Timeout,
	MSG_ChangeState,
	MSG_Reset,
	MSG_Kill,
	MSG_Stop,
	MSG_GoToWithGold,
	MSG_GoToWithLumber,
	MSG_GoTo,
	MSG_Build,
	MSG_Mine,
	MSG_Harvest,
	MSG_Die,
	MSG_Boost,
	MSG_Hit,
	MSG_HitStop,
	MSG_Seek,
	MSG_Flee,
	MSG_Pursuit,
	MSG_Evasion,
	MSG_Arrival,
	MSG_Wander,
	MSG_PathFollowing,
	MSG_UCA
} MSG_Name;

#endif
