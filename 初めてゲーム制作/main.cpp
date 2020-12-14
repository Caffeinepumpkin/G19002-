#include "DxLib.h"
#include "resource.h"


#define GAME_WIDTH			800	
#define GAME_HEIGHT			600	
#define GAME_COLOR			32	

#define GAME_WINDOW_BAR		0	
#define GAME_WINDOW_NAME	"GAME TITLE"	

#define GAME_FPS			60	

#define MOUSE_BUTTON_CODE	129		

#define PATH_MAX			255	
#define NAME_MAX			255	

#define FONT_TANU_PATH			TEXT(".\\FONT\\TanukiMagic.ttf")	
#define FONT_TANU_NAME			TEXT("たぬき油性マジック")		

#define FONT_INSTALL_ERR_TITLE	TEXT("フォントインストールエラー")
#define FONT_CREATE_ERR_TITLE	TEXT("フォント作成エラー")

#define IMAGE_LOAD_ERR_TITLE	TEXT("画像読み込みエラー")

#define IMAGE_BACK_PATH			TEXT(".\\IMAGE\\砂漠.png")
#define IMAGE_PLAYER_PATH		TEXT(".\\IMAGE\\player2.png")	

#define IMAGE_TITLE_BK_PATH			TEXT(".\\IMAGE\\砂漠.png")	
#define IMAGE_TITLE_ROGO_PATH		TEXT(".\\IMAGE\\砂漠.jpg")	
#define IMAGE_TITLE_ROGO_ROTA		0.005	
#define IMAGE_TITLE_ROGO_ROTA_MAX	1.0			
#define IMAGE_TITLE_ROGO_X_SPEED	1		
#define IMAGE_TITLE_START_PATH		TEXT(".\\IMAGE\\砂漠.png")	
#define IMAGE_TITLE_START_CNT		1			
#define IMAGE_TITLE_START_CNT_MAX	30		

#define IMAGE_END_COMP_PATH		TEXT(".\\IMAGE\\砂漠.png")	
#define IMAGE_END_COMP_CNT		1		
#define IMAGE_END_COMP_CNT_MAX	30		

#define IMAGE_END_FAIL_PATH		TEXT(".\\IMAGE\\砂漠.png")	
#define IMAGE_END_FAIL_CNT		1			
#define IMAGE_END_FAIL_CNT_MAX	30		

#define IMAGE_BACK_REV_PATH		TEXT(".\\IMAGE\\砂漠.png")
#define IMAGE_BACK_NUM			4						
#define MUSIC_LOAD_ERR_TITLE	TEXT("音楽読み込みエラー")

#define MUSIC_BGM_PATH			TEXT(".\\MUSIC\\タイトルBGM.mp3")	
#define MUSIC_BGM_TITLE_PATH		TEXT(".\\MUSIC\\タイトルBGM.mp3")	
#define MUSIC_BGM_COMP_PATH			TEXT(".\\MUSIC\\タイトルBGM.mp3")			
#define MUSIC_BGM_FAIL_PATH			TEXT(".\\MUSIC\\タイトルBGM.mp3")				
#define GAME_MAP_TATE_MAX	9	
#define GAME_MAP_YOKO_MAX	13
#define GAME_MAP_KIND_MAX	2	

#define GAME_MAP_PATH			TEXT(".\\IMAGE\\MAP\\map.PNG")

#define MAP_DIV_WIDTH		6
#define MAP_DIV_HEIGHT		64	
#define MAP_DIV_TATE		10
#define MAP_DIV_YOKO		4	
#define MAP_DIV_NUM	MAP_DIV_TATE * MAP_DIV_YOKO	

#define START_ERR_TITLE		TEXT("スタート位置エラー")
#define START_ERR_CAPTION	TEXT("スタート位置が決まっていません")

#define GOAL_ERR_TITLE		TEXT("ゴール位置エラー")
#define GOAL_ERR_CAPTION	TEXT("ゴール位置が決まっていません")
#define MOUSE_R_CLICK_TITLE		TEXT("ゲーム中断")
#define MOUSE_R_CLICK_CAPTION	TEXT("ゲームを中断し、タイトル画面に戻りますか？")

enum GAME_MAP_KIND
{
	n = -1,	
	k = 0,	
	t = 9,	
	s = 5,
	g = 3	,
	u = 2
};	

enum GAME_SCENE {
	GAME_SCENE_START,
	GAME_SCENE_PLAY,
	GAME_SCENE_END,
};	

enum GAME_END {
	GAME_END_COMP,	
	GAME_END_FAIL	
};	

enum CHARA_SPEED {
	CHARA_SPEED_LOW = 1,
	CHARA_SPEED_MIDI = 2,
	CHARA_SPEED_HIGH = 3
};	

enum CHARA_RELOAD {
	CHARA_RELOAD_LOW = 60,
	CHARA_RELOAD_MIDI = 30,
	CHARA_RELOAD_HIGH = 15
};

typedef struct STRUCT_I_POINT
{
	int x = -1;	
	int y = -1;
}iPOINT;

typedef struct STRUCT_MOUSE
{
	int InputValue = 0;	
	int WheelValue = 0;	
	iPOINT Point;		
	iPOINT OldPoint;	
	int OldButton[MOUSE_BUTTON_CODE] = { 0 };	
	int Button[MOUSE_BUTTON_CODE] = { 0 };	
}MOUSE;


typedef struct STRUCT_FONT
{
	char path[PATH_MAX];		
	char name[NAME_MAX];		
	int handle;				
	int size;					
	int bold;					
	int type;					

}FONT;

typedef struct STRUCT_IMAGE
{
	char path[PATH_MAX];	
	int handle;				
	int x;					
	int y;					
	int width;				
	int height;				
}IMAGE;

