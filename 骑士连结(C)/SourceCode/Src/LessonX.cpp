/////////////////////////////////////////////////////////////////////////////////
//
//
//
//
/////////////////////////////////////////////////////////////////////////////////
#include <Stdio.h>
#include <string.h>
#include "CommonAPI.h"
#include "LessonX.h"
#include "list.h"





float		g_fScreenLeft=0.f;    // ��Ļ��߽�ֵ
float		g_fScreenRight=0.f;    // ��
float		g_fScreenTop=0.f;    // ��
float		g_fScreenBottom=0.f;    // ��

float		g_fSpeedUp = 0.f;
float		g_fSpeedDown = 0.f;
float		g_fSpeedLeft = 0.f;
float		 g_fSpeedRight = 0.f;


Weapon*		g_pMypeople		=	NULL;
Weapon*		g_pMypeople1	=	NULL;	// �ҷ�
Weapon*		g_pBoss			=	NULL;	//boss

int			g_iLevel		=	1;		//�ؿ�
int			g_iSoundID		=	0;		//����ID
int			g_iSoundID2		=	0;		//�ҷ���ǹ
int			g_iSoundID3		=	0;		//boss��ǹ
bool		g_bStart		=	false;  // ����һ����Ϸ��ʼ�����
int			g_iScore		=	0;		// һ����Ϸ�÷�
float		g_fGameTime		=	100.f;    // һ����Ϸ��ʣ��ʱ��
int			g_iWallCount	=	0;		// ��¼ǽ������
int			g_iMap[11][13];				// ��ͼ����
int			g_iNum			=	0;		// ��¼�ҷ��ӵ����ҷ����з�������
float		g_fCreateTime	=	2.f;	// ÿ�������ɵ�ʣ��ʱ��
float		g_fSleepTime	=	3.f;	//�ؿ�֮��ļ��ʱ��


const float	FIRE_TIME		=  2.f;		// �з������ڵ���ʱ����
const float	BOSS_TIME		=  4.f;
const float CREATE_alien_TIME = 2.5f;		// ÿ�������ɵ�ʱ����
const float	GAME_TIME		=	100.f;	// һ����Ϸ��ʱ��
const float	alien_SPEED		 = 5.f;     // �з��ٶ�
const float BULLET_SPEED	 = 20.f;		// �ӵ��ٶ�
const float ROTATE_TIME		 = 2.f;     // �з���ת�����ʱ��

//�Ƿ�����С��
bool crene=true;
//����Ƿ�����
bool playerDead = false;
//����Ƿ�ʤ��
bool isWon=false;
//��ʷ��������
int		rank[3]={0,0,0};

// ��������

void	MoveMyalien(int, bool);
void	LoadMap();
void	OnFire(char*, int, const float fMouseX, const float fMouseY);
void	CreateEnemyaliens();
void	CreateEnemy2aliens();
void	MoveEnemyalien(const char*);
bool	IsDead(const char*);
void	CreateBoss();
void	OnDeleteAllAlien();
void	AfterDead(const char* szName);
void	skill_1();
void	skill_2();
void	skill_3();
void	skill_4();

void	WriteScore(const int x);
void	ReadScore();
// ��ײ����

void	OnMyalienColOther(const char*, const char*);
void	OnBulletColOther(const char*, const char*);
void	OnWallColOther(const char*, const char*);
void	OnalienColOther(const char*, const char*);
void	OnGoalColOther();

//UI����
void OnClickGo(const float fMouseX, const float fMouseY);
void paintUI(float fTime);
void CleanSprite(const char *szName);

//==============================================================


//���Ƶ�ͼ
#define	XBlock 32
#define YBlock 23
int mapitem[4];
int mapall[XBlock][YBlock];
char mapcontent[XBlock*YBlock][64];
int	iLoopX = 0, iLoopY = 0, iLoop = 0;
int ibasis;
int pIndex;
float sLeft=0;
float sTop=-0;
// 	�������С���ڱ༭����ڷŵĵ�һ�鷽�����ʼ����
const float	g_fBlockStartX	= sLeft+15.5;
const float	g_fBlockStartY	= sTop-4.6875;
// 	��Ļ�߶�75 / 4�� = 18.75ÿ��Ĵ�С.�༭����Ԥ�Ȱڷźõķ����͸�  //	�������ֵһ��
const float	g_fBlockSize1 = 3.125;
const float	g_fBlockSize2 = 4.6875;
////////////////////////////////////////////////////////////////////////////////
//
//
int			g_iGameState		=	0;		// ��Ϸ״̬��0 -- ��Ϸ�����ȴ���ʼ״̬��1 -- ���¿ո����ʼ����ʼ����Ϸ��2 -- ��Ϸ������
//
void		GameInit();
void		GameRun( float fTimeDelta );
void		GameEnd();
int XYToOneIndex( const int iIndexX, const int iIndexY );
void PaintMap(int decide1,int decide2,int decide3,int decide4);
//==============================================================================
//
// ����ĳ�������Ϊ��GameMainLoop����Ϊ��ѭ��������������ÿ֡ˢ����Ļͼ��֮�󣬶��ᱻ����һ�Ρ�


//==============================================================================
//
// ��Ϸ��ѭ�����˺���������ͣ�ĵ��ã�����ÿˢ��һ����Ļ���˺�����������һ��
// ���Դ�����Ϸ�Ŀ�ʼ�������С������ȸ���״̬. 
// ��������fDeltaTime : �ϴε��ñ��������˴ε��ñ�������ʱ��������λ����
void GameMainLoop( float	fDeltaTime)
{
	paintUI(fDeltaTime);
	switch( g_iGameState )
	{
		// ��ʼ����Ϸ�������һ���������
		
	case 1:
		{
			GameInit();
			g_iGameState	=	2; // ��ʼ��֮�󣬽���Ϸ״̬����Ϊ������
		}
		break;

		// ��Ϸ�����У����������Ϸ�߼�
	case 2:
		{
			// TODO �޸Ĵ˴���Ϸѭ�������������ȷ��Ϸ�߼�
			if( true )
			{
				GameRun( fDeltaTime );
			}
			else
			{
				// ��Ϸ������������Ϸ���㺯����������Ϸ״̬�޸�Ϊ����״̬
				g_iGameState	=	0;
				GameEnd();
			}
		}
		break;

		// ��Ϸ����/�ȴ����ո����ʼ
	case 0:
	default:
		break;
	}
	
}

