/*++++++头文件区++++++*/
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include<SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include<stdio.h>
#include<GLES/gl.h>
/*++++++宏定义区+++++++*/
#define WIN_H 800
#define WIN_W 600
#define SPEED 6
/*++++++结构体区+++++++*/
class RoleClass {
	public:
        SDL_Surface* Surface_Rpic;
        SDL_Texture* Texture_Rpic;
        SDL_Rect Rect_Rpos_s;
        SDL_Rect Rect_Rpos_m;
        int StartX[3];
        int StartY[3];
        int SpeedX;
        int SpeedY;
        int g;
        int Fy;
        int t;
        int state;   //判断当前状态是否为在地面上 0在 1不在
        int directX;//判断x方向的移动碰撞
        int directY;//判断y方向的移动状态
};
class TextClass{
	public:
		SDL_Surface* Surface_Tpic;
		SDL_Texture* Texture_Tpic;
};
class MapClass{
	public:
	int **LimitMap;    //地图可行区域
	SDL_Surface* Surface_Mpic;
    SDL_Texture* Texture_Mpic;
	SDL_Rect Rect_Mpos;
	int MapID;
};
class LevelClass{
	public:
	int ID;
	SDL_Rect R_level;
	int x;
	int y;
	int is_active;
};
class SoundClass{
	public:
	Mix_Chunk *Chunk;
	Mix_Music* song;
};
class ButtonClass{
	public:
	SDL_Surface *Surface_PauseButton;
	SDL_Texture *Texture_PauseButton;
	SDL_Rect box[2];
};
class TitleClass{
	public:
	SDL_Surface *Surface_Title;
	SDL_Texture* Texture_Title;
	SDL_Rect rect;
};
/*+++++++全局变量+++++*/
ButtonClass GameButton[3];
 SoundClass sound[3];
 RoleClass Player;
 TextClass Text;
 LevelClass Level[30];	
MapClass Map;
 TitleClass TitlePic;
