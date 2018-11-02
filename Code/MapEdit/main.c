////////////////////////////////////////////////////////
//   Project Name:GameMapEdit
//			author:	Plane  Jun	
//      Delveloment:C4droid/CIDE
//				QQ:543322463
///////////////////////////////////////////////////////
//------head file----------
#include<SDL2/SDL.h>
#include<GLES/gl.h>
#include<SDL2/SDL_ttf.h>
#include<SDL2/SDL_image.h>
#include<time.h>
//------const------
#define RIGHT 6
#define LEFT 4
#define UP 2
#define DOWN 8
//------date--------
FILE *fp;
int **Map;		//Importance
clock_t tdown,tup;
int temp;
char str[100];
char str2[100];
int quit = 0;
int win_w,win_h;
int px1,py1,px2,py2,px3,py3;
int state=0;    //0 is move    1 is edit
SDL_Event event;
SDL_Rect rect;      //Map
SDL_Rect rect2;    //Draw Can't thought
SDL_Window *win;
SDL_Renderer *rend;
SDL_Surface *surface;
SDL_Texture *texture;
SDL_Surface *surface2;
SDL_Texture *texture2;
//------function-----
int GameInit();
void GamePaint();
void GameLimit();
void GameDrawLine();
void GameWriteMap();
void GameReadMap();
void GameDrawText();
void GameAdd(int px,int py);
void GameDrawRect();
void RenderText(int x, int y, const char *text);
int GameDirection(int tx1,int ty1,int tx2,int ty2);
int **GameChangeMapSize(int rows,int cols);
void GameFreeMapSize(int **p,int rows);
void GameDestroy();
//------function entrance:main----------
int main(int argc, char **argv)
{
  GameInit();
  GamePaint();
  GameDestroy();
  return 0;
}
///-------function entrance:GameInit------
int GameInit() 
{ 
  // InitSDL
  SDL_Init(SDL_INIT_EVERYTHING);
  // CreateWindow
  win = SDL_CreateWindow("MapEdit", 0, 0, 480, 640, SDL_WINDOW_SHOWN);
  //GetWindowSize
  SDL_GetWindowSize(win,&win_w,&win_h);
  // CreateRenderer
  rend = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
  //CreareSurface
  surface=SDL_LoadBMP("0.bmp");
  // CreateTexture
  texture = SDL_CreateTextureFromSurface(rend,surface);
  // Set Rect
  rect.x = 0;
  rect.y = 0;
  rect.w = surface->w * 50;
  rect.h = surface->h * 2;
  //SetMapSize
  Map=GameChangeMapSize(rect.h/50+1,rect.w/50+1);
  //Read Map
  GameReadMap();
  //ChangeLineWidth
  glLineWidth(2.0);
  return 1; 
}
//-------function entrance:GamePaint--------
void GamePaint() 
{
  while (!quit) {
  	
    while (SDL_PollEvent(&event)) {
    	
    	if(event.type==SDL_MOUSEBUTTONDOWN){
    		//Get Click Down Time
    		tdown=clock();
    		//Get Start X Y  
    		px1=event.motion.x;
    		py1=event.motion.y;
    	}
    	if (event.type == SDL_MOUSEMOTION)
			{
				//Get End X Y
				px2=event.motion.x;
    	    	py2=event.motion.y;
    	    	//Can or Can't move 
    	    	if(!state){
    	    	switch(GameDirection(px1,py1,px2,py2)){
    	    		case RIGHT:rect.x+=abs(px1-px2);break;
    	    		case LEFT:rect.x-=abs(px1-px2);break;
    	    		case UP:rect.y-=abs(py1-py2);break;
    	    		case DOWN:rect.y+=abs(py1-py2);break;
    	    	}
    	    	}
			}
		if(event.type==SDL_MOUSEBUTTONUP){
			//Get ClIck Up Time
			tup=clock();
			//Can or Can't draw rectangle
			if(state){
			px3=event.motion.x;
			py3=event.motion.y;
			GameAdd(px3,py3);
			}
			temp=tup-tdown;
			//Can or Can't edit
			if(temp>2000000){
				if(!state)
					//can
					state=1;
				else
				   //can't
				   state=0;	
			}
		}
    }
    SDL_Delay(10);
    GameLimit();
    SDL_RenderClear(rend);
    SDL_RenderCopy(rend,texture,NULL,&rect);
    GameDrawLine();
    GameDrawRect();
	//GameDrawText();
    SDL_RenderPresent(rend);
    
  }
}
//--------function entrance:Texture-------
void RenderText(int x, int y, const char *text)
{
  TTF_Init();
  TTF_Font *font = NULL;
  int w = 0, h = 0;
#if defined(__ANDROID__)
//安卓平台使用手机本身的字体
  font = TTF_OpenFont("/system/fonts/DroidSansFallback.ttf", 40);
#else
//其他平台使用默认字体
  font = TTF_OpenFont("f.ttf", 40);
#endif
  SDL_Color color;
  color.r = 0;
  color.g = 0;
  color.b = 0xff;
  color.a = 0xff;

  surface2 = TTF_RenderUTF8_Blended(font, text, color);
  w = surface2->w;
  h = surface2->h;
  texture2 = SDL_CreateTextureFromSurface(rend, surface2);
  TTF_CloseFont(font);
  SDL_Rect Box;
  Box.x = x;
  Box.y = y;
  Box.w = w;
  Box.h = h;
  SDL_RenderCopy(rend, texture2, NULL, &Box);
  TTF_Quit();

}
//--------function entrance:GameDirection
int GameDirection(int tx1,int ty1,int tx2,int ty2)
{
	if(tx1-tx2>8) return LEFT;
    if(tx2-tx1>8) return RIGHT;
    if(ty1-ty2>8) return UP;
	if(ty2-ty1>8) return DOWN;
    return -1;
}
//------function entrance:GameLimit------
void GameLimit()
{
	//x
	if(rect.x>0){
		rect.x=0;
	}
	else if(rect.x<win_w-rect.w){
		rect.x=win_w-rect.w;
	}
	
	//y
	if(rect.y>0){
		rect.y=0;
	}
	else if(rect.y<win_h-rect.h){
		rect.y=win_h-rect.h;
	}
}
//------function entrance:GameDrawLine
void GameDrawLine()
{
	SDL_SetRenderDrawColor(rend,0,0,0,0);
    //y
    for(int y=0;y<rect.h/50+1;y++){
    	SDL_RenderDrawLine(rend,rect.x,rect.y+y*50,rect.x+(rect.w/50+1)*50,rect.y+y*50);
    }
    //x
        for(int x=0;x<rect.w/50+1;x++){
    	SDL_RenderDrawLine(rend,rect.x+x*50,rect.y,rect.x+x*50,rect.y+(rect.h/50+1)*50);
    }
}
//------function entrance:GameDrawRect
void GameDrawRect()
{
	for(int i=0;i<rect.h/50+1;i++){
		for(int j=0;j<rect.w/50+1;j++){
				rect2.x=rect.x+j*50;
				rect2.y=rect.y+i*50;
				rect2.w=50;
				rect2.h=50;
			if(Map[i][j]==1){
				//行走
				//Draw Empty Rectangle
				SDL_SetRenderDrawColor(rend,0,255,0,0);
				SDL_RenderFillRect(rend,&rect2);
				//SDL_RenderDrawRect(rend,&rect2);
			}
			else if(Map[i][j]==2){
				//树根
				SDL_SetRenderDrawColor(rend,0,0,0,0);
				SDL_RenderFillRect(rend,&rect2);
			}
			else if(Map[i][j]==3){
				//树叶
				SDL_SetRenderDrawColor(rend,255,255,0,0);
				SDL_RenderFillRect(rend,&rect2);
			}
			else if(Map[i][j]==4){
				//通关
				SDL_SetRenderDrawColor(rend,255,0,255,0);
				SDL_RenderFillRect(rend,&rect2);
			}
			else if(Map[i][j]==5){
				//死亡
				SDL_SetRenderDrawColor(rend,0,0,255,0);
				SDL_RenderFillRect(rend,&rect2);
			}
			else if(Map[i][j]==6){
				//钉子
				SDL_SetRenderDrawColor(rend,255,0,0,0);
				SDL_RenderFillRect(rend,&rect2);
			}
			else if(Map[i][j]==7){
				//出生地
    			SDL_SetRenderDrawColor(rend,0,255,255,0);
				SDL_RenderFillRect(rend,&rect2);
			}
			else if(Map[i][j]==8){
				//旗帜
				SDL_SetRenderDrawColor(rend,100,0,0,0);
				SDL_RenderFillRect(rend,&rect2);
			}
	}
}	
}
//------function entrance:GameDrawText
void GameDrawText()
{
	sprintf(str,"当前位置=%d,%d",rect2.x/50,rect2.y/50);
	sprintf(str2,"按下值=%d,弹起值=%d,变化值=%d",tdown,tup,temp);
    RenderText(10,10,str);
    RenderText(10,60,str2);
}
//-----function entrance:GameChangeMapSize-----
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
//------function entrance:GameFreeMapSize------
void GameFreeMapSize(int **p,int rows) {
    int i;
 
    for (i=0;i<rows;i++) {
        free(p[i]);
    }
    free(p);
}
//------function entrance:GameAdd------
void GameAdd(int px,int py)
{
	int tempx,tempy;
	tempx=(px+abs(rect.x))/50;
	tempy=(py+abs(rect.y))/50;
	if(Map[tempy][tempx])
		Map[tempy][tempx]=0;
	else
		Map[tempy][tempx]=2;
		
	//write new map
	GameWriteMap();
}
//-------function entrance:GameWriteMap-------
void GameWriteMap()
{
	fp=fopen("map1.txt","w+");
	for(int i=0;i<rect.h/50+1;i++){
		for(int j=0;j<rect.w/50+1;j++){
			fprintf(fp,"%d\n",Map[i][j]);
		}
	}
	fclose(fp);
}
//---------function entrance:GameReadMap-------
void GameReadMap()
{
	fp=fopen("map1.txt","r");
	if(fp){
		for(int i=0;i<rect.h/50+1;i++){
			for(int j=0;j<rect.w/50+1;j++){
				fscanf(fp,"%d",&Map[i][j]);
			}
		}
	fclose(fp);
	}
}
//------function entrance:GameDestroy-------
void GameDestroy()
{
	//Free Surface
	SDL_FreeSurface(surface);
	SDL_FreeSurface(surface2);
	//Destroy Texture
	SDL_DestroyTexture(texture);
	SDL_DestroyTexture(texture2);
	//Destroy  Renderer
	SDL_DestroyRenderer(rend);
	//Destroy window
	SDL_DestroyWindow(win);
    // Close SDL
     SDL_Quit();
}