typedef struct STRUCT_MUSIC
{
	char path[PATH_MAX];	
	int handle;				
}MUSIC;


typedef struct STRUCT_IMAGE_ROTA
{
	IMAGE image;	
	double angle;	
	double angleMAX;
	double rate;	
	double rateMAX;	

}IMAGE_ROTA;

typedef struct STRUCT_IMAGE_BLINK
{
	IMAGE image;	
	int Cnt;		
	int CntMAX;		
	BOOL IsDraw;	

}IMAGE_BLINK;

typedef struct STRUCT_MAP_IMAGE
{
	char path[PATH_MAX];			
	int handle[MAP_DIV_NUM];	
	int kind[MAP_DIV_NUM];		
	int width;					
	int height;						
}MAPCHIP;

typedef struct STRUCT_MAP
{
	GAME_MAP_KIND kind;		
	int x;					
	int y;					
	int width;				
	int height;				
}MAP;	

typedef struct STRUCT_CHARA
{
	IMAGE image;				
	int speed;					
	int CenterX;				
	int CenterY;				

	RECT coll;					
	iPOINT collBeforePt;		

}CHARA;

typedef struct STRUCT_IMAGE_BACK
{
	IMAGE image;	
	BOOL IsDraw;		
}IMAGE_BACK;

int StartTimeFps;			
int CountFps;				
float CalcFps;				
int SampleNumFps = GAME_FPS;	

char AllKeyState[256] = { '\0' };			
char OldAllKeyState[256] = { '\0' };	

MOUSE mouse;

FONT FontTanu32;	

int GameScene;		
int GameEndKind;					
RECT GoalRect = { -1,-1, -1, -1 };

IMAGE_BACK ImageBack[IMAGE_BACK_NUM];	

IMAGE ImageTitleBK;						
IMAGE_ROTA ImageTitleROGO;				
IMAGE_BLINK ImageTitleSTART;			

IMAGE_BLINK ImageEndCOMP;				
IMAGE_BLINK ImageEndFAIL;				

CHARA player;		

MUSIC BGM;		
MUSIC BGM_TITLE;	
MUSIC BGM_COMP;		
MUSIC BGM_FAIL;	


GAME_MAP_KIND mapData[GAME_MAP_TATE_MAX][GAME_MAP_YOKO_MAX]{
	//  0,1,2,3,4,5,6,7,8,9,0,1,2,
		k,k,k,k,k,k,k,k,k,k,k,g,k,	// 0
		k,t,t,t,t,t,t,t,t,t,t,t,k,	// 1
		k,t,t,t,t,t,t,t,t,t,t,t,k,	// 2
		k,t,t,t,t,t,t,t,t,t,t,t,k,	// 3
		k,t,k,k,k,k,k,k,k,k,k,k,k,	// 4
		k,t,t,t,t,t,t,t,t,t,t,t,k,	// 5
		k,t,t,t,t,t,t,t,t,t,t,t,k,	// 6
		k,t,t,t,t,t,t,t,t,t,t,t,k,	// 7
		k,k,u,k,k,k,u,k,k,k,k,u,k,	// 8
};	


GAME_MAP_KIND mapDataInit[GAME_MAP_TATE_MAX][GAME_MAP_YOKO_MAX];


MAPCHIP mapChip;
MAP map[GAME_MAP_TATE_MAX][GAME_MAP_YOKO_MAX];

iPOINT startPt{ -1,-1 };

RECT mapColl[GAME_MAP_TATE_MAX][GAME_MAP_YOKO_MAX];
VOID MY_FPS_UPDATE(VOID);			
VOID MY_FPS_DRAW(VOID);				
VOID MY_FPS_WAIT(VOID);				

VOID MY_ALL_KEYDOWN_UPDATE(VOID);
BOOL MY_KEY_DOWN(int);				
BOOL MY_KEY_UP(int);				
BOOL MY_KEYDOWN_KEEP(int, int);		

VOID MY_MOUSE_UPDATE(VOID);			
BOOL MY_MOUSE_DOWN(int);		
BOOL MY_MOUSE_UP(int);				
BOOL MY_MOUSEDOWN_KEEP(int, int);	

BOOL MY_FONT_INSTALL_ONCE(VOID);	
VOID MY_FONT_UNINSTALL_ONCE(VOID);	
BOOL MY_FONT_CREATE(VOID);			
VOID MY_FONT_DELETE(VOID);		

VOID MY_START(VOID);	
VOID MY_START_PROC(VOID);
VOID MY_START_DRAW(VOID);	
VOID MY_PLAY(VOID);			
VOID MY_PLAY_PROC(VOID);	
VOID MY_PLAY_DRAW(VOID);	

VOID MY_END(VOID);			
VOID MY_END_PROC(VOID);		
VOID MY_END_DRAW(VOID);		

BOOL MY_LOAD_IMAGE(VOID);		
VOID MY_DELETE_IMAGE(VOID);		

BOOL MY_LOAD_MUSIC(VOID);		
VOID MY_DELETE_MUSIC(VOID);		