//==============================================================================
//
// ÿ�ֿ�ʼǰ���г�ʼ���������һ���������
void GameInit()
{
	DeleteList();
	g_iLevel		=	1;
	g_iScore		=	0;
	g_pBoss			=	NULL;
	g_pMypeople		=	NULL;
	g_fGameTime		=	GAME_TIME;
	g_fCreateTime	=	0;
	g_iNum			=	0;	
	playerDead = false;
	crene=true;
	isWon=false;


	g_fScreenLeft=dGetScreenLeft();    // ��Ļ��߽�ֵ
	g_fScreenRight=dGetScreenRight();    // ��
	g_fScreenTop=dGetScreenTop();    // ��
	g_fScreenBottom=dGetScreenBottom();    // ��
	PaintMap(1,2,3,4);
	
	g_pMypeople = (Weapon*)malloc(sizeof(Weapon));
	strcpy(g_pMypeople->szName, "player");
	dCloneSprite("playerA",g_pMypeople->szName);
	g_pMypeople->iHp = 20;
	dSetSpritePosition(g_pMypeople->szName,-3,23);//�������λ��
	dSetSpriteWorldLimit(g_pMypeople->szName, WORLD_LIMIT_NULL, g_fScreenLeft+3.125, g_fScreenTop+4.6875, g_fScreenRight-3.125, g_fScreenBottom-4.6875); // �����������߽�
	dSetSpriteCollisionSend(g_pMypeople->szName, true);

	dSetSpritePosition("alien1", g_fScreenRight+10, g_fScreenTop-10);
	dSetSpritePosition("alien2", g_fScreenRight+10, g_fScreenTop-10);
	dSetSpritePosition("alien3", g_fScreenRight+10, g_fScreenTop-10);
	dSetSpriteLinearVelocity("alien1", 0, 0);
	dSetSpriteLinearVelocity("alien2", 0, 0);
	dSetSpriteLinearVelocity("alien3", 0, 0);
	

}
//==============================================================================
//
// ÿ����Ϸ������
void GameRun( float fDeltaTime )
{
	
		if(g_bStart)
		{
			//ÿ��һ��ʱ������з�Ŀ��
			g_fCreateTime-=fDeltaTime;
			if(g_fCreateTime<0.f&&crene)
			{
				for(int i=0;i<1;i++)
					CreateEnemyaliens();
				g_fCreateTime=CREATE_alien_TIME;
				g_fCreateTime-=(g_iLevel-1)/2;//ˢ�ּ�������̣�1 2.5�룻2 2�룻3 1.5�롣
			}

			//ÿ������boss
			g_fGameTime-=fDeltaTime;
			if(g_fGameTime<0.f&&g_pBoss==NULL)
			{
				crene=false;
				CreateBoss();
				g_fGameTime=GAME_TIME;
			}

			Weapon* pNode	= g_pHead;
			int num = GetListSize();  // ��ǰ����Ľڵ�����
			// ������ǰ����Ѱ�ҵз�����ڵ�
			for(int i=0; i<num; i++)
			{
				if(pNode->iType == ENEMYalien||pNode->iType == BOSS)  // �з�����
				{
					if(pNode->iHp != 0)
					{
						float fPosX=dGetSpritePositionX(g_pMypeople->szName);
						float fPosY=dGetSpritePositionY(g_pMypeople->szName);
						
						pNode->fFireTime -= fDeltaTime;
						if(pNode->fFireTime < 0.f)
						{
							if(pNode->iType == ENEMYalien)
							{
								pNode->fFireTime = FIRE_TIME;
								OnFire(pNode->szName, ENEMYBULLET, fPosX, fPosY);
							}
							else if(pNode->iType == BOSS)
							{
								pNode->fFireTime = BOSS_TIME;
								int ibasis = dRandomRange(1,90);
								if(g_iLevel==1)
								{
									if(ibasis<=50)
										OnFire(pNode->szName, BOSSBULLET, fPosX, fPosY);									
									else if(ibasis<=90)
										skill_1();							
								}else if(g_iLevel==2)
								{
									if(ibasis<=50)
										OnFire(pNode->szName, BOSSBULLET, fPosX, fPosY);
									else if(ibasis<=70)									
										skill_2();
									else									
										skill_3();
								}else if(g_iLevel==3)
								{
									if(ibasis<=60)
										OnFire(pNode->szName, BOSSBULLET, fPosX, fPosY);
									else if(ibasis<=90)
										skill_4();
								}
							}
						}
					}
				}
				pNode = pNode->pNext;
			}
			
		}
}
//==============================================================================
//
// ������Ϸ����
void GameEnd()
{
}
//==========================================================================
//
// ����ƶ�
// ���� fMouseX, fMouseY��Ϊ��굱ǰ����
void OnMouseMove( const float fMouseX, const float fMouseY )
{
	
}
//==========================================================================
//
// �����
// ���� iMouseType����갴��ֵ���� enum MouseTypes ����
// ���� fMouseX, fMouseY��Ϊ��굱ǰ����
void OnMouseClick( const int iMouseType, const float fMouseX, const float fMouseY )
{

	if(iMouseType == MOUSE_LEFT)//��⵽����������
	{
		//��Ϸ����ʱ
		if(g_bStart)
			OnFire(g_pMypeople->szName, MYBULLET, fMouseX, fMouseY);//�򿪻����������������Ϣ�������λ�ã����Ҽ�¼���ӵ����������
		//δ����ʱִ����������
		else
			OnClickGo(fMouseX,fMouseY);
	}
}
//==========================================================================
//
// ��굯��
// ���� iMouseType����갴��ֵ���� enum MouseTypes ����
// ���� fMouseX, fMouseY��Ϊ��굱ǰ����
void OnMouseUp( const int iMouseType, const float fMouseX, const float fMouseY )
{
	
}
//==========================================================================
//
// ���̰���
// ���� iKey�������µļ���ֵ�� enum KeyCodes �궨��
// ���� iAltPress, iShiftPress��iCtrlPress�������ϵĹ��ܼ�Alt��Ctrl��Shift��ǰ�Ƿ�Ҳ���ڰ���״̬(0δ���£�1����)
void OnKeyDown( const int iKey, const bool bAltPress, const bool bShiftPress, const bool bCtrlPress )
{	
	if((playerDead||isWon)&&iKey==KEY_SPACE)
	{
		g_iGameState	=	0;
		WriteScore(g_iScore);
		DeleteList();
	}
	if(iKey == KEY_W || iKey == KEY_A || iKey == KEY_S || iKey == KEY_D )
	{
		switch(iKey)
		{
		case KEY_W:		
			g_fSpeedUp = -25.f;
			//g_pMypeople->iDir = UP;
			break;
		case KEY_A:
			g_fSpeedLeft = -25.f;
			//	g_pMypeople->iDir = LEFT;
			break;
		case KEY_S:	
			g_fSpeedDown = 25.f;
			//	g_pMypeople->iDir = DOWN;
			break;
		case KEY_D:
			g_fSpeedRight = 25.f;	
			//	g_pMypeople->iDir = RIGHT;
			break;
		default:
			break;
		}
		dSetSpriteLinearVelocity("player", g_fSpeedLeft + g_fSpeedRight, g_fSpeedUp + g_fSpeedDown);
		if((g_fSpeedLeft + g_fSpeedRight) > 0)	
			dSetSpriteFlipX("player", false);	// ���ң�ͼƬ����Ҫ��ת
		else if((g_fSpeedLeft + g_fSpeedRight) < 0)
			dSetSpriteFlipX("player",true);	// ����ͼƬ��Ҫ��ת
	}
}
//==========================================================================
//
// ���̵���
// ���� iKey������ļ���ֵ�� enum KeyCodes �궨��
void OnKeyUp( const int iKey )
{
	switch(iKey)
	{
	case KEY_W:		
		g_fSpeedUp = 0.f;
		break;
	case KEY_A:
		g_fSpeedLeft = 0.f;
		break;
	case KEY_S:	
		g_fSpeedDown = 0.f;
		break;
	case KEY_D:
		g_fSpeedRight = 0.f;	
		break;
	default:
		break;
	}
	dSetSpriteLinearVelocity("player", g_fSpeedLeft + g_fSpeedRight, g_fSpeedUp + g_fSpeedDown);
	if((g_fSpeedLeft + g_fSpeedRight) > 0)	
		dSetSpriteFlipX("player", false);	// �����Σ�ͼƬ����Ҫ��ת
	else if((g_fSpeedLeft + g_fSpeedRight) < 0)
		dSetSpriteFlipX("player", true);	// �����Σ�ͼƬ��Ҫ��ת

	
}
//===========================================================================
//
// �����뾫����ײ
// ���� szSrcName��������ײ�ľ�������
// ���� szTarName������ײ�ľ�������
void OnSpriteColSprite( const char *szSrcName, const char *szTarName )
{
	
	if(strcmp(szSrcName, g_pMypeople->szName) == 0)  // �ҷ�
		OnMyalienColOther(szSrcName, szTarName);
	if(strstr(szSrcName, "bullet_") != NULL)           // �ӵ����ез�
		OnBulletColOther(szSrcName, szTarName);
	if(strstr(szSrcName, "alien_") != NULL)			  // �з�����
		OnalienColOther(szSrcName, szTarName);
	if(strstr(szSrcName, "boss_") != NULL)			  // �з�boss
		OnalienColOther(szSrcName, szTarName);

	if(strstr(szTarName, "alien_") != NULL)			  // �з�Ϊ������ײ��
		OnalienColOther(szTarName, szSrcName);
	if(strstr(szTarName, "boss_") != NULL)			  // bossΪ������ײ��
		OnalienColOther(szTarName, szSrcName);
	if(strstr(szTarName, "bullet_") != NULL)           // �ӵ�
		OnBulletColOther(szTarName, szSrcName);

	Weapon* p0 = FindNode(szSrcName);
	Weapon* p1 = FindNode(szTarName);

	if(p0 != NULL && IsDead(szSrcName))
	{
		dDeleteSprite(szSrcName);
		DeleteNode(szSrcName);
	}
	if(p1 != NULL && IsDead(szTarName))
	{
		dDeleteSprite(szTarName);
		DeleteNode(szTarName);
	}
}
//===========================================================================
//
// ����������߽���ײ
// ���� szName����ײ���߽�ľ�������
// ���� iColSide����ײ���ı߽� 0 ��ߣ�1 �ұߣ�2 �ϱߣ�3 �±�
void OnSpriteColWorldLimit( const char *szName, const int iColSide )
{
	if(strstr(szName, "alien_") != NULL)   // �з������߽��ж�
	{
		Weapon* enemyalien = FindNode(szName);
		float fSpeedX = 0.f, fSpeedY = 0.f;
		int	  iDir = 0;

		// ��ͷ
		switch(iColSide)
		{
		case 0:  // ������ߣ���ͷ����
			fSpeedX = alien_SPEED;
			iDir = RIGHT;
			break;
		case 1:  // �����ұߣ���ͷ����
			fSpeedX = -alien_SPEED;
			iDir =	LEFT;
			break;
		case 2:  // �����ϱߣ���ͷ����
			fSpeedY = alien_SPEED;
			iDir = DOWN;
			break;
		case 3:  // �����±ߣ���ͷ����
			fSpeedY = -alien_SPEED;
			iDir = UP;
			break;
		}
		dSetSpriteLinearVelocity(szName, fSpeedX, fSpeedY);
		dSetSpriteRotation(szName, iDir*90);

	}

	if((strcmp(szName, "player"))== 0)//�����ײ�߽��ж�
	{//�жϱ߽���ײ
	if(iColSide == 0)
	{
		dSetSpriteLinearVelocity(szName,0,0);
		dSetSpriteFlipX(szName, 0);
	}
	else if(iColSide == 1)
	{
		dSetSpriteLinearVelocity(szName,0,0);
		dSetSpriteFlipX(szName, 1);
	}
	else if(iColSide == 2)
	{
		dSetSpriteLinearVelocity(szName,0,0);
		dSetSpriteFlipY(szName, 0);
	}
	else if(iColSide == 3)
	{
		dSetSpriteLinearVelocity(szName,0,0);
		dSetSpriteFlipY(szName, 0);
	}
	}

}
//=========================================================================
int XYToOneIndex( const int iIndexX, const int iIndexY )
{
	return (iIndexY * XBlock + iIndexX);
}
void CleanMap()
{
	Weapon* pNode	= g_pHead;
	int num = GetListSize();  // ��ǰ����Ľڵ�����
	for(int i=0; i<num; i++)
	{
		if(pNode->iType==MAP)
		{
			dDeleteSprite(pNode->szName);
		}
	}
}
void PaintMap(int decide1,int decide2,int decide3,int decide4)
{
	//���Ƶ�ͼ
	char szName[XBlock*YBlock];
	char copyName[XBlock*YBlock];
	Weapon* map = (Weapon*)malloc(sizeof(Weapon));
	int i=4;
	for(iLoopY = 0;iLoopY<YBlock;iLoopY++)
	{
		float y=iLoopY;
		for(iLoopX = 0;iLoopX<XBlock;iLoopX++)
		{
			float x=iLoopX;
			i++;
			pIndex=XYToOneIndex(iLoopX,iLoopY);
			if(iLoopY==0||iLoopY==22||iLoopX==0||iLoopX==31)
			{
				ibasis = dRandomRange(decide3,decide4);
				sprintf(szName, "map_%d", i);
				sprintf(copyName, "map%d", ibasis);
				dCloneSprite(copyName, szName);
				strcpy( mapcontent[pIndex],szName);
				dSetSpritePosition(mapcontent[pIndex], sLeft+g_fBlockSize1*(x-15.5), sTop+g_fBlockSize1*(y-12.5)+g_fBlockSize2);
				strcpy(map->szName,  szName);
				map->iType = MAP;

				AddToList(map);
			}else
			{
				ibasis = dRandomRange(decide1,decide2);
				sprintf(szName, "map_%d", i);
				sprintf(copyName, "map%d", ibasis);
				dCloneSprite(copyName, szName);
				strcpy( mapcontent[pIndex], szName);
				dSetSpritePosition(mapcontent[pIndex], sLeft+g_fBlockSize1*(x-15.5), sTop+g_fBlockSize1*(y-12.5)+g_fBlockSize2);
				strcpy(map->szName,  szName);
				map->iType = MAP;

				AddToList(map);
			}
			
		}
	}
}

