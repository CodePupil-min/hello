/////////////////////////////////////////////////////////////////////////////////
//
//
// �������
//
/////////////////////////////////////////////////////////////////////////////////
#ifndef _LIST_H_
#define _LIST_H_
//
#include <Windows.h>
#include "CommonAPI.h"
/////////////////////////////////////////////////////////////////////////////////
//
//
struct Weapon{
	char	szName[128];		// ��������
	float	fPosX, fPosY;		// ��������
	float	fSpeedX, fSpeedY;	// X��Y�������ٶ�
	int		iHp;				// ����ֵ
	int		iDir;				// ����0 - �Ϸ���1 - �ҷ���2 - �·���3 - ��
	int		iType;				// ���ͣ� 0 - �ҷ�̹�ˣ�1 - �з�̹�ˣ�2 - �ҷ��ӵ��� 3 - �з��ӵ�
	float	fFireTime;			// �з��Ŀ���ʱ��
	float	fRotateTime;		// �з�������һ�ε�ת�����ʣ��ʱ��
	Weapon* pNext;				// ָ����һ���ڵ��ָ��
};

enum  Role
{
	MYalien		=	0,		// �ҷ�
	ENEMYalien	=	1,		// �з�
	BOSS		=	2,		//boss
	MYBULLET	=	3,		// �ҷ��ӵ�
	ENEMYBULLET	=	4,		// �з��ӵ�
	BOSSBULLET	=	5,		//boss�ӵ�
	MAP			=	6,
};

enum  Direction
{
	UP		=	0,		// �Ϸ�
    RIGHT	=	1,		// �ҷ�
	DOWN	=	2,		// �·�
	LEFT	=	3		// ��
};

extern	Weapon *g_pHead;

// ���������
extern int	GetListSize();								// ���ص�ǰ����ڵ�����

extern Weapon	*FindNode(const char *szName );					// �������ֻ�ȡSprite
//
extern void	AddToList( Weapon *pSprite );				// ���һ��Sprite��������
//
extern void			DeleteNode( const char *szName );		// ��������ɾ��Sprite.
//
extern void			DeleteList(  );							// ɾ������Sprite.

#endif // _LIST_H_