BOOL MY_CHECK_MAP1_PLAYER_COLL(RECT);	
BOOL MY_CHECK_RECT_COLL(RECT, RECT);	
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	ChangeWindowMode(TRUE);								
	SetGraphMode(GAME_WIDTH, GAME_HEIGHT, GAME_COLOR);	
	SetWindowStyleMode(GAME_WINDOW_BAR);				
	SetMainWindowText(TEXT(GAME_WINDOW_NAME));			
	SetAlwaysRunFlag(TRUE);								
	SetWindowIconID(IDI_ICON1);							

	if (DxLib_Init() == -1) { return -1; }	

	if (MY_LOAD_IMAGE() == FALSE) { return -1; }

	if (MY_LOAD_MUSIC() == FALSE) { return -1; }

	if (MY_FONT_INSTALL_ONCE() == FALSE) { return -1; }

	if (MY_FONT_CREATE() == FALSE) { return -1; }

	SetMouseDispFlag(TRUE);	

	GameScene = GAME_SCENE_START;

	SetDrawScreen(DX_SCREEN_BACK);

	
	for (int tate = 0; tate < GAME_MAP_TATE_MAX; tate++)
	{
		for (int yoko = 0; yoko < GAME_MAP_YOKO_MAX; yoko++)
		{
			
			if (mapData[tate][yoko] == s)
			{
				startPt.x = mapChip.width * yoko + mapChip.width / 2;	
				startPt.y = mapChip.height * tate + mapChip.height / 2;
			}
			if (mapData[tate][yoko] == g)
			{
				GoalRect.left = mapChip.width * yoko;
				GoalRect.top = mapChip.height * tate;
				GoalRect.right = mapChip.width * (yoko + 1);
				GoalRect.bottom = mapChip.height * (tate + 1);
			}
		}
	}

	if (startPt.x == -1 && startPt.y == -1)
	{
		MessageBox(GetMainWindowHandle(), START_ERR_CAPTION, START_ERR_TITLE, MB_OK);	return -1;
	}

	if (GoalRect.left == -1)
	{
		MessageBox(GetMainWindowHandle(), GOAL_ERR_CAPTION, GOAL_ERR_TITLE, MB_OK);	return -1;
	}

	while (TRUE)
	{
		if (ProcessMessage() != 0) { break; }
		if (ClearDrawScreen() != 0) { break; }

		MY_ALL_KEYDOWN_UPDATE();			

		MY_MOUSE_UPDATE();					

		MY_FPS_UPDATE();
		switch (GameScene)
		{
		case GAME_SCENE_START:
			MY_START();
			break;
		case GAME_SCENE_PLAY:
			MY_PLAY();	
			break;
		case GAME_SCENE_END:
			MY_END();
			break;
		}

		MY_FPS_DRAW();	

		ScreenFlip();	

		MY_FPS_WAIT();	
	}

	MY_FONT_DELETE();

	MY_FONT_UNINSTALL_ONCE();

	MY_DELETE_IMAGE();

	MY_DELETE_MUSIC();

	DxLib_End();

	return 0;
}

VOID MY_FPS_UPDATE(VOID)
{
	if (CountFps == 0) 
	{
		StartTimeFps = GetNowCount();
	}

	if (CountFps == SampleNumFps) 
	{
		int now = GetNowCount();
		CalcFps = 1000.f / ((now - StartTimeFps) / (float)SampleNumFps);
		CountFps = 0;
		StartTimeFps = now;
	}
	CountFps++;
	return;
}

VOID MY_FPS_DRAW(VOID)
{
	DrawFormatString(0, GAME_HEIGHT - 20, GetColor(255, 255, 255), "FPS:%.1f", CalcFps);
	return;
}

VOID MY_FPS_WAIT(VOID)
{
	int resultTime = GetNowCount() - StartTimeFps;					
	int waitTime = CountFps * 1000 / GAME_FPS - resultTime;

	if (waitTime > 0)	
	{
		WaitTimer(waitTime);	
	}
	return;
}

VOID MY_ALL_KEYDOWN_UPDATE(VOID)
{

	char TempKey[256];	

	for (int i = 0; i < 256; i++)
	{
		OldAllKeyState[i] = AllKeyState[i];
	}

	GetHitKeyStateAll(TempKey);

	for (int i = 0; i < 256; i++)
	{
		if (TempKey[i] != 0)	
		{
			AllKeyState[i]++;	
		}
		else
		{
			AllKeyState[i] = 0;
		}
	}
	return;
}

BOOL MY_KEY_DOWN(int KEY_INPUT_)
{
	if (AllKeyState[KEY_INPUT_] != 0)
	{
		return TRUE;	
	}
	else
	{
		return FALSE;
	}
}

BOOL MY_KEY_UP(int KEY_INPUT_)
{
	if (OldAllKeyState[KEY_INPUT_] >= 1	
		&& AllKeyState[KEY_INPUT_] == 0)	
	{
		return TRUE;	
	}
	else
	{
		return FALSE;	
	}
}