//=====================================================================
void CreateEnemyaliens()
{
	// ÿ�γ���һ�����ˣ��������ڵ�һ�У������������ǰ�治�����ϰ�
	float	fPosX, fPosY;
	float	fSpeedX, fSpeedY;
	char	szName[128];
	char	dpName[128];
	int		iDir;
	static int posx=-45;
	//if(crene){
		Weapon* alien = (Weapon*)malloc(sizeof(Weapon));

		fPosX = dRandomRange(-30, 30);  // ͳһ�����ڵ�һ��
		posx+=8;
		iDir = 1;	  // ����ͳһ����

		// ���ǰ�����ϰ�����������λ��
		int j;
		do{
			j = dRandomRange(-10, 10);
		}while(g_iMap[1][j] == 1);


		fPosY = -35;
		sprintf(dpName, "alien%d", g_iLevel);
		sprintf(szName, "alien_%d", g_iNum++);
		
		dCloneSprite(dpName, szName);
		dSetSpritePosition(szName, fPosX, fPosY);
		dSetSpriteRotation(szName, iDir*90);
		dSetSpriteLinearVelocityY(szName, alien_SPEED);
		// ������ײ
		dSetSpriteCollisionSend(szName, true);
		dSetSpriteCollisionReceive(szName, true);
		// ��������߽�
		dSetSpriteWorldLimit(szName, WORLD_LIMIT_NULL, g_fScreenLeft, g_fScreenTop, g_fScreenRight, g_fScreenBottom);

			// ��ָ��ڵ���뵽������
		strcpy(alien->szName,  szName);
		alien->iType = ENEMYalien;
		alien->iDir = iDir;
		alien->fRotateTime = ROTATE_TIME;
		alien->iHp = 1;
		alien->fFireTime = FIRE_TIME;
		alien->pNext=NULL;

		AddToList(alien);
	
}



