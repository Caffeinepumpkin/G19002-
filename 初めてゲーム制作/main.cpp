#include "DxLib.h"
#include "resource.h"


#define GAME_WIDTH			576	
#define GAME_HEIGHT			576
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

#define MUSIC_BGM_TITLE_PATH		TEXT(".\\MUSIC\\タイトルBGM.mp3")

#define IMAGE_LOAD_ERR_TITLE	TEXT("画像読み込みエラー")




#define IMAGE_TITLE_BK_PATH			TEXT(".\\IMAGE\\砂漠.png")		
#define IMAGE_TITLE_ROGO_PATH		TEXT(".\\IMAGE\\たいとる.png")	
#define IMAGE_TITLE_ROGO_ROTA		0.005	
#define IMAGE_TITLE_ROGO_ROTA_MAX	1.0			
#define IMAGE_TITLE_ROGO_X_SPEED	1		
#define IMAGE_TITLE_START_PATH		TEXT(".\\IMAGE\\砂漠.png")	
#define IMAGE_TITLE_START_CNT		1			
#define IMAGE_TITLE_START_CNT_MAX	30	


#define IMAGE_PLAY_BK_PATH			TEXT(".\\IMAGE\\ging.png")	
#define IMAGE_BACK_PATH				TEXT(".\\IMAGE\\砂漠.png")
#define IMAGE_GOAL_PATH				TEXT(".\\IMAGE\\くりあ.png")

#define IMAGE_BACK_REV_PATH		TEXT(".\\IMAGE\\砂漠.png")	
#define IMAGE_BACK_NUM			4								



#define MAP_DIV_WIDTH		64	
#define MAP_DIV_HEIGHT		64
#define MAP_DIV_TATE		10	
#define MAP_DIV_YOKO		4
#define MAP_DIV_NUM	MAP_DIV_TATE * MAP_DIV_YOKO	

#define  GAME_MAP_PATH ".\\IMAGE\\MAP\\map.PNG"

#define	IMAGE_PLAYER_PATH	TEXT(".\\IMAGE\\カエル2.png")

#define IMAGE_MINI_MERGIN	3

#define GAME_MAP_TATE_MAX	27
#define GAME_MAP_YOKO_MAX	9
#define GAME_MAP_KIND_MAX	2	


#define START_ERR_TITLE		TEXT("スタート位置エラー")
#define START_ERR_CAPTION	TEXT("スタート位置が決まっていません")

#define MOUSE_R_CLICK_TITLE		TEXT("ゲーム中断")
#define MOUSE_R_CLICK_CAPTION	TEXT("ゲームを中断し、タイトル画面に戻りますか？")

enum GAME_MAP_KIND
{
	k = 0,		//外周壁
	r = 9,		//縦レール
	y = 10,		//横レール
	t = 11,		//空白
	s = 99,		//スタート
	g = 100		//ゴール
};