BOOL MY_KEYDOWN_KEEP(int KEY_INPUT_, int DownTime)
{
	int UpdateTime = DownTime * GAME_FPS;

	if (AllKeyState[KEY_INPUT_] > UpdateTime)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

VOID MY_MOUSE_UPDATE(VOID)
{
	mouse.OldPoint = mouse.Point;

	for (int i = 0; i < MOUSE_BUTTON_CODE; i++) { mouse.OldButton[i] = mouse.Button[i]; }

	GetMousePoint(&mouse.Point.x, &mouse.Point.y);

	mouse.InputValue = GetMouseInput();

	if ((mouse.InputValue & MOUSE_INPUT_LEFT) == MOUSE_INPUT_LEFT) { mouse.Button[MOUSE_INPUT_LEFT]++; }		
	if ((mouse.InputValue & MOUSE_INPUT_LEFT) != MOUSE_INPUT_LEFT) { mouse.Button[MOUSE_INPUT_LEFT] = 0; }		

	if ((mouse.InputValue & MOUSE_INPUT_MIDDLE) == MOUSE_INPUT_MIDDLE) { mouse.Button[MOUSE_INPUT_MIDDLE]++; }	
	if ((mouse.InputValue & MOUSE_INPUT_MIDDLE) != MOUSE_INPUT_MIDDLE) { mouse.Button[MOUSE_INPUT_MIDDLE] = 0; }

	if ((mouse.InputValue & MOUSE_INPUT_RIGHT) == MOUSE_INPUT_RIGHT) { mouse.Button[MOUSE_INPUT_RIGHT]++; }		
	if ((mouse.InputValue & MOUSE_INPUT_RIGHT) != MOUSE_INPUT_RIGHT) { mouse.Button[MOUSE_INPUT_RIGHT] = 0; }	

	mouse.WheelValue = GetMouseWheelRotVol();

	return;
}

BOOL MY_MOUSE_DOWN(int MOUSE_INPUT_)
{
	if (mouse.Button[MOUSE_INPUT_] != 0)
	{
		return TRUE;	
	}
	else
	{
		return FALSE;
	}
}

BOOL MY_MOUSE_UP(int MOUSE_INPUT_)
{
	if (mouse.OldButton[MOUSE_INPUT_] >= 1	
		&& mouse.Button[MOUSE_INPUT_] == 0)	
	{
		return TRUE;	
	}
	else
	{
		return FALSE;	
	}
}

BOOL MY_MOUSEDOWN_KEEP(int MOUSE_INPUT_, int DownTime)
{
	int UpdateTime = DownTime * GAME_FPS;

	if (mouse.Button[MOUSE_INPUT_] > UpdateTime)
	{
		return TRUE;	
	}
	else
	{
		return FALSE;	
	}
}

BOOL MY_FONT_INSTALL_ONCE(VOID)
{
	if (AddFontResourceEx(FONT_TANU_PATH, FR_PRIVATE, NULL) == 0)
	{
		MessageBox(GetMainWindowHandle(), FONT_TANU_NAME, FONT_INSTALL_ERR_TITLE, MB_OK);
		return FALSE;
	}

	return TRUE;
}

VOID MY_FONT_UNINSTALL_ONCE(VOID)
{
	RemoveFontResourceEx(FONT_TANU_PATH, FR_PRIVATE, NULL);

	return;
}

BOOL MY_FONT_CREATE(VOID)
{

	strcpy_s(FontTanu32.path, sizeof(FontTanu32.path), FONT_TANU_PATH);
	strcpy_s(FontTanu32.name, sizeof(FontTanu32.name), FONT_TANU_NAME);	
	FontTanu32.handle = -1;								
	FontTanu32.size = 32;								
	FontTanu32.bold = 1;							
	FontTanu32.type = DX_FONTTYPE_ANTIALIASING_EDGE;	


	FontTanu32.handle = CreateFontToHandle(FontTanu32.name, FontTanu32.size, FontTanu32.bold, FontTanu32.type);

	if (FontTanu32.handle == -1) { MessageBox(GetMainWindowHandle(), FONT_TANU_NAME, FONT_CREATE_ERR_TITLE, MB_OK); return FALSE; }


	return TRUE;
}

VOID MY_FONT_DELETE(VOID)
{
	DeleteFontToHandle(FontTanu32.handle);	
	return;
}

VOID MY_START(VOID)
{
	MY_START_PROC();
	MY_START_DRAW();

	return;
}


VOID MY_START_PROC(VOID)
{

	if (CheckSoundMem(BGM_TITLE.handle) == 0)
	{
		ChangeVolumeSoundMem(255 * 50 / 100, BGM_TITLE.handle);
		PlaySoundMem(BGM_TITLE.handle, DX_PLAYTYPE_LOOP);
	}


	if (MY_KEY_DOWN(KEY_INPUT_RETURN) == TRUE)
	{
		if (CheckSoundMem(BGM_TITLE.handle) != 0)
		{
			StopSoundMem(BGM_TITLE.handle);
		}

		SetMouseDispFlag(FALSE);		

		player.CenterX = startPt.x;
		player.CenterY = startPt.y;

		player.image.x = player.CenterX;
		player.image.y = player.CenterY;
		player.collBeforePt.x = player.CenterX;
		player.collBeforePt.y = player.CenterY;
		SetMousePoint(player.image.x, player.image.y);
		GameEndKind = GAME_END_FAIL;

		GameScene = GAME_SCENE_PLAY;
	}

	if (ImageTitleROGO.rate < ImageTitleROGO.rateMAX)
	{
		ImageTitleROGO.rate += IMAGE_TITLE_ROGO_ROTA;
	}

	if (ImageTitleROGO.image.x < GAME_WIDTH / 2)
	{
		ImageTitleROGO.image.x += IMAGE_TITLE_ROGO_X_SPEED;
	}
	else
	{
		if (ImageTitleSTART.Cnt < ImageTitleSTART.CntMAX)
		{
			ImageTitleSTART.Cnt += IMAGE_TITLE_START_CNT;
		}
		else
		{
			if (ImageTitleSTART.IsDraw == FALSE)
			{
				ImageTitleSTART.IsDraw = TRUE;
			}
			else if (ImageTitleSTART.IsDraw == TRUE)
			{
				ImageTitleSTART.IsDraw = FALSE;
			}
			ImageTitleSTART.Cnt = 0;
		}
	}

	return;
}

VOID MY_START_DRAW(VOID)
{
	DrawGraph(ImageTitleBK.x, ImageTitleBK.y, ImageTitleBK.handle, TRUE);
	DrawRotaGraph(
		ImageTitleROGO.image.x, ImageTitleROGO.image.y,
		ImageTitleROGO.rate,							
		ImageTitleROGO.angle,							
		ImageTitleROGO.image.handle, TRUE);				
	if (ImageTitleSTART.IsDraw == TRUE)
	{
		DrawGraph(ImageTitleSTART.image.x, ImageTitleSTART.image.y, ImageTitleSTART.image.handle, TRUE);
	}

	DrawString(0, 0, "スタート画面(エンターキーを押して下さい)", GetColor(255, 255, 255));
	return;
}

VOID MY_PLAY(VOID)
{
	MY_PLAY_PROC();
	MY_PLAY_DRAW();	
	return;
}

VOID MY_PLAY_PROC(VOID)
{
	if (CheckSoundMem(BGM.handle) == 0)
	{		ChangeVolumeSoundMem(255 * 50 / 100, BGM.handle);
		PlaySoundMem(BGM.handle, DX_PLAYTYPE_LOOP);
	}

	if (mouse.Button[MOUSE_INPUT_RIGHT] == TRUE)
	{
		iPOINT R_ClickPt = mouse.Point;

		SetMouseDispFlag(TRUE);

		int Ret = MessageBox(GetMainWindowHandle(), MOUSE_R_CLICK_CAPTION, MOUSE_R_CLICK_TITLE, MB_YESNO);

		if (Ret == IDYES)		
		{
			if (CheckSoundMem(BGM.handle) != 0)
			{
				StopSoundMem(BGM.handle);
			}

			SetMouseDispFlag(TRUE);		

			GameScene = GAME_SCENE_START;
			return;

		}
		else if (Ret == IDNO)	
		{
			SetMousePoint(R_ClickPt.x, R_ClickPt.y);

			SetMouseDispFlag(FALSE);
		}
	}

	if (mouse.Point.x >= 0 && mouse.Point.x <= GAME_WIDTH
		&& mouse.Point.y >= 0 && mouse.Point.y <= GAME_HEIGHT)
	{
		int MoveValue = 100;

		if (abs(player.collBeforePt.x - mouse.Point.x) < MoveValue
			&& abs(player.collBeforePt.y - mouse.Point.y) < MoveValue)
		{
			player.CenterX = mouse.Point.x;
			player.CenterY = mouse.Point.y;
		}
		else
		{
			player.CenterX = player.collBeforePt.x;
			player.CenterY = player.collBeforePt.y;

			SetMousePoint(player.collBeforePt.x, player.collBeforePt.y);
		}

	}

	player.coll.left = player.CenterX - mapChip.width / 2 + 5;
	player.coll.top = player.CenterY - mapChip.height / 2 + 5;
	player.coll.right = player.CenterX + mapChip.width / 2 - 5;
	player.coll.bottom = player.CenterY + mapChip.height / 2 - 5;

	BOOL IsMove = TRUE;

	if (MY_CHECK_MAP1_PLAYER_COLL(player.coll) == TRUE)
	{
		SetMousePoint(player.collBeforePt.x, player.collBeforePt.y);
		IsMove = FALSE;
	}

	if (IsMove == TRUE)
	{
		if (mouse.Point.x >= 0 && mouse.Point.x <= GAME_WIDTH
			&& mouse.Point.y >= 0 && mouse.Point.y <= GAME_HEIGHT)
		{
			player.image.x = player.CenterX - player.image.width / 2;
			player.image.y = player.CenterY - player.image.height / 2;

			player.collBeforePt.x = player.CenterX;
			player.collBeforePt.y = player.CenterY;
		}
	}

	RECT PlayerRect;
	PlayerRect.left = player.image.x + 40;
	PlayerRect.top = player.image.y + 40;
	PlayerRect.right = player.image.x + player.image.width - 40;
	PlayerRect.bottom = player.image.y + player.image.height - 40;

	if (MY_CHECK_RECT_COLL(PlayerRect, GoalRect) == TRUE)
	{
		if (CheckSoundMem(BGM.handle) != 0)
		{
			StopSoundMem(BGM.handle);
		}

		SetMouseDispFlag(TRUE);			

		GameEndKind = GAME_END_COMP;	

		GameScene = GAME_SCENE_END;

		return;
	}


	if (player.image.x > GAME_WIDTH || player.image.y > GAME_HEIGHT
		|| player.image.x + player.image.width < 0 || player.image.y + player.image.height < 0)
	{
	
		if (CheckSoundMem(BGM.handle) != 0)
		{
			StopSoundMem(BGM.handle);
		}

		SetMouseDispFlag(TRUE);		
		GameEndKind = GAME_END_FAIL;

		GameScene = GAME_SCENE_END;

		return;
	}

	
		for (int num = 0; num < IMAGE_BACK_NUM; num++)
		{
			
			ImageBack[num].image.y++;

			if (ImageBack[num].IsDraw == FALSE)
			{
				
				if (ImageBack[num].image.y + ImageBack[num].image.height > 0)
				{
					ImageBack[num].IsDraw = TRUE;
				}
			}

			if (ImageBack[num].image.y > GAME_HEIGHT)
			{
				ImageBack[num].image.y = 0 - ImageBack[0].image.height * 3;	
				ImageBack[num].IsDraw = FALSE;					
			}
		}

		return;
	}


	VOID MY_PLAY_DRAW(VOID)
	{
	
		for (int num = 0; num < IMAGE_BACK_NUM; num++)
		{
			
			if (ImageBack[num].IsDraw == TRUE)
			{
			
				DrawGraph(ImageBack[num].image.x, ImageBack[num].image.y, ImageBack[num].image.handle, TRUE);

				
				DrawFormatString(
					ImageBack[num].image.x,
					ImageBack[num].image.y,
					GetColor(255, 255, 255), "背景画像：%d", num + 1);
			}
		}

		for (int tate = 0; tate < GAME_MAP_TATE_MAX; tate++)
		{
			for (int yoko = 0; yoko < GAME_MAP_YOKO_MAX; yoko++)
			{
				
				DrawGraph(
					map[tate][yoko].x,
					map[tate][yoko].y,
					mapChip.handle[map[tate][yoko].kind],
					TRUE);
			}
		}

		for (int tate = 0; tate < GAME_MAP_TATE_MAX; tate++)
		{
			for (int yoko = 0; yoko < GAME_MAP_YOKO_MAX; yoko++)
			{
				if (mapData[tate][yoko] == k)
				{
					DrawBox(mapColl[tate][yoko].left, mapColl[tate][yoko].top, mapColl[tate][yoko].right, mapColl[tate][yoko].bottom, GetColor(0, 0, 255), FALSE);
				}

				if (mapData[tate][yoko] == t)
				{
					DrawBox(mapColl[tate][yoko].left, mapColl[tate][yoko].top, mapColl[tate][yoko].right, mapColl[tate][yoko].bottom, GetColor(255, 255, 0), FALSE);
				}
			}
		}

		DrawBox(GoalRect.left, GoalRect.top, GoalRect.right, GoalRect.bottom, GetColor(255, 255, 0), TRUE);

		DrawGraph(player.image.x, player.image.y, player.image.handle, TRUE);

		DrawBox(player.coll.left, player.coll.top, player.coll.right, player.coll.bottom, GetColor(255, 0, 0), FALSE);

		
		return;
	}

	VOID MY_END(VOID)
	{
		MY_END_PROC();
		MY_END_DRAW();	
		return;
	}

	VOID MY_END_PROC(VOID)
	{
		if (MY_KEY_DOWN(KEY_INPUT_ESCAPE) == TRUE)
		{

			if (CheckSoundMem(BGM_COMP.handle) != 0)
			{
				StopSoundMem(BGM_COMP.handle);	
			}

			if (CheckSoundMem(BGM_FAIL.handle) != 0)
			{
				StopSoundMem(BGM_FAIL.handle);
			}


			SetMouseDispFlag(TRUE);		
			GameScene = GAME_SCENE_START;


			return;

		}

		switch (GameEndKind)
		{
		case GAME_END_COMP:

			if (CheckSoundMem(BGM_COMP.handle) == 0)
			{
				ChangeVolumeSoundMem(255 * 50 / 100, BGM_COMP.handle);
				PlaySoundMem(BGM_COMP.handle, DX_PLAYTYPE_LOOP);
			}

			if (ImageEndCOMP.Cnt < ImageEndCOMP.CntMAX)
			{
				ImageEndCOMP.Cnt += IMAGE_END_COMP_CNT;
			}
			else
			{
				if (ImageEndCOMP.IsDraw == FALSE)
				{
					ImageEndCOMP.IsDraw = TRUE;
				}
				else if (ImageEndCOMP.IsDraw == TRUE)
				{
					ImageEndCOMP.IsDraw = FALSE;
				}
				ImageEndCOMP.Cnt = 0;
			}
			break;

		case GAME_END_FAIL:
			if (CheckSoundMem(BGM_FAIL.handle) == 0)
			{
				ChangeVolumeSoundMem(255 * 50 / 100, BGM_FAIL.handle);	
				PlaySoundMem(BGM_FAIL.handle, DX_PLAYTYPE_LOOP);
			}

			if (ImageEndFAIL.Cnt < ImageEndFAIL.CntMAX)
			{
				ImageEndFAIL.Cnt += IMAGE_END_FAIL_CNT;
			}
			else
			{
				if (ImageEndFAIL.IsDraw == FALSE)
				{
					ImageEndFAIL.IsDraw = TRUE;
				}
				else if (ImageEndFAIL.IsDraw == TRUE)
				{
					ImageEndFAIL.IsDraw = FALSE;
				}
				ImageEndFAIL.Cnt = 0;
			}
			break;
		}

		return;
	}

	VOID MY_END_DRAW(VOID)
	{
		MY_PLAY_DRAW();

		switch (GameEndKind)
		{
		case GAME_END_COMP:

			if (ImageEndCOMP.IsDraw == TRUE)
			{
				DrawGraph(ImageEndCOMP.image.x, ImageEndCOMP.image.y, ImageEndCOMP.image.handle, TRUE);
			}
			break;

		case GAME_END_FAIL:
			if (ImageEndFAIL.IsDraw == TRUE)
			{
				DrawGraph(ImageEndFAIL.image.x, ImageEndFAIL.image.y, ImageEndFAIL.image.handle, TRUE);
			}
			break;

		}

		DrawString(0, 0, "エンド画面(エスケープキーを押して下さい)", GetColor(255, 255, 255));
		return;
	}

	BOOL MY_LOAD_IMAGE(VOID)
	{
		strcpy_s(ImageTitleBK.path, IMAGE_TITLE_BK_PATH);			
		ImageTitleBK.handle = LoadGraph(ImageTitleBK.path);			
		if (ImageTitleBK.handle == -1)
		{
			MessageBox(GetMainWindowHandle(), IMAGE_TITLE_BK_PATH, IMAGE_LOAD_ERR_TITLE, MB_OK);
			return FALSE;
		}
		GetGraphSize(ImageTitleBK.handle, &ImageTitleBK.width, &ImageTitleBK.height);	
		ImageTitleBK.x = GAME_WIDTH / 2 - ImageTitleBK.width / 2;		
		ImageTitleBK.y = GAME_HEIGHT / 2 - ImageTitleBK.height / 2;		

		strcpy_s(ImageTitleROGO.image.path, IMAGE_TITLE_ROGO_PATH);					
		ImageTitleROGO.image.handle = LoadGraph(ImageTitleROGO.image.path);		
		if (ImageTitleROGO.image.handle == -1)
		{
			MessageBox(GetMainWindowHandle(), IMAGE_TITLE_ROGO_PATH, IMAGE_LOAD_ERR_TITLE, MB_OK);
			return FALSE;
		}
		GetGraphSize(ImageTitleROGO.image.handle, &ImageTitleROGO.image.width, &ImageTitleROGO.image.height);	//画像の幅と高さを取得
		ImageTitleROGO.image.x = 230;							
		ImageTitleROGO.image.y = GAME_HEIGHT / 2;				
		ImageTitleROGO.angle = DX_PI * 2;						
		ImageTitleROGO.angleMAX = DX_PI * 2;					
		ImageTitleROGO.rate = 0.0;								
		ImageTitleROGO.rateMAX = IMAGE_TITLE_ROGO_ROTA_MAX;		

		strcpy_s(ImageTitleSTART.image.path, IMAGE_TITLE_START_PATH);					
		ImageTitleSTART.image.handle = LoadGraph(ImageTitleSTART.image.path);		
		if (ImageTitleSTART.image.handle == -1)
		{
			MessageBox(GetMainWindowHandle(), IMAGE_TITLE_START_PATH, IMAGE_LOAD_ERR_TITLE, MB_OK);
			return FALSE;
		}
		GetGraphSize(ImageTitleSTART.image.handle, &ImageTitleSTART.image.width, &ImageTitleSTART.image.height);	//画像の幅と高さを取得
		ImageTitleSTART.image.x = GAME_WIDTH / 2 - ImageTitleSTART.image.width / 2;			
		ImageTitleSTART.image.y = ImageTitleROGO.image.y + ImageTitleROGO.image.height / 2 + 10;	
		ImageTitleSTART.Cnt = 0.0;								
		ImageTitleSTART.CntMAX = IMAGE_TITLE_START_CNT_MAX;		
		ImageTitleSTART.IsDraw = FALSE;							

		strcpy_s(ImageEndCOMP.image.path, IMAGE_END_COMP_PATH);					
		ImageEndCOMP.image.handle = LoadGraph(ImageEndCOMP.image.path);			
		if (ImageEndCOMP.image.handle == -1)
		{
			MessageBox(GetMainWindowHandle(), IMAGE_END_COMP_PATH, IMAGE_LOAD_ERR_TITLE, MB_OK);
			return FALSE;
		}
		GetGraphSize(ImageEndCOMP.image.handle, &ImageEndCOMP.image.width, &ImageEndCOMP.image.height);
		ImageEndCOMP.image.x = GAME_WIDTH / 2 - ImageEndCOMP.image.width / 2;		
		ImageEndCOMP.image.y = GAME_HEIGHT / 2 - ImageEndCOMP.image.height / 2;			
		ImageEndCOMP.Cnt = 0.0;									
		ImageEndCOMP.CntMAX = IMAGE_END_COMP_CNT_MAX;			
		ImageEndCOMP.IsDraw = FALSE;						

		//エンドフォール
		strcpy_s(ImageEndFAIL.image.path, IMAGE_END_FAIL_PATH);				
		ImageEndFAIL.image.handle = LoadGraph(ImageEndFAIL.image.path);			
		if (ImageEndFAIL.image.handle == -1)
		{
		
			MessageBox(GetMainWindowHandle(), IMAGE_END_FAIL_PATH, IMAGE_LOAD_ERR_TITLE, MB_OK);
			return FALSE;
		}
		GetGraphSize(ImageEndFAIL.image.handle, &ImageEndFAIL.image.width, &ImageEndFAIL.image.height);	
		ImageEndFAIL.image.x = GAME_WIDTH / 2 - ImageEndFAIL.image.width / 2;			
		ImageEndFAIL.image.y = GAME_HEIGHT / 2 - ImageEndFAIL.image.height / 2;			
		ImageEndFAIL.Cnt = 0.0;								
		ImageEndFAIL.CntMAX = IMAGE_END_FAIL_CNT_MAX;		
		ImageEndFAIL.IsDraw = FALSE;							

		//背景画像
		strcpy_s(ImageBack[0].image.path, IMAGE_BACK_PATH);			
		strcpy_s(ImageBack[1].image.path, IMAGE_BACK_REV_PATH);		
		strcpy_s(ImageBack[2].image.path, IMAGE_BACK_PATH);			
		strcpy_s(ImageBack[3].image.path, IMAGE_BACK_REV_PATH);	

		for (int num = 0; num < IMAGE_BACK_NUM; num++)
		{
			ImageBack[num].image.handle = LoadGraph(ImageBack[num].image.path);	//読み込み
			if (ImageBack[num].image.handle == -1)
			{
				MessageBox(GetMainWindowHandle(), IMAGE_BACK_PATH, IMAGE_LOAD_ERR_TITLE, MB_OK);
				return FALSE;
			}
			GetGraphSize(ImageBack[num].image.handle, &ImageBack[num].image.width, &ImageBack[num].image.height);
		}
		ImageBack[0].image.x = GAME_WIDTH / 2 - ImageBack[0].image.width / 2;	
		ImageBack[0].image.y = 0 - ImageBack[0].image.height * 0;				
		ImageBack[0].IsDraw = FALSE;

		ImageBack[1].image.x = GAME_WIDTH / 2 - ImageBack[1].image.width / 2;	
		ImageBack[1].image.y = 0 - ImageBack[0].image.height * 1;				
		ImageBack[1].IsDraw = FALSE;

		ImageBack[2].image.x = GAME_WIDTH / 2 - ImageBack[2].image.width / 2;	
		ImageBack[2].image.y = 0 - ImageBack[0].image.height * 2;				
		ImageBack[2].IsDraw = FALSE;

		ImageBack[3].image.x = GAME_WIDTH / 2 - ImageBack[2].image.width / 2;	
		ImageBack[3].image.y = 0 - ImageBack[0].image.height * 3;				
		ImageBack[3].IsDraw = FALSE;

		strcpy_s(player.image.path, IMAGE_PLAYER_PATH);		
		player.image.handle = LoadGraph(player.image.path);	
		if (player.image.handle == -1)
		{
		
			MessageBox(GetMainWindowHandle(), IMAGE_PLAYER_PATH, IMAGE_LOAD_ERR_TITLE, MB_OK);
			return FALSE;
		}
		GetGraphSize(player.image.handle, &player.image.width, &player.image.height);	
		player.image.x = GAME_WIDTH / 2 - player.image.width / 2;		
		player.image.y = GAME_HEIGHT / 2 - player.image.height / 2;		
		player.CenterX = player.image.x + player.image.width / 2;	
		player.CenterY = player.image.y + player.image.height / 2;		
		player.speed = CHARA_SPEED_LOW;									

		int mapRes = LoadDivGraph(
			GAME_MAP_PATH,										
			MAP_DIV_NUM, MAP_DIV_TATE, MAP_DIV_YOKO,			
			MAP_DIV_WIDTH, MAP_DIV_HEIGHT,						
			&mapChip.handle[0]);
		
		if (mapRes == -1)
		{
		
			MessageBox(GetMainWindowHandle(), GAME_MAP_PATH, IMAGE_LOAD_ERR_TITLE, MB_OK);
			return FALSE;
		}

	
		GetGraphSize(mapChip.handle[0], &mapChip.width, &mapChip.height);

		for (int tate = 0; tate < GAME_MAP_TATE_MAX; tate++)
		{
			for (int yoko = 0; yoko < GAME_MAP_YOKO_MAX; yoko++)
			{
				mapDataInit[tate][yoko] = mapData[tate][yoko];

				map[tate][yoko].kind = mapData[tate][yoko];

				map[tate][yoko].width = mapChip.width;
				map[tate][yoko].height = mapChip.height;

				map[tate][yoko].x = yoko * map[tate][yoko].width;
				map[tate][yoko].y = tate * map[tate][yoko].height;
			}
		}

		for (int tate = 0; tate < GAME_MAP_TATE_MAX; tate++)
		{
			for (int yoko = 0; yoko < GAME_MAP_YOKO_MAX; yoko++)
			{
				mapColl[tate][yoko].left = (yoko + 0) * mapChip.width + 1;
				mapColl[tate][yoko].top = (tate + 0) * mapChip.height + 1;
				mapColl[tate][yoko].right = (yoko + 1) * mapChip.width - 1;
				mapColl[tate][yoko].bottom = (tate + 1) * mapChip.height - 1;
			}
		}

		return TRUE;
	}

	VOID MY_DELETE_IMAGE(VOID)
	{
		for (int num = 0; num < IMAGE_BACK_NUM; num++)
		{
			DeleteGraph(ImageBack[0].image.handle);
		}

		DeleteGraph(player.image.handle);

		DeleteGraph(ImageTitleBK.handle);
		DeleteGraph(ImageTitleROGO.image.handle);
		DeleteGraph(ImageTitleSTART.image.handle);
		DeleteGraph(ImageEndCOMP.image.handle);
		DeleteGraph(ImageEndFAIL.image.handle);


		for (int i_num = 0; i_num < MAP_DIV_NUM; i_num++) { DeleteGraph(mapChip.handle[i_num]); }

		return;
	}

	
	BOOL MY_LOAD_MUSIC(VOID)
	{
		
		strcpy_s(BGM.path, MUSIC_BGM_PATH);		
		BGM.handle = LoadSoundMem(BGM.path);	
		if (BGM.handle == -1)
		{
		
			MessageBox(GetMainWindowHandle(), MUSIC_BGM_PATH, MUSIC_LOAD_ERR_TITLE, MB_OK);
			return FALSE;
		}

	

		
		strcpy_s(BGM_TITLE.path, MUSIC_BGM_TITLE_PATH);			
		BGM_TITLE.handle = LoadSoundMem(BGM_TITLE.path);			
		if (BGM_TITLE.handle == -1)
		{ 
			MessageBox(GetMainWindowHandle(), MUSIC_BGM_TITLE_PATH, MUSIC_LOAD_ERR_TITLE, MB_OK);
			return FALSE;
		}

		
		strcpy_s(BGM_COMP.path, MUSIC_BGM_COMP_PATH);			
		BGM_COMP.handle = LoadSoundMem(BGM_COMP.path);				
		if (BGM_COMP.handle == -1)
		{
		
			MessageBox(GetMainWindowHandle(), MUSIC_BGM_COMP_PATH, MUSIC_LOAD_ERR_TITLE, MB_OK);
			return FALSE;
		}

		
		strcpy_s(BGM_FAIL.path, MUSIC_BGM_FAIL_PATH);				
		BGM_FAIL.handle = LoadSoundMem(BGM_FAIL.path);			
		if (BGM_FAIL.handle == -1)
		{
		
			MessageBox(GetMainWindowHandle(), MUSIC_BGM_FAIL_PATH, MUSIC_LOAD_ERR_TITLE, MB_OK);
			return FALSE;
		}

		

		return TRUE;
	}


	VOID MY_DELETE_MUSIC(VOID)
	{
		DeleteSoundMem(BGM.handle);

		DeleteSoundMem(BGM_TITLE.handle);
		DeleteSoundMem(BGM_COMP.handle);
		DeleteSoundMem(BGM_FAIL.handle);


		return;
	}

	BOOL MY_CHECK_MAP1_PLAYER_COLL(RECT player)
	{
		for (int tate = 0; tate < GAME_MAP_TATE_MAX; tate++)
		{
			for (int yoko = 0; yoko < GAME_MAP_YOKO_MAX; yoko++)
			{
				if (MY_CHECK_RECT_COLL(player, mapColl[tate][yoko]) == TRUE)
				{
					if (map[tate][yoko].kind == k) { return TRUE; }
				}
			}
		}

		return FALSE;
	}

	BOOL MY_CHECK_RECT_COLL(RECT a, RECT b)
	{
		if (a.left < b.right &&
			a.top < b.bottom &&
			a.right > b.left &&
			a.bottom > b.top
			)
		{
			return TRUE;
		}

		return FALSE;
	}