bool IsDead(const char* szName)//�ж�����ĳһĿ��hpֵ��������
{
	Weapon* pSprite = FindNode(szName);

	if(pSprite != NULL && pSprite->iHp <= 0)
		return true;

	return false;
}

void	OnBulletColOther(const char* szBulletName, const char* szOtherName)//�ж��ӵ���ײʱʹ�ӵ�hp����
{
	Weapon * pBullet = FindNode(szBulletName);  // ���������ҳ���Ӧ���ӵ��ڵ�
	if(strcmp(szOtherName, "player")==0&&(pBullet->iType==ENEMYBULLET||pBullet->iType==BOSSBULLET))//�������
	{	
		dDeleteSprite(szBulletName);
		DeleteNode(szBulletName);
		g_pMypeople->iHp-=g_iLevel;
		if(g_pMypeople->iHp<=0){
			g_bStart=false;
			playerDead=true;
			AfterDead(g_pMypeople->szName);
			dDeleteSprite("player");
		}
	}
	
	else if(pBullet->iType == MYBULLET)
	{	
		if(strstr(szOtherName, "alien_")!=NULL)//���е���
		{	
			dDeleteSprite(szBulletName);
			DeleteNode(szBulletName);
			Weapon * palien = FindNode(szOtherName);
			if(!palien)return;
			palien->iHp-=1;
			if(palien->iHp<=0)
			{
				AfterDead(szOtherName);
				dDeleteSprite(szOtherName);	
				g_iScore+=g_iLevel*10;
			}
		}
		if(strstr(szOtherName, "boss_")!=NULL)//����boss
		{
			dDeleteSprite(szBulletName);
			DeleteNode(szBulletName);
			if(!g_pBoss)return;
			g_pBoss->iHp-=1;
			if(g_pBoss->iHp<=0)
			{
				AfterDead(g_pBoss->szName);
				OnDeleteAllAlien();//boss����ɾ������С��
				//ɾ��boss
				dDeleteSprite(szOtherName);
				g_bStart=false;//�������
				g_fGameTime=GAME_TIME;//��Ϸʱ��
				g_iScore+=g_iLevel*100;//�ӷ�
				g_pMypeople->iHp+=g_iLevel*3;//��Ѫ
				crene=true;//����ˢ��
				g_pBoss=NULL;//����״̬
				if(g_iLevel==3)isWon=true;//����ʤ��
				g_iLevel++;//�ؿ�+1
			}
		}
	}
		


}