enum GAME_SCENE {
	GAME_SCENE_START,
	GAME_SCENE_PLAY,
	GAME_SCENE_END,
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


typedef struct STRUCT_CHARA
{
	char path[PATH_MAX];
	int handle;
	int x;
	int y;
	int width;
	int height;

	int speed = 10;

	int defaultY;

	RECT coll;

}CHARA;

typedef struct STRUCT_IMAGE_BACK
{
	IMAGE image;
	BOOL IsDraw;
}IMAGE_BACK;


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

int StartTimeFps;
int CountFps;
float CalcFps;
int SampleNumFps = GAME_FPS;

char keyState[256];
char AllKeyState[256] = { '\0' };
char OldAllKeyState[256] = { '\0' };

FONT FontTanu32;

int GameScene;

IMAGE_BACK ImageBack[IMAGE_BACK_NUM];


IMAGE ImageTitleBK;
IMAGE_ROTA ImageTitleROGO;
IMAGE_BLINK ImageTitleSTART;


CHARA player;
MUSIC BGM;

IMAGE ImageGoal;

int scrollCnt = 0;		//スクロール
int scrollCntMax = 2;	//スクロールＭＡＸ

GAME_MAP_KIND mapData[GAME_MAP_TATE_MAX][GAME_MAP_YOKO_MAX]{
	//  0,1,2,3,4,5,6,7,8,
		k,k,k,k,g,k,k,k,k,	// 0
		k,t,r,t,r,t,r,t,k,	// 1
		k,t,r,t,r,y,r,t,k,	// 2
		k,t,r,y,r,t,r,t,k,	// 3
		k,t,r,k,k,k,k,k,k,	// 4
		k,t,r,t,r,t,r,t,k,	// 5
		k,t,r,y,r,t,r,t,k,//6
		k,t,r,t,r,y,r,t,k,//7
		k,t,k,t,r,t,k,t,k,//8
		k,t,r,y,r,t,r,t,k,//9
		k,t,r,t,r,y,r,t,k,//0
		k,t,r,t,t,t,r,t,k,	// 1
		k,t,r,t,r,y,r,t,k,	// 2
		k,t,r,t,r,t,r,t,k,	// 3
		k,t,r,k,r,k,k,k,k,	// 4
		k,t,k,t,r,t,r,t,k,	// 5
		k,t,r,y,r,y,r,t,k,//6
		k,t,r,t,r,t,r,t,k,//7
		k,t,t,t,k,t,r,t,k,//8
		k,t,r,t,r,y,r,t,k,//9
		k,t,r,y,r,t,r,t,k,//0
		k,t,r,t,r,t,r,t,k,//1s
		k,t,r,t,r,k,r,t,k,//2
		k,t,r,y,r,t,r,t,k,//3
		k,t,r,t,r,t,r,t,k,// 4
		k,t,r,t,r,t,r,t,k,	// 5
		k,k,k,k,s,k,k,k,k,	// 6
};

GAME_MAP_KIND mapDataInit[GAME_MAP_TATE_MAX][GAME_MAP_YOKO_MAX];

MAPCHIP mapChip;

MAP map[GAME_MAP_TATE_MAX][GAME_MAP_YOKO_MAX];

RECT mapColl[GAME_MAP_TATE_MAX][GAME_MAP_YOKO_MAX];

VOID MY_FPS_UPDATE(VOID);
VOID MY_FPS_DRAW(VOID);
VOID MY_FPS_WAIT(VOID);

VOID MY_ALL_KEYDOWN_UPDATE(VOID);
BOOL MY_KEY_DOWN(int);
BOOL MY_KEY_UP(int);
BOOL MY_KEYDOWN_KEEP(int, int);

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
BOOL MY_CHECK_GOAL_PLAYER_COLL(RECT);
BOOL MY_CHECK_RECT_COLL(RECT, RECT);

VOID MY_PLAY_INIT(VOID);	//プレイ画面の初期化

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	ChangeWindowMode(TRUE);
	SetGraphMode(GAME_WIDTH, GAME_HEIGHT, GAME_COLOR);
	SetWindowStyleMode(GAME_WINDOW_BAR);
	SetMainWindowText(TEXT(GAME_WINDOW_NAME));
	SetAlwaysRunFlag(TRUE);
	SetWindowIconID(IDI_ICON1);

	SetBackgroundColor(204, 170, 0);	//デフォルトの背景色変更

	if (DxLib_Init() == -1) { return -1; }

	if (MY_LOAD_IMAGE() == FALSE) { return -1; }
	if (MY_LOAD_MUSIC() == FALSE) { return -1; }

	if (MY_FONT_INSTALL_ONCE() == FALSE) { return -1; }
	if (MY_FONT_CREATE() == FALSE) { return -1; }

	GameScene = GAME_SCENE_START;

	SetDrawScreen(DX_SCREEN_BACK);


	MY_PLAY_INIT();	//プレイ画面の初期化

	while (TRUE)
	{
		if (ProcessMessage() != 0) { break; }
		if (ClearDrawScreen() != 0) { break; }

		MY_ALL_KEYDOWN_UPDATE();

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

//プレイ画面の初期化
VOID MY_PLAY_INIT(VOID)
{
	//マップの情報を初期化
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

			//マップの最初の位置を上に上げる
			map[tate][yoko].y -= (GAME_MAP_TATE_MAX - 9) * MAP_DIV_HEIGHT;
		}
	}
	
	//マップの当たり判定
	for (int tate = 0; tate < GAME_MAP_TATE_MAX; tate++)
	{
		for (int yoko = 0; yoko < GAME_MAP_YOKO_MAX; yoko++)
		{
			mapColl[tate][yoko].left = (yoko + 0) * mapChip.width + 1;
			mapColl[tate][yoko].top = (tate + 0) * mapChip.height + 1;
			mapColl[tate][yoko].right = (yoko + 1) * mapChip.width - 1;
			mapColl[tate][yoko].bottom = (tate + 1) * mapChip.height - 1;

			//マップの最初の位置を上に上げる
			mapColl[tate][yoko].top -= (GAME_MAP_TATE_MAX - 9) * MAP_DIV_HEIGHT;
			mapColl[tate][yoko].bottom -= (GAME_MAP_TATE_MAX - 9) * MAP_DIV_HEIGHT;
		}
	}