SDL_Rect Title[100];
SDL_Window* win = NULL;
SDL_Renderer* rend = NULL;
SDL_Event event;
SDL_Surface* Serweima;
SDL_Texture* Terweima;
SDL_Rect Rerweima;
SDL_Rect Chance;
int GameState=0;//0表示界面   1表示游戏  2表示关卡选择  3暂停模式 4游戏说明 5游戏欢迎
int PressTime;
int win_w,win_h;
int quit=0;
char str[100];
int currentmap=0;
int choose;
int first;
int alpha=255;
int sign=0;
SDL_Surface test;
SDL_Rect rect;
/*++++++全局函数区+++++*/
void GameWelCome();
int GameInit();    //游戏初始化
void GamePaint();  //游戏绘图
void GameClean();  //游戏清理
void GameMove();  //小球移动以及碰撞
void GameKey();  //操作主界面
void GamePauseKey();//游戏暂停操作
void GameDebug();//游戏调试
void GameDrawMap(); //显示游戏地图
void GameReadMap();	//读取地图
void GameReadLevel();//读取关卡
void GameLevelKey();//关卡函数
void GameInformKey();//游戏说明操作
void GameWriteLevel();//写关卡，标记关卡是否打开
void GameDrawLevelRect();//画关卡矩形
int GameLimitWalk(int nowx,int nowy); //游戏可行函数
int **GameChangeMapSize(int rows,int cols);	//动态更改地图大小
void GameFreeMapSize(int **p,int rows);	//释放地图
void GameMapMove();	//地图移动
void RenderText(int x, int y, const char *text,int size,int r,int g,int b,int index); //显示文字
void GameChangeMap(int MapID,int PlayerX,int PlayerY);//更换地图
void GameMenuKey();//界面操作
int GameRectHit(int x1,int y1,int rx,int ry,int rw,int rh);//矩形相交
void GameFlag();//判断是否第一次启动游戏
void GameInitRect();//计算关卡选择的框
//----------主函数入口--------
int main(int argc,char** argv)
{
        if (!GameInit()) {
                GameClean();
                return 0;
        }
        GamePaint();
        GameClean();
        return 0;
}
//-------游戏初始化入口---------
int GameInit()
{
	   GameFlag();
		Player.g=3;
		Player.Fy=-4;
	    //GameWriteLevel();
        GameState = 5;
        Player.SpeedX = SPEED;
        Player.SpeedY = SPEED;
        SDL_Init(SDL_INIT_EVERYTHING);
        Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 2048);
        win = SDL_CreateWindow("像素球", 100, 100, WIN_W, WIN_H,
                               SDL_WINDOW_SHOWN);
        SDL_GetWindowSize(win,&win_w,&win_h);
        rend=SDL_CreateRenderer(win,-1,SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC);
        //载入资源
        Map.Surface_Mpic=SDL_LoadBMP("PIC/Map/map0.bmp");
        Player.Surface_Rpic=IMG_Load("PIC/qq.png");
        Serweima=IMG_Load("PIC/erweima.png");
        sound[0].Chunk = Mix_LoadWAV("PIC/Sounds/hit.wav");
        sound[1].Chunk = Mix_LoadWAV("PIC/Sounds/Peow.wav");
        sound[2].song = Mix_LoadMUS("PIC/Sounds/1.wav");
        GameButton[0].Surface_PauseButton=IMG_Load("PIC/Button/pause.png");
        GameButton[1].Surface_PauseButton=IMG_Load("PIC/Button/back.png");
        GameButton[2].Surface_PauseButton=IMG_Load("PIC/Button/set.png");
        TitlePic.Surface_Title=SDL_LoadBMP("PIC/Title/Title.bmp");
	    //去背景
  	  Uint32 color_key = SDL_MapRGB(GameButton[0].Surface_PauseButton->format, 255, 255, 255);
  	  SDL_SetColorKey(GameButton[0].Surface_PauseButton, SDL_TRUE, color_key);
		//SDL_SetColorKey(pause.Surface_PauseButton,SDL_TRUE,SDL_MapRGB(pause.Surface_PauseButton->format,0,0,0));
        //创建渲染纹理
        Map.Texture_Mpic=SDL_CreateTextureFromSurface(rend,Map.Surface_Mpic);
        Player.Texture_Rpic=SDL_CreateTextureFromSurface(rend,Player.Surface_Rpic);
        GameButton[0].Texture_PauseButton=SDL_CreateTextureFromSurface(rend,GameButton[0].Surface_PauseButton);
        GameButton[1].Texture_PauseButton=SDL_CreateTextureFromSurface(rend,GameButton[1].Surface_PauseButton);
        GameButton[2].Texture_PauseButton=SDL_CreateTextureFromSurface(rend,GameButton[2].Surface_PauseButton);
        Terweima=SDL_CreateTextureFromSurface(rend,Serweima);
        TitlePic.Texture_Title=SDL_CreateTextureFromSurface(rend,TitlePic.Surface_Title);
        //设置alpha通道
        SDL_SetTextureBlendMode(TitlePic.Texture_Title,SDL_BLENDMODE_BLEND);
        //地图背景
		Map.Rect_Mpos.x=0;
		Map.Rect_Mpos.y=0;
		Map.Rect_Mpos.w=50*Map.Surface_Mpic->w;
		Map.Rect_Mpos.h=2*Map.Surface_Mpic->h;
		//Size
		GameButton[0].box[0].x=10;
		GameButton[0].box[0].y=10;
		GameButton[0].box[0].w=GameButton[0].Surface_PauseButton->w/3;
		GameButton[0].box[0].h=GameButton[0].Surface_PauseButton->h/3;
		GameButton[1].box[0].w=GameButton[1].Surface_PauseButton->w*3;
		GameButton[1].box[0].h=GameButton[1].Surface_PauseButton->h*3;
		GameButton[1].box[0].x=10;
		GameButton[1].box[0].y=win_h-GameButton[1].box[0].h;
		GameButton[2].box[0].w=GameButton[2].Surface_PauseButton->w*2;
		GameButton[2].box[0].h=GameButton[2].Surface_PauseButton->h*2;
		GameButton[2].box[0].x=10;
		GameButton[2].box[0].y=10;
		TitlePic.rect.x=-900;
		TitlePic.rect.y=-470;
		TitlePic.rect.w=win_w*2;
		TitlePic.rect.h=win_h*2;
		Rerweima.w=Serweima->w/2;
		Rerweima.h=Serweima->h/2;
		Rerweima.x=win_w-Rerweima.w-280;
		Rerweima.y=win_h-Rerweima.h-120;
		choose=1;
		glLineWidth(3.0);
        return 1;
}
//-------游戏清理--------
void GameClean()
{
		if(Map.LimitMap!=0){
			GameFreeMapSize(Map.LimitMap,Map.Rect_Mpos.h/50+1);
		}
	    Mix_FreeChunk(sound[0].Chunk);
	    Mix_FreeChunk(sound[1].Chunk);
	    Mix_FreeMusic(sound[2].song);
	    Mix_Quit();
        SDL_FreeSurface(Map.Surface_Mpic);
        SDL_FreeSurface(Player.Surface_Rpic);
        SDL_FreeSurface(Text.Surface_Tpic);
        SDL_FreeSurface(Serweima);
        SDL_DestroyTexture(Terweima);
  	  SDL_DestroyTexture(Map.Texture_Mpic);
  	  SDL_DestroyTexture(Player.Texture_Rpic);
  	  SDL_DestroyTexture(Text.Texture_Tpic);
        SDL_DestroyRenderer(rend);
        SDL_DestroyWindow(win);
        SDL_Quit();
        quit=1;
        exit(0);
}
//---------游戏绘图----------
void GamePaint()
{
	while(!quit){
		while(SDL_PollEvent(&event)){
			if(GameState==1){
			GameKey();
			}
			else if(GameState==0){
			GameMenuKey();
			}
			else if(GameState==2){
				GameLevelKey();
			}
			else if(GameState==3){
				GamePauseKey();
			}
			else if(GameState==4){
				GameInformKey();
			}
		}
		if(GameState==1){
			//游戏模式
        GameMove();
        GameMapMove();
		SDL_RenderClear(rend);
		SDL_RenderCopy(rend,Map.Texture_Mpic,0,&Map.Rect_Mpos);
		GameDrawMap();
		SDL_RenderCopy(rend,Player.Texture_Rpic,0,&Player.Rect_Rpos_s);
		SDL_RenderCopy(rend,GameButton[2].Texture_PauseButton,0,&GameButton[2].box[0]);
		GameDebug();
		}	
		else if(GameState==0){
			SDL_RenderClear(rend);
			//界面模式
			SDL_RenderCopy(rend,Map.Texture_Mpic,0,0);
			//菜单
			RenderText(win_w/2-280,win_h/9,"像素球",250,0,162,232,0);
			//RenderText(win_w/2-280,win_h/9,"-By:飞机君",50,0,162,232,0);
			RenderText(win_w/2-100,win_h/2,"开始游戏",80,0,255,0,1);
			RenderText(win_w/2-100,(win_h/2)+150,"游戏说明",80,255,0,0,2);
			RenderText(win_w/2-100,(win_h/2)+300,"退出游戏",80,0,0,0,3);
			RenderText(win_w-290,win_h-80,"版本号v3.1",50,0,0,0,4);
			RenderText(Title[choose].x-80,Title[choose].y,"→",80,0,0,0,5);
			//GameDebug();
		}
		else if(GameState==2){
			SDL_RenderClear(rend);
			//SDL_SetRenderDrawColor(rend,255,255,255,0);
			SDL_RenderCopy(rend,Map.Texture_Mpic,0,0);
			RenderText(win_w/2-450,win_h/96,"关卡选择",250,127,127,127,6);
			RenderText(win_w-550,400,"Let's go",100,0,255,0,7);
			RenderText(win_w-400,700,"开始",80,0,255,255,8);
			RenderText(win_w-400,900,"返回",80,0,0,0,9);
			GameDrawLevelRect();
			RenderText(Title[choose].x-80,Title[choose].y,"→",80,0,0,0,5);
			//SDL_RenderDrawRect(rend,&Chance);
			//SDL_SetRenderDrawColor(rend,255,255,255,0);
		}
		else if(GameState==3){
			SDL_RenderCopy(rend,Map.Texture_Mpic,0,0);
			RenderText(win_w/2-450,win_h/9,"游戏暂停",250,0,0,0,10);
			RenderText(win_w/2-100,win_h/2,"继续游戏",80,0,255,0,11);
			RenderText(win_w/2-100,(win_h/2)+150,"重新开始",80,255,0,0,12);
			RenderText(win_w/2-100,(win_h/2)+300,"返回界面",80,0,0,0,13);
			RenderText(Title[choose].x-80,Title[choose].y,"→",80,0,0,0,5);
		}
		else if(GameState==4){
			SDL_RenderCopy(rend,Map.Texture_Mpic,0,0);
			SDL_RenderCopy(rend,Terweima,0,&Rerweima);
			SDL_RenderCopy(rend,GameButton[1].Texture_PauseButton,0,&GameButton[1].box[0]);
			RenderText(50,50,"游戏操作",100,174,0,0,14);
			RenderText(Title[14].x+50,Title[14].y+150,"游戏中点击屏幕小球跳起,长",60,0,0,0,16);
			RenderText(Title[14].x,Title[16].y+100,"按屏幕小球跳得更高,一定高度",60,0,0,0,17);
			RenderText(Title[14].x,Title[17].y+100,"后会自动掉下.利用跳跃来通过",60,0,0,0,18);
			RenderText(Title[14].x,Title[18].y+100,"重重障碍.注：红色方块为炸药",60,0,0,0,19);
			RenderText(Title[14].x,Title[19].y+100,"如果小球碰到就会 Boom !!!",60,0,0,0,20);
			RenderText(win_w/2+50,50,"关于作者",100,0,138,0,15);
			RenderText(Title[15].x+50,Title[15].y+150,"余家贫,耕种不足以自给. 幼",60,0,0,0,21);
			RenderText(Title[15].x,Title[21].y+100,"稚盈室,瓶无储粟,生生所资,未",60,0,0,0,22);
			RenderText(Title[15].x,Title[22].y+100,"见其术。所以望各位老铁施舍1",60,0,0,0,23);
			RenderText(Title[15].x,Title[23].y+100,"元,扫描以下二维码即可支付~~",60,0,0,0,24);
			//RenderText(Title[15].x,Rerweima.y+Rerweima.h+50,"作者QQ:543322463  -By:飞机君",60,0,0,0,25);
			SDL_RenderDrawLine(rend,Title[14].x,Title[14].y+Title[14].h+10,Title[14].x+Title[14].w,Title[14].y+Title[14].h+10);
			SDL_RenderDrawLine(rend,Title[15].x,Title[15].y+Title[15].h+10,Title[15].x+Title[15].w,Title[15].y+Title[15].h+10);
			SDL_RenderDrawLine(rend,win_w/2,50,win_w/2,win_h-50);
		}
		else if(GameState==5){
			GameWelCome();
		}
		SDL_RenderPresent(rend);
	
	}
}
//----------小球的移动和碰撞-------------
void GameMove()
{	
		//x轴方向移动
		switch(Player.directX)
		{
			case 1://向右
					if(GameLimitWalk((Player.Rect_Rpos_m.x+Player.Rect_Rpos_m.w)+SPEED,Player.Rect_Rpos_m.y+Player.Rect_Rpos_m.h/2)){
						Player.SpeedX=SPEED;
						Player.Rect_Rpos_m.x+=Player.SpeedX;
					}
					else{//如果向右不行则向左
						Player.directX=-1;
					}
					break;
			case -1://向左
					if(GameLimitWalk(Player.Rect_Rpos_m.x-SPEED,Player.Rect_Rpos_m.y+Player.Rect_Rpos_m.h/2)){
						Player.SpeedX=SPEED;
						Player.Rect_Rpos_m.x-=Player.SpeedX;
					}
					else{//向左不行则向右
						Player.directX=1;
					}
					break;
		}
		
		//y洲方向移动
		switch(Player.directY)
		{
			case 1://向上  飞
					if(GameLimitWalk(Player.Rect_Rpos_m.x+Player.Rect_Rpos_m.w/2,Player.Rect_Rpos_m.y-SPEED)){
						//Player.SpeedY=SPEED+Player.g+Player.Fy;
						Player.SpeedY=SPEED;
						Player.Rect_Rpos_m.y-=Player.SpeedY;
					}	
					else{
						Player.directY=-1;
						Player.state=1;
					}
					break;
			case -1://向下
					if(GameLimitWalk(Player.Rect_Rpos_m.x+Player.Rect_Rpos_m.w/2,(Player.Rect_Rpos_m.y+Player.Rect_Rpos_m.h)+SPEED)){
						//Player.SpeedY=SPEED+Player.g+Player.Fy;
						Player.SpeedY=SPEED;
						Player.Rect_Rpos_m.y+=Player.SpeedY;
					}
					else{
						Player.state=0;
						Player.StartX[1]=0;
						Player.StartY[1]=0;
						sign=0;
						//Player.StartY[2]=0;
					}
					break;
		}
		
		//更新玩家坐标
		Player.Rect_Rpos_s.x=Map.Rect_Mpos.x+Player.Rect_Rpos_m.x;
		Player.Rect_Rpos_s.y=Map.Rect_Mpos.y+Player.Rect_Rpos_m.y;
}
//---------操作函数入口---------
void GameKey()
{
	if(event.type==SDL_FINGERMOTION/*SDL_FINGERDOWN*/){
		if(!(Player.StartX[1]) && !(Player.StartY[1])){
			Mix_PlayChannelTimed(1, sound[1].Chunk, 0, -1);
			//Player.StartY[2]=Player.Rect_Rpos_m.y;
			Player.StartX[1]=Player.Rect_Rpos_m.x;
			Player.StartY[1]=Player.Rect_Rpos_m.y;
		}
	/*if(abs(Player.Rect_Rpos_m.y-Player.StartY[1])<250)	*/if(Player.Rect_Rpos_m.y>1200){
	if(Player.state==0){
		//判断时间
			sign=1;
			Player.directY=1;
		}
	}
		else{
			sign=0;
			Player.directY=-1;
			Player.state=1;
		}
	
	}
	else if (event.type == SDL_FINGERUP) {
		//不触屏后球球设置为不飞行,贴地面,按下时间设置为0,便于下次飞行
		Player.state=1;
		Player.directY=-1;
		PressTime=0;

	}
	else if(GameRectHit(event.motion.x,event.motion.y,GameButton[2].box[0].x,GameButton[2].box[0].y,GameButton[2].box[0].w,GameButton[2].box[0].h)){
		GameState=3;
		choose=11;
		Mix_PlayChannelTimed(1, sound[0].Chunk, 0, -1);
}
	
}
//---------显示文字函数入口----------
void RenderText(int x, int y, const char *text,int size,int r,int g,int b,int index)
{
  TTF_Init();
  TTF_Font *font = NULL;
  int w = 0, h = 0;
#if defined(__ANDROID__)
//安卓平台使用手机本身的字
  //font = TTF_OpenFont("/system/fonts/DroidSansFallback.ttf", size);
  font = TTF_OpenFont("PIC/Ttf/1.TTF", size);
#else
//其他平台使用默认字体
  font = TTF_OpenFont("PIC/Ttf/1.TTF", size);
#endif
  SDL_Color color;
  color.r =r;
  color.g = g;
  color.b = b;
  color.a = 0;

  Text.Surface_Tpic = TTF_RenderUTF8_Blended(font, text, color);
  w = Text.Surface_Tpic->w;
  h = Text.Surface_Tpic->h;

  Text.Texture_Tpic = SDL_CreateTextureFromSurface(rend, Text.Surface_Tpic);
  TTF_CloseFont(font);
  SDL_Rect Box;
  Box.x = x;
  Box.y = y;
  Box.w = w;
  Box.h = h;
  Title[index]=Box;
  SDL_RenderCopy(rend, Text.Texture_Tpic, NULL, &Box);
  TTF_Quit();

}
//--------游戏调试函数入口------
void GameDebug()
{
	sprintf(str,"Player.StartX[1]:%d  Player.StartY[1]:%d",Player.StartX[1],Player.StartY[1]);
	RenderText(100,100,str,30,0,0,0,26);
	sprintf(str,"abs(Player.StartY[2]-Player.StartY[1]):%d",abs(Player.Rect_Rpos_m.y-Player.StartY[1]));
	RenderText(100,150,str,30,0,0,0,27);
	
	sprintf(str,"Player.State:%d",Player.state);
	RenderText(100,200,str,30,0,0,0,28);
	
	sprintf(str,"Sign:%d",sign);
	RenderText(100,250,str,30,0,0,0,29);
	
	sprintf(str,"PressTime:%d",PressTime);
	RenderText(100,300,str,30,0,0,0,30);
	
	sprintf(str,":resyly%d",abs(Player.Rect_Rpos_m.y-Player.StartY[1]<250));
	RenderText(100,350,str,30,0,0,0,31);
	
	/*
	if(GameState==1){
	SDL_Rect DebugRectRole[2];
	sprintf(str,"PressTime:%d",PressTime);
	//sprintf(str,"PlayerX,Y:%d,%d",Player.StartX[1],Player.StartY[1]);
	//RenderText(10,10,str);
  	SDL_SetRenderDrawColor(rend,0,0,0,0);
  	/*
  	//向右移动显示右碰撞点
  	if(Player.directX==1){
  		DebugRectRole[0].x=Player.Rect_Rpos_m.x+Map.Rect_Mpos.x+Player.Rect_Rpos_m.w;
  		DebugRectRole[0].y=Player.Rect_Rpos_m.y+Map.Rect_Mpos.y+Player.Rect_Rpos_m.h/2;
  		DebugRectRole[0].w=10;
  		DebugRectRole[0].h=10;
  	}
  	//向左移动显示左碰撞点
  	else if(Player.directX==-1){
  		DebugRectRole[0].x=Player.Rect_Rpos_m.x+Map.Rect_Mpos.x;
  		DebugRectRole[0].y=Player.Rect_Rpos_m.y+Map.Rect_Mpos.y+Player.Rect_Rpos_m.h/2;
  		DebugRectRole[0].w=10;
  		DebugRectRole[0].h=10;
  	}
  	
  	
  	//向上移动显示上碰撞点
   	if(Player.directY==1){
  		DebugRectRole[1].x=Player.Rect_Rpos_m.x+Map.Rect_Mpos.x+Player.Rect_Rpos_m.w/2;
  		DebugRectRole[1].y=Player.Rect_Rpos_m.y+Map.Rect_Mpos.y;
  		DebugRectRole[1].w=10;
  		DebugRectRole[1].h=10;
  	}
  	//向下移动显示下碰撞点
  	else if(Player.directY==-1){
  		DebugRectRole[1].x=Player.Rect_Rpos_m.x+Map.Rect_Mpos.x+Player.Rect_Rpos_m.w/2;
  		DebugRectRole[1].y=Player.Rect_Rpos_m.y+Map.Rect_Mpos.y+Player.Rect_Rpos_m.h;
  		DebugRectRole[1].w=10;
  		DebugRectRole[1].h=10;
  	}
  	for(int i=0;i<2;i++){
  		SDL_RenderFillRect(rend,&DebugRectRole[i]);
  	}
  	
		
  		DebugRectRole[0].x=Player.Rect_Rpos_m.x+Map.Rect_Mpos.x;
  		DebugRectRole[0].y=Player.Rect_Rpos_m.y+Map.Rect_Mpos.y;
  		DebugRectRole[0].w=Player.Rect_Rpos_m.w;
  		DebugRectRole[0].h=Player.Rect_Rpos_m.h;
  		SDL_RenderFillRect(rend,&DebugRectRole[0]);
	}
	else if(GameState==0){
		int i;
		for(i=0;i<5;i++){
		SDL_RenderDrawRect(rend,&Title[i]);
		}
	}
  		*/
  		
}
//--------------------
void GameDrawMap()
{
	SDL_Rect RectMap;
	  //画不可行方格
	  int i,j;
  	for ( i = 0; i < Map.Rect_Mpos.h / 50; i++) {
    		for ( j = 0; j < Map.Rect_Mpos.w / 50; j++) {
       		   RectMap.x=Map.Rect_Mpos.x+j*50;
      			RectMap.y=Map.Rect_Mpos.y+i*50;
      			RectMap.w=50;
      			RectMap.h=50;
      			 if (Map.LimitMap[i][j] == 1) {
      				SDL_SetRenderDrawColor(rend,0,255,0,0);
      				SDL_RenderFillRect(rend,&RectMap);
      			}
      			else if (Map.LimitMap[i][j] ==2) {
      				SDL_SetRenderDrawColor(rend,0,0,0,0);
      				SDL_RenderFillRect(rend,&RectMap);
      			}
				 else if (Map.LimitMap[i][j] == 3) {
      				SDL_SetRenderDrawColor(rend,255,255,0,0);
      				SDL_RenderFillRect(rend,&RectMap);
				 }
				 else if (Map.LimitMap[i][j] == 6) {
      				SDL_SetRenderDrawColor(rend,255,0,0,0);
      				SDL_RenderFillRect(rend,&RectMap);
      			}
 				 else if (Map.LimitMap[i][j] == 8) {
      				SDL_SetRenderDrawColor(rend,100,0,0,0);
      				SDL_RenderFillRect(rend,&RectMap);
      			}
      			if((Map.LimitMap[i][j]!=0)){
      				if((Map.LimitMap[i][j]!=4)){
      					if((Map.LimitMap[i][j]!=5)){
      				if((Map.LimitMap[i][j]!=7)){	SDL_SetRenderDrawColor(rend,255,255,255,0);
      					SDL_RenderDrawRect(rend,&RectMap);
      				}
      					}
      				}
      			}
    		}
  	}
}
//-----------读取地图函数入口------------
void GameReadMap(int MapID)
{
		char FileSource[51];
		sprintf(FileSource,"PIC/Map/map%d.txt",MapID);
		/*如果未加载地图则加载地图文件*/
	    FILE *fp;
		fp=fopen(FileSource,"r");//打开文件
		int i,j;
		for( i=0;i<Map.Rect_Mpos.h/50+1;i++){
		     for( j=0;j<Map.Rect_Mpos.w/50+1;j++){
                       fscanf(fp, "%d", &Map.LimitMap[i][j]);
		     }
		}
         fclose(fp);
}
//-----------动态更改地图-------------
int **GameChangeMapSize(int rows,int cols)
{
	int **p,i;
    p=(int **)malloc(rows*sizeof(int *));
    if (NULL==p) exit(1);
    for (i=0;i<rows;i++) {
        p[i]=(int *)malloc(cols*sizeof(int));
        if (NULL==p[i]) exit(1);
    }
    return p;
}
//-----------释放地图--------------
void GameFreeMapSize(int **p,int rows)
{
	int i;
 
    for (i=0;i<rows;i++) {
        free(p[i]);
    }
    free(p);
}
//------------地图移动---------------
void GameMapMove()
{
	/*x轴*/
	if(Player.Rect_Rpos_m.x-win_w/2<0)
		Map.Rect_Mpos.x=0;
	else if(Player.Rect_Rpos_m.x+win_w/2>Map.Rect_Mpos.w)
	    Map.Rect_Mpos.x=win_w-Map.Rect_Mpos.w;
	 else
	    Map.Rect_Mpos.x=win_w/2-Player.Rect_Rpos_m.x;
	    
	   /*y轴*/
	if(Player.Rect_Rpos_m.y-win_h/2<0)
		Map.Rect_Mpos.y=0;
	else if(Player.Rect_Rpos_m.y+win_h/2>Map.Rect_Mpos.h)
	    Map.Rect_Mpos.y=win_h-Map.Rect_Mpos.h;
	 else
	    Map.Rect_Mpos.y=win_h/2-Player.Rect_Rpos_m.y;	   
	    
	   
}
//------------游戏可行函数入口-----------
int GameLimitWalk(int nowx,int nowy)
{
	if(Map.LimitMap[nowy/50][nowx/50]==1 ){
		return 0;//不可走
	}
	else if(Map.LimitMap[nowy/50][nowx/50]==4){
		if(currentmap<29){
		Level[currentmap+1].ID=currentmap+1;
		GameWriteLevel();
		}
		//通关后选择关卡
		GameState=2;
		choose=8;
		//GameChangeMap(currentmap,Level[currentmap].x,Level[currentmap].y);
		
	}
	else if((Map.LimitMap[nowy/50][nowx/50]==5) || (Map.LimitMap[nowy/50][nowx/50]==6)){
		//死亡位置初始化
		Player.directX=1;
        Player.Rect_Rpos_m.x=Player.StartX[0];
        Player.Rect_Rpos_m.y=Player.StartY[0];
	}
	return 1;//可走
}
//----------更换地图函数入口--------
void GameChangeMap(int MapID,int PlayerX,int PlayerY)
{
		//释放地图，重新加载地图
		if(Map.LimitMap!=0){
			GameFreeMapSize(Map.LimitMap,Map.Rect_Mpos.h/50+1);
		}
		//创建地图可行区域
		Map.LimitMap=GameChangeMapSize(Map.Rect_Mpos.h/50+1,Map.Rect_Mpos.w/50+1);
		//读取地图
		GameReadMap(MapID);
        //设置玩家位置(Screen)
        Player.Rect_Rpos_s.w=Player.Surface_Rpic->w/3;
        Player.Rect_Rpos_s.h=Player.Surface_Rpic->h/3;
        Player.Rect_Rpos_s.x=0;
        Player.Rect_Rpos_s.y=0;
        //设置玩家位置(Map)
        Player.StartX[0]=PlayerX;
        Player.StartY[0]=PlayerY;
        Player.Rect_Rpos_m.w=Player.Surface_Rpic->w/3;
        Player.Rect_Rpos_m.h=Player.Surface_Rpic->h/3;
        Player.Rect_Rpos_m.x=Player.StartX[0];
        Player.Rect_Rpos_m.y=Player.StartY[0];
        //设置玩家运动方向
        Player.directX=1;
        Player.directY=-1;
        //记录当前地图id
        Map.MapID=MapID;
}
//-------界面操作函数入口-------
void GameMenuKey()
{
	//手指弹起
	if (event.type == SDL_MOUSEBUTTONDOWN) {
		//如果点中开始游戏
		if(GameRectHit(event.motion.x,event.motion.y,Title[1].x,Title[1].y,Title[1].w,Title[1].h)){
			if(choose==1){
				GameInitRect();
				GameReadLevel();
				GameState=2;
				choose=8;
				
				/*
				GameState=1;
				
				//GameChangeMap(4,190,1400);*/
			}
			else{
				choose=1;
			}
			Mix_PlayChannelTimed(1, sound[0].Chunk, 0, -1);
		}
		//如果点中关于作者
		else if(GameRectHit(event.motion.x,event.motion.y,Title[2].x,Title[2].y,Title[2].w,Title[2].h)){
			if(choose==2){
				GameState=4;
			}
			else{
				choose=2;
			}
			Mix_PlayChannelTimed(1, sound[0].Chunk, 0, -1);
		}
		//如果点中推出游戏
		else if(GameRectHit(event.motion.x,event.motion.y,Title[3].x,Title[3].y,Title[3].w,Title[3].h)){
			if(choose==3){
				GameClean();
				exit(0);
			}
			else{
				choose=3;
			}
			Mix_PlayChannelTimed(1, sound[0].Chunk, 0, -1);
		}
	}
}
//-------矩形相交函数入口------
int GameRectHit(int x1,int y1,int rx,int ry,int rw,int rh)
{
	int c1=x1>rx;
	int c2=y1>ry;
	int c3=x1<(rx+rw);
	int c4=y1<(ry+rh);
	if(c1 && c2 && c3 && c4){
		return 1;
	}
	return 0;
}
//--------读取关卡函数入口--------
void GameReadLevel()
{
		/*如果未加载地图则加载地图文件*/
	    FILE *fp;
	    int i;
		fp=fopen("PIC/Level/level.txt","r");//打开文件
		for( i=0;i<30;i++){
                       fscanf(fp, "%d", &Level[i].ID);
                       
		}
         fclose(fp);
         
		fp=fopen("PIC/Level/x.txt","r");//打开文件
		for( i=0;i<30;i++){
                       fscanf(fp, "%d", &Level[i].x);  
		}
         fclose(fp);
         
		fp=fopen("PIC/Level/y.txt","r");//打开文件
		for(i=0;i<30;i++){
                       fscanf(fp, "%d", &Level[i].y);
                       
		}
         fclose(fp);
}
//---------画关卡矩形函数入口------
void GameDrawLevelRect()
{
	char str_level[22];
	int i;
	for( i=0;i<30;i++){
		if(Level[i].is_active){
			SDL_SetRenderDrawColor(rend,0,255,0,0);
			//SDL_SetRenderDrawColor(rend,0,0,0,0);
		}
	    else{
	    	SDL_SetRenderDrawColor(rend,0,0,0,0);
	    }
		SDL_RenderDrawRect(rend,&Level[i].R_level);
		sprintf(str_level,"%d",Level[i].ID);
		
		if(Level[i].ID==-1){
		  RenderText(Level[i].R_level.x+25,Level[i].R_level.y+18,"?",70,0,0,0,10);
		}
		else{
		  RenderText(Level[i].R_level.x+25,Level[i].R_level.y+18,str_level,70,0,0,0,10);
		}
		
	}
	
}
//--------关卡函数入口-----
void GameLevelKey()
{
	if (event.type == SDL_MOUSEBUTTONDOWN) {
		//返回
		if(GameRectHit(event.motion.x,event.motion.y,Title[9].x,Title[9].y,Title[9].w,Title[9].h)){
			if(choose==9){
				GameState=0;
				choose=1;
			}
			else{
				choose=9;
			}
			Mix_PlayChannelTimed(1, sound[0].Chunk, 0, -1);
		}
		//开始
		else  if(GameRectHit(event.motion.x,event.motion.y,Title[8].x,Title[8].y,Title[8].w,Title[8].h)){
			if(choose==8){
				GameState=1;
				GameChangeMap(currentmap,Level[currentmap].x,Level[currentmap].y);
			}
			else{
				choose=8;
			}
			Mix_PlayChannelTimed(1, sound[0].Chunk, 0, -1);
		}
		//点击判断
		int i,j;
		for(i=0;i<30;i++){
				if(GameRectHit(event.motion.x,event.motion.y,Level[i].R_level.x,Level[i].R_level.y,Level[i].R_level.w,Level[i].R_level.h)){
					if(Level[i].ID!=-1){
					for(j=0;j<30;j++){Level[j].is_active=0;}
					Level[i].is_active=1;
					currentmap=Level[i].ID;
					Mix_PlayChannelTimed(1, sound[0].Chunk, 0, -1);
					}
				}
		}

	}
}
//-------写关卡函数入口--------
void GameWriteLevel()
{
	FILE* fp;
	int i;
	fp=fopen("PIC/Level/level.txt","w+");
	for( i=0;i<30;i++){
			fprintf(fp,"%d\n",Level[i].ID);
	}
	fclose(fp);
}
//--------游戏是否第一次启动---------
void GameFlag()
{
	//读取配置
	FILE* fp;
	int i;
	fp=fopen("PIC/config.txt","r");
			fscanf(fp, "%d", &first);
	fclose(fp);
	
	//判断是否第一次启动
	if(!first){
		first=1;
		fp=fopen("PIC/config.txt","w+");
			fprintf(fp,"%d\n",first);
			fclose(fp);
		Level[0].ID=0;
		for( i=1;i<30;i++){
			Level[i].ID=-1;
		}
		GameWriteLevel();
	}
}
//---------游戏暂停操作-------
void GamePauseKey()
{
	if (event.type == SDL_MOUSEBUTTONDOWN) {
		//继续
		if(GameRectHit(event.motion.x,event.motion.y,Title[11].x,Title[11].y,Title[11].w,Title[11].h)){
			if(choose==11){
				GameState=1;
			}
			else{
				choose=11;
			}
			Mix_PlayChannelTimed(1, sound[0].Chunk, 0, -1);
		}
		//重新开始
		else if(GameRectHit(event.motion.x,event.motion.y,Title[12].x,Title[12].y,Title[12].w,Title[12].h)){
			if(choose==12){
				GameState=1;
				Player.directX=1;
        		Player.Rect_Rpos_m.x=Player.StartX[0];
        		Player.Rect_Rpos_m.y=Player.StartY[0];
			}
			else{
				choose=12;
			}
			Mix_PlayChannelTimed(1, sound[0].Chunk, 0, -1);
		}
		//返回界面
		else if(GameRectHit(event.motion.x,event.motion.y,Title[13].x,Title[13].y,Title[13].w,Title[13].h)){
			if(choose==13){
				GameState=0;
				choose=1;
			}
			else{
				choose=13;
			}
			Mix_PlayChannelTimed(1, sound[0].Chunk, 0, -1);
		}
	}
}
//----------游戏说明操作函数入口---------
void GameInformKey()
{
	if (event.type == SDL_MOUSEBUTTONDOWN) {
		//返回
		if(GameRectHit(event.motion.x,event.motion.y,GameButton[1].box[0].x,GameButton[1].box[0].y,GameButton[1].box[0].w,GameButton[1].box[0].h)){
			GameState=0;
			Mix_PlayChannelTimed(1, sound[0].Chunk, 0, -1);
		}
	}
}
//---------游戏欢迎界面函数入口---------
void GameWelCome()
{
	SDL_SetTextureAlphaMod(TitlePic.Texture_Title,alpha);
	SDL_RenderCopy(rend,Map.Texture_Mpic,0,0);
	SDL_RenderCopy(rend,TitlePic.Texture_Title,0,&TitlePic.rect);
	alpha--;
	if(alpha==0){
		GameState=0;
		Mix_PlayMusic(sound[2].song,-1);
	}
}
//-------------关卡框绘制函数入口----------------
void GameInitRect()
{
		//初始化矩形	
		int tempx,tempy;
		int n=0;
		/*for( i=0;i<30;i++){
			if((i==0)||(i==6)||(i==12)||(i==18)||(i==24)){
				tempx=0;
				tempy++;
			}
			else{
				tempx++;
			}
				Level[i].R_level.x=250+((win_w/12)*tempx);
				Level[i].R_level.y=30+((win_h/7)*tempy);		
				Level[i].R_level.w=100;
				Level[i].R_level.h=100;		
		}*/
		for (int i=0;i<5;i++){
			for(int j=0;j<6;j++){
				Level[n].R_level.x=((Title[6].x+150)*j)+250;
				Level[n].R_level.y=((Title[6].y+150)*i)+350;		
				Level[n].R_level.w=100;
				Level[n].R_level.h=100;
				++n;
			}
		}
		Chance.x=190;
		Chance.y=300;
		Chance.w=9*100;
		Chance.h=100*100;
		//默认第0关选中
		Level[0].is_active=1;
}