void	OnalienColOther(const char* szalienName, const char* szOtherName)//�жϵ��˸�����ײʱ������Ĳ���
{
	Weapon * aim = FindNode(szalienName);
	if(aim->iType!=BOSS)
	{
		// ���������ң�ͣ����
		if(strcmp(szOtherName, "player") == 0)
			dSetSpriteLinearVelocity(szalienName, 0.f, 0.f);

	
		// �������Ҳ�ǵз����з�ͣ����
		if(strstr(szOtherName, "alien_") != NULL)
		{
			float fSpeedX0 = dGetSpriteLinearVelocityX(szalienName);
			float fSpeedY0 = dGetSpriteLinearVelocityY(szalienName);
			float fSpeedX1 = dGetSpriteLinearVelocityX(szOtherName);
			float fSpeedY1 = dGetSpriteLinearVelocityY(szOtherName);

			// �����Ŀ��ԭ�����ٶȣ���ͷǰ��

			if(fSpeedX0 > 0.0001)
			{
				dSetSpriteLinearVelocity(szalienName, -alien_SPEED, 0);
				dSetSpriteRotation(szalienName, 270);
			}
			else
			{
				dSetSpriteLinearVelocity(szalienName, alien_SPEED, 0);
				dSetSpriteRotation(szalienName, 90);
			}


			if(fSpeedY0 > 0.0001)
			{
				dSetSpriteLinearVelocity(szalienName, 0, -alien_SPEED);
				dSetSpriteRotation(szalienName, 0);
			}
			else
			{
				dSetSpriteLinearVelocity(szalienName, 0, alien_SPEED);
				dSetSpriteRotation(szalienName, 180);
			}

		}
	}
}


void	OnFire(char* szName, int iType, const float fMouseX, const float fMouseY)//����������Һ͵���ͨ�ã�
{
	char dpBullet[128];
	char dpsBullet[128];
	char myBullet[128];
	char szBullet[128];	
	float fPosX, fPosY;
	int iDir;
	float fSpeedX,fSpeedY;

	//��ȡÿһ�ص�С���ӵ�ģ��
	sprintf(dpBullet, "bullet%d", g_iLevel);
	//��ȡÿһ�ص�boss�ӵ�ģ��
	sprintf(dpsBullet, "sbullet%d", g_iLevel);
	//����ӵ�
	sprintf(myBullet, "mybullet");

	//�������ӵ�����
	sprintf(szBullet, "bullet_%d",g_iNum++);

	if(iType==BOSSBULLET)
	{
		dCloneSprite(dpsBullet,szBullet);
		g_iSoundID3 = dPlaySound("eneshot.ogg",0,1);
	}
	else if(iType==MYBULLET)
	{
		dCloneSprite(myBullet,szBullet);
		g_iSoundID2 = dPlaySound("shot.ogg",0,1);
	}
	else if(iType==ENEMYBULLET)
	{
		dCloneSprite(dpBullet,szBullet);
	}
	dSetSpriteWorldLimit(szBullet,WORLD_LIMIT_NULL,g_fScreenLeft+3.125f, g_fScreenTop+4.685f, g_fScreenRight-3.125f, g_fScreenBottom-4.685f);  // �����ӵ�����߽�
	// ������ײģʽ
	dSetSpriteCollisionSend(szBullet,true);
	dSetSpriteCollisionReceive(szBullet,true);

	// ��ȡ��ǰ�������ֵ
	fPosX = dGetSpritePositionX(szName);
	fPosY = dGetSpritePositionY(szName);
	float mPosX = dGetSpritePositionX("player");
	float mPosY = dGetSpritePositionY("player");
	

	//��������ǵз�
	if(iType==ENEMYBULLET)
	{
		int x = mPosX-fPosX;
		int y = mPosY-fPosY;
		if(x>0)
		{
			fPosX += dGetSpriteWidth(szName)/2 + 1;
		}else
		{
			fPosX -= dGetSpriteWidth(szName)/2 + 1;
		}
		if(y>0)
		{
			fPosY += dGetSpriteHeight(szName)/2 + 1;
		}else
		{
			fPosY -= dGetSpriteHeight(szName)/2 + 1;
		}
	}
	//boss
	else if(iType==BOSSBULLET)
	{
		fPosY += dGetSpriteHeight(szName)/2 + 2;
	}
	//me
	else if(iType==MYBULLET)
	{
		int x = fMouseX-fPosX;
		int y = fMouseY-fPosY;
		if(x>0)
		{
			fPosX += dGetSpriteWidth(szName)/2 + 1;
		}else
		{
			fPosX -= dGetSpriteWidth(szName)/2 + 1;
		}
	}
	dSetSpritePosition(szBullet, fPosX, fPosY);

	dSpriteMoveTo(szBullet,fMouseX, fMouseY, 15, false);

	// �����ӵ���ָ������������뵽������
	Weapon *pBullet = (Weapon*)malloc(sizeof(Weapon));
	pBullet->iDir = iDir;   // �ӵ��������ҳ���һ��
	sprintf(pBullet->szName, szBullet);
	pBullet->fPosX = fPosX;
	pBullet->fPosY = fPosY;
	pBullet->iType = iType;
	pBullet->iHp  = 1;
	pBullet->pNext=NULL;

	AddToList(pBullet);
	


}