	//プレイヤーの位置を初期化
	for (int tate = 0; tate < GAME_MAP_TATE_MAX; tate++)
	{
		for (int yoko = 0; yoko < GAME_MAP_YOKO_MAX; yoko++)
		{
			if (mapData[tate][yoko] == s)
			{
				//プレイヤーのスタート位置
				player.x = map[tate][yoko].x;
				player.y = map[tate][yoko].y;

				player.defaultY = player.y;

				//プレイヤーの当たり判定
				player.coll.left = player.x + IMAGE_MINI_MERGIN;
				player.coll.top = player.y + IMAGE_MINI_MERGIN;
				player.coll.right = player.x + player.width - IMAGE_MINI_MERGIN;
				player.coll.bottom = player.y + player.height - IMAGE_MINI_MERGIN;

				break;
			}
		}
	}

	return;
}

VOID MY_START_PROC(VOID)
{

	//プレイ画面へ移動する時
	if (MY_KEY_DOWN(KEY_INPUT_RETURN) == TRUE)
	{
		//プレイ画面の初期化
		VOID MY_PLAY_INIT(VOID);

		GameScene = GAME_SCENE_PLAY;
		return;
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
	{
		ChangeVolumeSoundMem(255 * 50 / 100, BGM.handle);

		PlaySoundMem(BGM.handle, DX_PLAYTYPE_LOOP);
	}

	//マップを強制スクロール
	if (scrollCnt < scrollCntMax)
	{
		scrollCnt++;
	}
	else
	{
		for (int tate = 0; tate < GAME_MAP_TATE_MAX; tate++)
		{
			for (int yoko = 0; yoko < GAME_MAP_YOKO_MAX; yoko++)
			{
				map[tate][yoko].y++;
				mapColl[tate][yoko].top++;
				mapColl[tate][yoko].bottom++;
			}
		}

		scrollCnt = 0;
	}

	//当たり判定で当たっているとき
	if (MY_CHECK_MAP1_PLAYER_COLL(player.coll) == TRUE)
	{
		//スクロールする時
		if (scrollCnt == scrollCntMax)
		{
			//プレイヤーもスクロールさせる
			player.y++;

			//３pxぐらい縮める
			int margin = 3;

			//プレイヤーの当たり判定
			player.coll.left = player.x + margin;
			player.coll.top = player.y + margin;
			player.coll.right = player.x + player.width - margin;
			player.coll.bottom = player.y + player.height - margin;
		}
	}
	else
	{
		//プレイヤーのＹ位置がデフォルトの位置よりも大きい時
		if (player.y > player.defaultY)
		{
			//上にプレイヤーを少しずつ上げる
			player.y--;

			player.coll.left = player.x + IMAGE_MINI_MERGIN;
			player.coll.top = player.y + IMAGE_MINI_MERGIN;
			player.coll.right = player.x + player.width - IMAGE_MINI_MERGIN;
			player.coll.bottom = player.y + player.height - IMAGE_MINI_MERGIN;
		}
	}

	//左キーを押したとき
	if (MY_KEY_DOWN(KEY_INPUT_LEFT) == TRUE)
	{
		CHARA dummy = player;	//ダミーのプレイヤーのに、情報をコピー

		dummy.x -= dummy.speed;				//左に移動
		dummy.coll.left -= dummy.speed;		//当たり判定も移動
		dummy.coll.right -= dummy.speed;	//当たり判定も移動

		//当たり判定で当たっていなければ
		if (MY_CHECK_MAP1_PLAYER_COLL(dummy.coll) == FALSE)
		{
			player = dummy;	//ダミーの情報をプレイヤーに入れる
		}
	}

	//右キーを押したとき
	if (MY_KEY_DOWN(KEY_INPUT_RIGHT) == TRUE)
	{
		CHARA dummy = player;	//ダミーのプレイヤーのに、情報をコピー

		dummy.x += dummy.speed;				//左に移動
		dummy.coll.left += dummy.speed;		//当たり判定も移動
		dummy.coll.right += dummy.speed;	//当たり判定も移動

		//当たり判定で当たっていなければ
		if (MY_CHECK_MAP1_PLAYER_COLL(dummy.coll) == FALSE)
		{
			player = dummy;	//ダミーの情報をプレイヤーに入れる
		}
	}

	//ゴールに当たったら
	if(MY_CHECK_GOAL_PLAYER_COLL(player.coll)==TRUE)
	{
		GameScene = GAME_SCENE_END;
		return;
	}
	
	return;
}

VOID MY_PLAY_DRAW(VOID)
{
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

	/*for (int tate = 0; tate < GAME_MAP_TATE_MAX; tate++)
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
	}*/

	DrawGraph(player.x, player.y, player.handle, TRUE);
	DrawBox(player.coll.left, player.coll.top, player.coll.right, player.coll.bottom, GetColor(255, 0, 0), FALSE);

	DrawString(0, 0, "プレイ画面(スペースキーを押して下さい)", GetColor(255, 255, 255));
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

		GameScene = GAME_SCENE_START;
		return;
	}

	return;
}