// �ҷ�����������������ײ
void	OnMyalienColOther(const char* szalienName, const char* szOtherName)
{
	 if(strstr(szOtherName, "boss_") != NULL)
		 dSetSpriteLinearVelocity(g_pMypeople->szName, 0, 0);

	/*if(strstr(szOtherName, "wall") != NULL)
		dSetSpriteLinearVelocity(g_pMypeople1->szName, 0, 0);
    if(strstr(szOtherName, "stock") != NULL)
		dSetSpriteLinearVelocity(g_pMypeople1->szName, 0, 0);
    if(strstr(szOtherName, "sea") != NULL)
		dSetSpriteLinearVelocity(g_pMypeople1->szName, 0, 0);*/
	 if(strstr(szOtherName, "boss_") != NULL)
		 dSetSpriteLinearVelocity(g_pMypeople1->szName, 0, 0);

}


//����boss
void  CreateBoss()
{
	// ÿ�γ���һ�����ˣ��������ڵ�һ�У������������ǰ�治�����ϰ�
	float	fPosX, fPosY;
	char	szName[128];
	char	dpName[128];
	int		iDir;

	Weapon* boss = (Weapon*)malloc(sizeof(Weapon));

	//ͳһ�����ڵ�ͼ�Ϸ����м�
	fPosY = g_fScreenTop+5.f;
	fPosX = (g_fScreenLeft + g_fScreenRight) / 2;
	iDir = 0;	  // ����ͳһ����

	sprintf(dpName, "boss%d", g_iLevel);
	sprintf(szName, "boss_%d_%d",g_iLevel, g_iNum++);
	dCloneSprite(dpName, szName);
	
	dSetSpritePosition(szName, fPosX, fPosY);
	
	// ������ײ
	dSetSpriteCollisionSend(szName, true);
	dSetSpriteCollisionReceive(szName, true);
	// ��������߽�
	dSetSpriteWorldLimit(szName, WORLD_LIMIT_NULL, g_fScreenLeft, g_fScreenTop, g_fScreenRight, g_fScreenBottom);

	// ��ָ��ڵ���뵽������
	strcpy(boss->szName, szName);
	boss->iType = BOSS;
	boss->iDir = iDir;
	boss->fRotateTime = ROTATE_TIME;
	boss->iHp = g_iLevel*10;
	boss->fFireTime =FIRE_TIME;
	boss->pNext=NULL;
	g_pBoss=boss;

	AddToList(boss);
}

//�����ǰ��Ļ����С��
void OnDeleteAllAlien()
{
	Weapon* pNode	= g_pHead; 
	for(;pNode!=NULL;pNode=pNode->pNext)
	{
		if(pNode->iType==ENEMYalien)
		{
			dDeleteSprite(pNode->szName);
			AfterDead(pNode->szName);
		}
		
	}
}

//==========================================================

char help_map[]="UI_helpmap";

char cover_box[]="UI_cover";

char close_box[]="UI_close";
char begin_box[] = "UI_begin";
char help_box[] = "UI_help";
char message_box[]="UI_message";
char over_box[] = "UI_over";

char score_box[] = "UI_score";

char my_score[]="UI_myscore";
char score_first[]="UI_Fscore";
char score_second[]="UI_Sscore";
char score_third[]="UI_Tscore";


char level_box[] = "UI_level";
char myhp_box[]="UI_myhp";
char bosshp_box[]="UI_bosshp";
char time_box[]="UI_time";
void OnClickGo(const float fMouseX, const float fMouseY)
{
	//��Ϸδ����ʱ
	if (g_iGameState==0)
	{	//�����ʼ��Ϸ
		if(dIsPointInSprite(begin_box, fMouseX, fMouseY))
		{
			CleanSprite(help_map);
			CleanSprite(help_box);
			CleanSprite(close_box);
			CleanSprite(begin_box);
			CleanSprite(cover_box);

			dSetTextString(score_first,"");
			dSetTextString(score_second,"");
			dSetTextString(score_third,"");
			g_bStart=true;
			g_iGameState=1;
			g_iSoundID = dPlaySound("bgmusic1.ogg",1,1);
		}
		//����������͹رհ�ť����ȥ
		else if (dIsPointInSprite(help_box, fMouseX, fMouseY))
		{
			dSetSpritePosition(help_map,(g_fScreenLeft+g_fScreenRight)/2,(g_fScreenTop+g_fScreenBottom)/2);
			dSetSpritePosition(close_box,40,30);
		}
		//����رս��������
		if(dIsPointInSprite(close_box, fMouseX, fMouseY))
		{
			CleanSprite(help_map);
			CleanSprite(close_box);
		}
	}	
}
//����ָ�Ƴ���Ļ��
void CleanSprite(const char *szName)
{
	dSetSpritePosition(szName,g_fScreenLeft-400.f,g_fScreenTop-400.f);
}
void paintUI(float fTime)
{
	if(g_iGameState==0)
	{
		ReadScore();//��ȡ����

		dSetTextString(message_box,"");
		dSetTextString(over_box,"");
		dSetTextString(my_score,"");

		char* first=dMakeSpriteName("1st : ", rank[0]);
		char* second=dMakeSpriteName("2nd : ", rank[1]);
		char* third=dMakeSpriteName("3th : ", rank[2]);
		dSetTextString(score_first,first);
		dSetTextString(score_second,second);
		dSetTextString(score_third,third);

		//����ʼ��Ϸ�Ͱ�����ť����ȥ
		dSetSpritePosition(begin_box,(g_fScreenLeft+g_fScreenRight)/2,(g_fScreenTop+g_fScreenBottom)/2+10.f);
		dSetSpritePosition(help_box,(g_fScreenLeft+g_fScreenRight)/2+36.f,(g_fScreenTop+g_fScreenBottom)/2+32.f);
		dSetSpritePosition(cover_box,(g_fScreenLeft+g_fScreenRight)/2,(g_fScreenTop+g_fScreenBottom)/2);

		dSetSpritePosition(score_first,(g_fScreenLeft+g_fScreenRight)/2+36.f,(g_fScreenTop+g_fScreenBottom)/2+15.f);
		dSetSpritePosition(score_second,(g_fScreenLeft+g_fScreenRight)/2+36.f,(g_fScreenTop+g_fScreenBottom)/2+20.f);
		dSetSpritePosition(score_third,(g_fScreenLeft+g_fScreenRight)/2+36.f,(g_fScreenTop+g_fScreenBottom)/2+25.f);
	}
	else if(g_iGameState==2)
	{
		if(!g_bStart)
		{
			CleanSprite("TURRET");
			dSetTextString(bosshp_box,"");
			//�������
			if(playerDead)
			{
				 char *szScore=dMakeSpriteName("SCORE:", g_iScore);
				dSetTextString(myhp_box,"");
				dSetTextString(score_box,"");
				dSetTextString(level_box,"");

				dSetTextString(message_box,"GAME OVER");
				dSetTextString(over_box,"Press space to start again");
				dSetTextString(my_score,szScore);
			}
			else if(isWon)
			{
				 char *szScore=dMakeSpriteName("SCORE:", g_iScore);
				dSetTextString(myhp_box,"");
				dSetTextString(score_box,"");
				dSetTextString(level_box,"");

				dSetTextString(message_box,"CONGRATULATIONS");
				dSetTextString(over_box,"Press space to start again");
				dSetTextString(my_score,szScore);
			}
			else if(g_iLevel==2)
			{
				dSetTextString(message_box,"�ڶ���");
				dStopSound(g_iSoundID);
				dSetSpritePosition("TURRET",(g_fScreenLeft+g_fScreenRight),(g_fScreenTop+g_fScreenBottom)/2);
				g_iSoundID = dPlaySound("bgmusic2.ogg",1,1);
				g_fSleepTime-=fTime;
				if(g_fSleepTime<0.f)
				{
					crene=true;
					dSetTextString(message_box," ");
					PaintMap(5,6,7,8);
					g_fSleepTime=3.f;
					g_bStart=true;
				}
			}
			else if(g_iLevel==3)
			{
				dSetTextString(message_box,"������");
				dStopSound(g_iSoundID);
				dSetSpritePosition("TURRET",(g_fScreenLeft+g_fScreenRight),(g_fScreenTop+g_fScreenBottom)/2);
				g_iSoundID = dPlaySound("bgmusic3.ogg",1,1);
				g_fSleepTime-=fTime;
				if(g_fSleepTime<0.f)
				{
					crene=true;
					dSetTextString(message_box," ");
					PaintMap(9,10,11,12);
					g_fSleepTime=3.f;
					g_bStart=true;
				}
			}
			dSetSpritePosition(message_box,(g_fScreenLeft+g_fScreenRight)/2,(g_fScreenTop+g_fScreenBottom)/2-10.f);
			dSetSpritePosition(my_score,(g_fScreenLeft+g_fScreenRight)/2,(g_fScreenTop+g_fScreenBottom)/2);
			dSetSpritePosition(over_box,(g_fScreenLeft+g_fScreenRight)/2,(g_fScreenTop+g_fScreenBottom)/2+20.f);
		}
		//��Ϸ������
		else if(g_bStart)
		{
			char *szScore=dMakeSpriteName("score:", g_iScore);
			char *szLevel=dMakeSpriteName("level:", g_iLevel);
			char *szHp	 =dMakeSpriteName("lives:", g_pMypeople->iHp);
			char *szBshp=" ";
			char *szTime=dMakeSpriteName(" ",g_fGameTime);
			dSetTextString(score_box,szScore);
			dSetTextString(level_box,szLevel);
			dSetTextString(myhp_box,szHp);
			if(g_pBoss==NULL)
			{
				dSetTextString(bosshp_box,"");
				dSetTextString(time_box,szTime);
			}
			else
			{
				szBshp = dMakeSpriteName("lives:", g_pBoss->iHp);
				dSetTextString(bosshp_box,szBshp);
				dSetTextString(time_box,"");
			}
		}
	}
}