VOID MY_END_DRAW(VOID)
{
	//ゴール画像を描画
	DrawGraph(ImageGoal.x, ImageGoal.y, ImageGoal.handle, TRUE);
	
	DrawString(0, 0, "エンド画面(エスケープキーを押して下さい)", GetColor(255, 255, 255));
	return;
}

BOOL MY_LOAD_IMAGE(VOID)
{
	strcpy_s(ImageGoal.path, IMAGE_GOAL_PATH);
	ImageGoal.handle = LoadGraph(ImageGoal.path);
	if (ImageGoal.handle == -1)
	{
		MessageBox(GetMainWindowHandle(), IMAGE_GOAL_PATH, IMAGE_LOAD_ERR_TITLE, MB_OK);
		return FALSE;
	}
	GetGraphSize(ImageGoal.handle, &ImageGoal.width, &ImageGoal.height);
	ImageGoal.x = GAME_WIDTH / 2 - ImageGoal.width / 2;
	ImageGoal.y = GAME_HEIGHT / 2 - ImageGoal.height / 2;

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
	GetGraphSize(ImageTitleROGO.image.handle, &ImageTitleROGO.image.width, &ImageTitleROGO.image.height);
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
	GetGraphSize(ImageTitleSTART.image.handle, &ImageTitleSTART.image.width, &ImageTitleSTART.image.height);
	ImageTitleSTART.image.x = GAME_WIDTH / 2 - ImageTitleSTART.image.width / 2;
	ImageTitleSTART.image.y = ImageTitleROGO.image.y + ImageTitleROGO.image.height / 2 + 10;
	ImageTitleSTART.Cnt = 0.0;
	ImageTitleSTART.CntMAX = IMAGE_TITLE_START_CNT_MAX;
	ImageTitleSTART.IsDraw = FALSE;

	//プレイヤーの画像を読み込む
	strcpy_s(player.path, IMAGE_PLAYER_PATH);
	player.handle = LoadGraph(player.path);
	if (player.handle == -1)
	{
		MessageBox(GetMainWindowHandle(), IMAGE_PLAYER_PATH, IMAGE_LOAD_ERR_TITLE, MB_OK);
		return FALSE;
	}

	GetGraphSize(player.handle, &player.width, &player.height);
	player.x = 0;
	player.y = 0;

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

	return TRUE;
}

VOID MY_DELETE_IMAGE(VOID)
{
	/*for (int num = 0; num < IMAGE_BACK_NUM; num++)
	{
		DeleteGraph(ImageBack[0].image.handle);
	}*/

	//DeleteGraph(player.image.handle);


	DeleteGraph(ImageTitleBK.handle);
	DeleteGraph(ImageGoal.handle);
	DeleteGraph(ImageTitleROGO.image.handle);
	DeleteGraph(ImageTitleSTART.image.handle);



	return;
}

BOOL MY_LOAD_MUSIC(VOID)
{
	/*strcpy_s(BGM.path, MUSIC_BGM_PATH);*/
	BGM.handle = LoadSoundMem(BGM.path);
	/*if (BGM.handle == -1)
	{
		MessageBox(GetMainWindowHandle(), MUSIC_BGM_PATH, MUSIC_LOAD_ERR_TITLE, MB_OK);
		return FALSE;
	}*/

	return TRUE;
}

VOID MY_DELETE_MUSIC(VOID)
{
	DeleteSoundMem(BGM.handle);
	/*DeleteSoundMem(player.musicShot.handle);*/

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
				//岩か空白の時は、当たっている
				if (map[tate][yoko].kind == k || map[tate][yoko].kind == t)
				{
					return TRUE;
				}
			}
		}
	}

	return FALSE;
}


BOOL MY_CHECK_GOAL_PLAYER_COLL(RECT player)
{
	for (int tate = 0; tate < GAME_MAP_TATE_MAX; tate++)
	{
		for (int yoko = 0; yoko < GAME_MAP_YOKO_MAX; yoko++)
		{
			if (MY_CHECK_RECT_COLL(player, mapColl[tate][yoko]) == TRUE)
			{
				//ゴールにたどり着いた
				if (map[tate][yoko].kind == g)
				{
					return TRUE;
				}
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