//��Ч
void AfterDead(const char* szName)
{
	static int i=0;//��¼�ڼ���������
	//������ɫ������Чģ��
	char dpAlien[]="deadA";
	char dpBoss[]="deadB";
	char dpBoss2[]="deadC";
	char dpPlayer[]="deadP";
	//������Ч
	char dead[128];
	sprintf(dead,"dead_%d",i++);
	float fPosX=dGetSpritePositionX(szName);
	float fPosY=dGetSpritePositionY(szName);
	if(strstr(szName,"alien_"))
	{
		dCloneSprite(dpAlien, dead);
		dSetSpriteLifeTime(dead,0.5);
	}
	else if(strstr(szName,"boss_2"))
	{
		dCloneSprite(dpBoss2, dead);
		dSetSpriteLifeTime(dead,1);
	}
	else if(strstr(szName,"boss_"))
	{
		dCloneSprite(dpBoss, dead);
		dSetSpriteLifeTime(dead,1);
	}
	else if(strstr(szName,"player"))
	{
		dCloneSprite(dpPlayer, dead);
		dSetSpriteLifeTime(dead,1);
	}
	dSetSpritePosition(dead,fPosX,fPosY);
	
}
//����
void skill_1()
{
	char dpName[128]="skill1";
	char szName[128];
	Weapon* skill = (Weapon*)malloc(sizeof(Weapon));
	int numy[5]={-1,0,1,2,3};
	float numx[2]={-5.f,5.f};
	for(int j=0;j<2;j++)
		for(int i=0;i<5;i++)
		{
			sprintf(szName, "bullet_%d", g_iNum++);
			dCloneSprite(dpName, szName);
			strcpy(skill->szName, szName);
			skill->iType = BOSSBULLET;
			skill->iHp=1;
			AddToList(skill);
			dSetSpriteCollisionSend(szName, true);
			dSetSpriteCollisionReceive(szName, true);
			dSetSpriteWorldLimit(szName, WORLD_LIMIT_NULL, g_fScreenLeft, g_fScreenTop, g_fScreenRight, g_fScreenBottom);
			dSetSpritePosition(skill->szName,numx[j],numy[i]*10.f);
			dSetSpriteLinearVelocity(skill->szName,numx[j],0.f);
		}

}

void skill_2()
{
	char dpName[128]="skill2";
	char szName[128];
	int  num[9]={-4,-3,-2,-1,0,1,2,3,4};
	Weapon* skill = (Weapon*)malloc(sizeof(Weapon));
	int ibasis = dRandomRange(1,11);
	for(int i=1;i<12;i++)
	{
		if(i!=ibasis)
		{
			sprintf(szName, "bullet_%d", g_iNum++);
			dCloneSprite(dpName, szName);
			strcpy(skill->szName, szName);
			skill->iType = BOSSBULLET;
			skill->iHp=1;
			AddToList(skill);
			dSetSpriteCollisionSend(szName, true);
			dSetSpriteCollisionReceive(szName, true);
			dSetSpriteWorldLimit(szName, WORLD_LIMIT_NULL, g_fScreenLeft, g_fScreenTop, g_fScreenRight, g_fScreenBottom);
			dSetSpritePosition(skill->szName,num[i]*11.1f-5.55f,-30.f);
			dSetSpriteLinearVelocity(skill->szName,0.f,5.f);
		}
	}
	
}
void skill_3()
{
	char dpName[128]="skill3";
	char szName[128];
	int num[7]={-3,-2,-1,0,1,2,3};
	Weapon* skill = (Weapon*)malloc(sizeof(Weapon));
	float bpX = dGetSpritePositionX("boss2");
	float bpY = dGetSpritePositionY("boss2")+20.f;
	for(int i = 1;i<7;i++)
	{
		sprintf(szName, "bullet_%d", g_iNum++);
		dCloneSprite(dpName, szName);
		strcpy(skill->szName, szName);
		skill->iType = BOSSBULLET;
		skill->iHp=1;
		AddToList(skill);
		dSetSpriteCollisionSend(szName, true);
		dSetSpriteCollisionReceive(szName, true);
		dSetSpriteWorldLimit(szName, WORLD_LIMIT_NULL, g_fScreenLeft, g_fScreenTop, g_fScreenRight, g_fScreenBottom);
		dSetSpritePosition(skill->szName,bpX,bpY);
		dSetSpriteLinearVelocityPolar(skill->szName,30.f,num[i]*30);
	}

}

void skill_4()
{
	char dpName[128]="skill4";
	char szName[128];
	Weapon* skill = (Weapon*)malloc(sizeof(Weapon));
	int numx[5]={-2,-1,0,1,2};
	int numy[4]={-1,0,1,2};
	for(int i=0;i<5;i++)
		for(int j=0;j<4;j++)
		{
			sprintf(szName, "bullet_%d", g_iNum++);
			dCloneSprite(dpName, szName);
			strcpy(skill->szName, szName);
			skill->iType = BOSSBULLET;
			skill->iHp=1;
			AddToList(skill);
			dSetSpriteCollisionSend(szName, true);
			dSetSpriteCollisionReceive(szName, true);
			dSetSpriteWorldLimit(szName, WORLD_LIMIT_NULL, g_fScreenLeft, g_fScreenTop, g_fScreenRight, g_fScreenBottom);
			dSetSpritePosition(skill->szName,numx[i]*20.f,numy[j]*15.f);
			dSetSpriteLifeTime(skill->szName,1.5f);
		}

}

//
void  WriteScore(const int score) {
	for(int i=0;i<3;i++)
	{
		if(score>rank[i])
		{
			for(int j=i+1;j<3;j++)
				rank[j]=rank[j-1];
			rank[i]=score;
			break;
		}
	}

	FILE* file = fopen("���а�.txt", "w");
	if(file==NULL)return;
	fwrite(rank,sizeof(int),3,file);
	fclose(file);
}

void ReadScore() {
	int s[3]={0,0,0};
	FILE* file = fopen("���а�.txt", "r");
	if(file==NULL)return;
	fread(s,sizeof(int),3,file);
	fclose(file);
	rank[0]=s[0];
	rank[1]=s[1];
	rank[2]=s[2];
}