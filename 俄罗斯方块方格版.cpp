//#define _DEBUG
#include <graphics.h>
#include <time.h>
#include <stdio.h>
#include <conio.h>
#include <mmsystem.h>
#include <tchar.h>

#pragma message("编译成功")


/*  待改进:
    1.BGM,消行音效(音乐无法播放?):原因:mus.close()
    2.消行特效-完成(改变EGERGB参数)
    3.一落到底功能
    4.问题:当有多个函数定义了同一个名字(不同参数)如何选择其中不同的函数?
    5.restart功能-完成
    6.注意setcolor和setfillcolor的区别
    7.键位提示-完成
    8.自选砖块功能(鼠标操作)
    9.文字颜色,字体
    10.变更时的发光- (做颜色渐变时要写sleep(1)
    11.多线程？
    12.占用内存太大？
    13.Sleep可以理解为刷新率?
    14.能否用数据结构优化CPU占用?
    15.AI判断最佳路径
    16.清空输入流: while( kbhit() ) getch();       //相当于清空输入流
*/

#define BOX_R 20 /*容器行数*/
#define BOX_C 10 /*容器列数*/

#define CS 20 /*方块的大小*/
#define BX 100 /*容器左上角的x坐标*/
#define BY 100 /*容器左上角的y坐标*/
#define BSR 20*20 /*容器的大小*/
#define BSC 10*20 /*容器的大小*/
#define INIT_X 80 /*方块初始化X坐标*/
#define INIT_Y 10 /*方块初始化Y坐标*/
#define BLKNUM 40 /*方块数组大小(备选的方块形态总数,旋转后的算新的一种)*/
#define CURBLKNUM 28 /*当前使用的方块个数*/

/*定义要响应的按键(getch()函数返回的是虚拟键码的后两位(十六进制))*/
#define DOWN  VK_DOWN /*↓键*/
#define UP  VK_UP /*↑键*/
#define LEFT  VK_LEFT /*←键*/
#define RIGHT  VK_RIGHT /*→键*/
#define ESC  VK_ESCAPE /*退出键*/
#define SPACE VK_SPACE /*空格键*/
#define TAB VK_TAB
#define HOME VK_HOME
#define CONTROL VK_CONTROL
#define NUM1 VK_NUMPAD1
#define NUM2 VK_NUMPAD2
#define NUM3 VK_NUMPAD3
#define TIMER 0x1c          /*时间中断号*/
#define _CPPARGS

/*以下是键盘扫描码,TC中的getkey()函数才返回)*/
//#define DOWN  0X5000  /*↓键*/
//#define UP  0X4800 /*↑键*/
//#define LEFT  0X4B00  /*←键*/
//#define RIGHT  0X4D00 /*→键*/
//#define ESC  0X11b   /*退出键*/

#define LSCORE  100      /*消除一行所得的分数*/
#define SCX  200  /*显示分数的位置，X坐标*/
#define SCY  100  /*显示分数的位置，Y坐标*/

typedef struct {
    int x;      //左上角格子坐标
    int y;
    int xy[4][2];    //每一块的格子坐标(因为每种方块固定由4个格子构成)
    int color;
}block;

extern int b[BLKNUM][4][4];             //方块形态数组,详细在下面
block blk[BLKNUM];                      //备选方块数组
int box[BOX_C][BOX_R];                  //容积每个方格状态


int X=INIT_X, Y=INIT_Y; /*方块的当前位置，全局变量*/
int key,keyPress; /*保存按键消息，全局变量*/
int k,next,nextColor; /*当前的方块类型代号(1~24),只有生成和旋转时改变(该全局变量就避免了给方块数据结构增加一项代号的麻烦*/
int count = 0;/*计数器，记录已经消除的行数*/
char score[100];
int stop = 0;
int alive = 0;
int MaxTicks = 250;
int CurBlkNum = CURBLKNUM;          /*当前使用的方块个数*/

int dstX,dstY;
int res;                /*记录当前AI是否找到了目的地*/
int AIMode = 0;
int ChangeTimes;        /*可以变换的次数*/
block curblk;           /*当前操作的方块(省去传递指针)*/
MUSIC mus;
char FilePath[_MAX_PATH];
char ImagePath[_MAX_PATH];


/*
LPSTR：32bit指针指向一个字符串，每个字符占1字节
LPCSTR:32-bit指针指向一个常字符串，每个字符占1字节
LPCTSTR:32-bit指针指向一个常字符串,每字符可能占1字节或2字节，取决于Unicode是否定义
LPTSTR:32-bit指针每字符可能占1字节或2字节，取决于Unicode是否定义
*/
int ExtractResource(LPCTSTR strDstFile, LPCTSTR strResType, LPCTSTR strResName);    //释放存在EXE中的多媒体资源
int LoadMp3();                      //读取MP3文件
int LoadBmp();                      //读取bmp文件
int MusicPlay();                    //播放音乐
int MusicStop();                    //结束播放
int Help();                         //帮助文字
int InitBox();                      //初始化容器
int ShowBox();                      //显示容器
int InitBlock();                    //初始化方块
int InitFonts();                    //初始化字体
int ShowCube();                     //产生一个新的方块
int ShowCube(int x,int y);          //在某一个位置产生一个新的方块
int GameInitial();                  //游戏初始化
int DrawCube();                     //画出一个方块
int ClearCube();                    //擦除一个方块
int isOk(block nextblk);            //传入下一步方块的属性,结合容器状态数组判断是否可进行下一步
int MoveDown();                     //向下移动
int MoveLeft();                     //向左移动
int MoveRight();                    //向右移动
int Rotate();                       //旋转
int TurnNext();                     //(原创玩法)直接变形成下一个要出场的方块
int DelLine();                      //消除一行
int ShowScore();                    //显示分数
int Complete();                     //程序结束
int Restart();                      //重新开始
int SelfMadeBlocks(int n);          //(原创玩法)自定义一个方块
int GameOver();                     //游戏失败
int printBox(int Box[BOX_C][BOX_R]){
    int i,j;
    setfillcolor(BLACK);
    bar(0,0,100,300);
    for( i=0; i<BOX_C; i++){
        for( j=0; j<BOX_R; j++){
            xyprintf(i*8,j*15,"%d",Box[i][j]);
        }
    }
    return 0;
}

// 自动下落AI，未完成
int VirtualRotate(block * pdstblk,int * pcurnum){
    int type = *pcurnum/4;              //当前方块的大类序号
    int num  = *pcurnum%4;              //当前方块在大类中的序号
    *pcurnum = (num+1)%4;             //把方块变为同一组的下一个
    block newblk = blk[type*4+num];
    newblk.x = pdstblk->x;
    newblk.y = pdstblk->y;
    *pdstblk = newblk;
    return 0;
}

int FindDst(){
    int y,x,curnum,rotatetimes;
    block dstblk = curblk;
    for( y=BOX_R; y>=curblk.y; y--){
        dstblk.y = y;
        for( x=0; x<=BOX_C-1; x++){
            dstblk.x = x;
            curnum = k;
            rotatetimes = 0;
            for(int i=0; i<3; i++){
                if( isOk(dstblk) ){
                    dstX = x;
                    dstY = y;
//                    xyprintf(0,20,"rotatetimes = %d",rotatetimes);
                    while( rotatetimes-- ) Rotate();
                    return 1;
                }
                VirtualRotate(&dstblk,&curnum);
                rotatetimes++;
            }
        }
//        getch();
    }
    return 0;
}


/*        if( kbhit() ){
            key = getch();
            key = key & 0xff;
            switch(key){
                case UP:if( coorY>0 ) coorY--;
                        break;
                case DOWN:if( coorY<3 ) coorY++;
                        break;
                case LEFT:if( coorX>0 ) coorX--;
                        break;
                case RIGHT:if( coorX<3 ) coorX++;
                        break;
            }
            if( key==SPACE ){                   //操作较复杂,额外处理
                if( TempBox[coorX][coorY] == 1 ){           //已经填充过了

                }else{                          //未填充过

                }
            }
        }
*/


/*      Print Box           */
//        for(k=0; k<20; k++){
//            for(j=0; j<10; j++){
//                xyprintf(j*10,k*20,"%d",box[j][k]);
//            }
//        }

//-------------------------------------↓【MAIN】↓-------------------------------------------//

int main(){
    initgraph(640,600);
    InitFonts();
    GameInitial();
    int ticks = 0;
    keyPress = 0;
    while( !stop ){
//        xyprintf(0,0,"AIMode = %d",AIMode);
//        xyprintf(0,40,"dstX = %d, dstY = %d",dstX,dstY);
        if( alive==0 ){
            while( DelLine() );             //不断消除直到DelLine返回0
            ShowCube();
            ShowScore();
            while( kbhit() ) getch();       //相当于清空输入流
//            FindDst();
        }
        if( ticks>=MaxTicks ){
//            xyprintf(0,0,"MaxTicks = %d",MaxTicks);
            ticks = 0;
            MoveDown();
//            printBox(box);
        }
        while( kbhit() ){
            key = getch();
            key = key & 0xff;                       //因为VK_UP这些键都只取16进制中的后两位
            if( key==VK_DELETE ){
                AIMode ^= 1;
            }
            switch( key ){
                case SPACE:getch();
                        break;
                case UP:Rotate();
                        break;
                case DOWN:MoveDown();
                        break;
                case LEFT:MoveLeft();
                        break;
                case RIGHT:MoveRight();
                        break;
                case ESC:stop = 1;
                        break;
                case TAB:TurnNext();
                        break;
                case HOME:Restart();
                        break;
                default:break;
            }
        }
        Sleep(1);
        ticks++;
    }
    getch();
    GameOver();
    return 0;
}

//-------------------------------------↑【MAIN】↑-------------------------------------------//

int Help(){
//    xyprintf(240,300,"Press any key to start...");
    cleardevice();
    xyprintf(240,300,"Select Mode :");
    xyprintf(260,320,"F1 - EASY");
    xyprintf(260,340,"F2 - NORMAL");
    xyprintf(260,360,"F3 - HELL");
    switch( key&0xff ) {
        case VK_F1:MaxTicks = 250;
                  break;
        case VK_F2:MaxTicks = 200;
                  break;
        case VK_F3:MaxTicks = 60;
                  break;
    }
    return 0;
}


int  ExtractResource(LPCTSTR strDstFile, LPCTSTR strResType, LPCTSTR strResName){
 /*提取指定模块中的资源文件(通用于任意类型)
 参数：
		strDstFile:		目标文件名。提取的资源将保存在这里；
		strResType:		资源类型；
		strResName:		资源名称；
 返回值：
		true: 执行成功；
		false: 执行失败。
*/
    HANDLE hFile = ::CreateFile(strDstFile, GENERIC_WRITE, NULL, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_TEMPORARY, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
		return 0;
    //查找并读取文件,加载(分配)到内存,返回资源大小
/* FindResource用来在一个指定的模块中定位所指定的资源：
     HRSRC FindResource(
     HMODULE hModule,        //包含所需资源的模块句柄，如果是程序本身，可以置为NULL
     LPCTSTR lpName,         //可以是资源名称或资源ID
     LPCTSTR lpType          //资源类型，在这里也就是我们自己指定的资源类型
     );
     LoadResource用来将所指定的资源加载到内存当中；
     HGLOBAL LoadResource(
     HMODULE hModule,                //模块句柄，同上
     HRSRC hResInfo          //需要加载的资源句柄，这里也就是FindResource的返回值
     );
*/
    HRSRC hRes   = ::FindResource(NULL,strResName,strResType);
    HGLOBAL hMem = ::LoadResource(NULL,hRes);
    DWORD dwSize = ::SizeofResource(NULL,hRes);
    //写入文件
    DWORD dwWrite = 0;
    ::WriteFile(hFile,hMem,dwSize,&dwWrite,NULL);
    ::CloseHandle(hFile);
//    xyprintf(0,40,"hMem = %ld, dwSize = %ld, dwWrite = %ld",dwSize,dwWrite);
    return 1;
}


int LoadBmp(){
    char s[300];
    ::GetTempPath(_MAX_PATH,ImagePath);
    strcat(ImagePath,"bkimage.bmp");
    ExtractResource(ImagePath,_T("BMP"),_T("bkimage.bmp"));

    return 0;
}

int LoadMp3(){
    char s[300];        //存放生成的临时文件的完整路径,以标准输出到另一个字符串中(实际用处并不大,可以只用FilePath
    ::GetTempPath(_MAX_PATH,FilePath);      //获取一个可以释放临时文件的目录
    strcat(FilePath,"bgm.mp3");
    ExtractResource(FilePath,_T("MP3"),_T("bgm.mp3"));
//    xyprintf(0,20,"Saved to %s",FilePath);

    sprintf(s,"open \"%s\" alias mymusic",FilePath);
    mciSendString(s,NULL,0,NULL);
    mciSendString(_T("Play Music"),NULL,0,NULL);


    return 0;
}

int MusicPlay(){
    mus.OpenFile(FilePath);
    mus.SetVolume(1.0f);
    if (mus.GetPlayStatus() == MUSIC_MODE_STOP) { // 发现停止了就重新播放
            // Play函数：参数1表示开始播放的地方，参数2是结束的地方
            // 如果两个参数都不写，就当前位置开始播，只填第一个表示播放到结束为止
            mus.Play(0);
        }
    return 0;
}

int MusicStop(){
    mciSendString(_T("Stop Music"),NULL,0,NULL);
    mciSendString(_T("close mymusic"), NULL, 0, NULL);

    return 0;
}

int InitBox(){
    int i,j;
    for( i=0; i<BOX_R; i++)
        for( j=0 ;j<BOX_C; j++)
            box[j][i] = 0;
    return 0;
}

int ShowBox(){
    cleardevice();
    int x,y;
    // Image Print
    PIMAGE img = newimage();
    getimage(img,ImagePath);
    putimage_alphablend(NULL,img,0,0,70,0,0,640,600);
    delimage(img);
    setfillcolor(BLACK);
    bar(BX,BY,BX+BSC,BY+BSR);
    // Line Print
    setcolor(EGERGB(80,80,80));             //Grid Color
    for( y=BY; y<=BSR+BY; y+=CS){           //横线
        line(BX,y,BX+BSC,y);
    }
    for( x=BX; x<=BSC+BX; x+=CS){           //竖线
        line(x,BY,x,BY+BSR);
    }
    setcolor(EGERGB(255,255,255));
    rectangle(BX,BY,BX+BSC,BY+BSR);
    // Key Settings
    xyprintf(330,300,"ROTATE                      Up");
    xyprintf(330,320,"PAUSE                         Space");
    xyprintf(330,340,"EXIT                            Esc");
    xyprintf(330,360,"NEXT BLOCK              Tab");
    xyprintf(330,380,"RESTART                    Home");
    return 0;
}

int InitFonts(){
    LOGFONTA fonts;
    getfont(&fonts);
    fonts.lfHeight = 20;
    setcolor(WHITE);
    strcpy(fonts.lfFaceName,"微软雅黑");
    fonts.lfQuality = ANTIALIASED_QUALITY;
    setfont(&fonts);
    return 0;
}

int InitBlock(){                            //初始化备选方块数组
    int i,j,k,blks;
    for( blks=0; blks<BLKNUM; blks++){
        blk[blks].x = 4;                          //初始化每个方块的初始坐标
        blk[blks].y = 0;
        k = 0;
        for( i=0; i<4; i++){
            for( j=0; j<4; j++){
                if( b[blks][i][j] ){
                    blk[blks].xy[k][0] = i;
                    blk[blks].xy[k][1] = j;
                    k++;
                }
            }
        }
        switch( blks/4 ){
            case 0: blk[blks].color = LIGHTCYAN;        // 0~3
                    break;
            case 1: blk[blks].color = LIGHTGREEN;        // 4~7
                    break;
            case 2: blk[blks].color = LIGHTBLUE;        // 8~11
                    break;
            case 3: blk[blks].color = LIGHTMAGENTA;        // 12~15
                    break;
            case 4: blk[blks].color = LIGHTRED;        // 16~19
                    break;
            case 5: blk[blks].color = LIGHTGRAY;        // 20~23
                    break;
            case 6: blk[blks].color = BROWN;
                    break;
            case 7: blk[blks].color = DARKGRAY;
                    break;
            case 8: blk[blks].color = CYAN;
                    break;
            default:blk[blks].color = WHITE;
                    break;
        }
    }
    return 0;
}

int ShowCube(){                                     //生成一个方块的同时,在右上角显示下一个方块的形态
    int left,top,i;
    randomize();
    k = next;
    next = random(CurBlkNum);
    // emerge next block
    block nextblk = blk[next];
    nextblk.x = 15;
    nextblk.y = 2;
    setfillcolor(BLACK);
    bar(350,100,500,300);                           // clear previous image
    xyprintf(330,100,"Next: ",next);
//    rectangle(350,120,500,220);
    setfillcolor(nextblk.color);
    for(i=0; i<4; i++){
        left = BX+(nextblk.x + nextblk.xy[i][0])*CS;
        top  = BY+(nextblk.y + nextblk.xy[i][1])*CS;
        bar(left+1,top+1,left+CS-1,top+CS-1);               //+,-1是为了不画到GRID上
    }

    // update a new block
    curblk = blk[k];
    if( !isOk(curblk) ){                            //满了
        DrawCube();
        Complete();
    }
    DrawCube();
    alive = 1;
    return 0;
}

int ShowCube(int x,int y){                                     //生成一个方块的同时,在右上角显示下一个方块的形态
    int left,top,i;
    randomize();
    k = next;
    next = random(CurBlkNum);
    // emerge next block
    block nextblk = blk[next];
    nextblk.x = 15;
    nextblk.y = 2;
    setfillcolor(BLACK);
    bar(350,100,500,300);                           // clear previous image
    xyprintf(330,100,"Next: ",next);
//    rectangle(350,120,500,220);
    setfillcolor(nextblk.color);
    for(i=0; i<4; i++){
        left = BX+(nextblk.x + nextblk.xy[i][0])*CS;
        top  = BY+(nextblk.y + nextblk.xy[i][1])*CS;
        bar(left+1,top+1,left+CS-1,top+CS-1);               //+,-1是为了不画到GRID上
    }
    // update a new block
    curblk = blk[k];
    curblk.x = x;
    curblk.y = y-1;
    if( !isOk(curblk) ){                            //满了
        DrawCube();
        Complete();
    }
    DrawCube();
    alive = 1;
    return 0;
}

int GameInitial(){
    cleardevice();
    SelfMadeBlocks((CurBlkNum/4));
    if( CurBlkNum < BLKNUM ) CurBlkNum += 4;
    LoadBmp();
    LoadMp3();
    MusicPlay();
    randomize();
    count = 0;
    ChangeTimes = 1;
    k = -1;
    next = random(CurBlkNum);
    InitBox();
    InitBlock();
    ShowBox();
    ShowScore();
    ShowCube();
    return 0;
}

int DrawCube(){                    //只需传入要描绘的方块序号(指针传入?)
    int left,top,k;
    setfillcolor(curblk.color);
    for(k=0; k<4; k++){
        box[curblk.x + curblk.xy[k][0]][curblk.y + curblk.xy[k][1]] = 1;
        left = BX+(curblk.x + curblk.xy[k][0])*CS;
        top  = BY+(curblk.y + curblk.xy[k][1])*CS;
//        xyprintf(500,k*20,"%d  %d",curblk.x + curblk.xy[k][0],curblk.y + curblk.xy[k][1]);
//        xyprintf(0,k*20,"left = %d, top = %d",left,top);
        bar(left+1,top+1,left+CS-1,top+CS-1);               //+,-1是为了不画到GRID上
    }
    return 0;
}

int ClearCube(){
    int left,top,k;
    setfillcolor(BLACK);
    for(k=0; k<4; k++){
        box[curblk.x + curblk.xy[k][0]][curblk.y + curblk.xy[k][1]] = 0;
        left = BX+(curblk.x + curblk.xy[k][0])*CS;
        top  = BY+(curblk.y + curblk.xy[k][1])*CS;
        bar(left+1,top+1,left+CS-1,top+CS-1);               //+,-1是为了不画到GRID上
    }
    return 0;
}

int isOk(block nextblk){         //传入下一步方块的属性,结合容器状态数组判断是否可进行下一步
    int flag = 1;               //默认可移动
    int i;
    for( i=0; i<4; i++){
        if( nextblk.x + nextblk.xy[i][0] >9 || nextblk.x + nextblk.xy[i][0] < 0
            || nextblk.y + nextblk.xy[i][1] > 19 ){                                          //越界

            flag = 0;
            break;
        }else if( box[nextblk.x + nextblk.xy[i][0]][nextblk.y + nextblk.xy[i][1]] ){            //有障碍
//            xyprintf(0,60,"Obstacle");
            flag = 0;
            break;
        }
    }
    return flag;
}

int MoveDown(){
    ClearCube();
    curblk.y += 1;
    if( !isOk(curblk) ){
        curblk.y -= 1;
        alive = 0;
    }
    DrawCube();
    return 0;
}
int MoveLeft(){
    ClearCube();
    curblk.x -= 1;
    if( !isOk(curblk) ){
        curblk.x += 1;
    }
    DrawCube();
    return 0;
}
int MoveRight(){
    ClearCube();
    curblk.x += 1;
    if( !isOk(curblk) ){
        curblk.x -= 1;
    }
    DrawCube();
    return 0;
}

int Rotate(){
    int type = k/4;              //当前方块的大类序号
    int num  = k%4;              //当前方块在大类中的序号
    num = (num+1)%4;             //把方块变为同一组的下一个
//    xyprintf(0,curblk.y*20,"k=%d,num=%d",k,num);
    block nextblk = blk[type*4+num];
    nextblk.x = curblk.x;
    nextblk.y = curblk.y;
    nextblk.color = curblk.color;
    ClearCube();
    if( isOk(nextblk) ){
        k = type*4+num;
        curblk = nextblk;
    }
    DrawCube();
    return 0;
}

int TurnNext(){
    if( ChangeTimes>0 ){
        int left,top,k,j;
        int x,y;
        x = curblk.x;
        y = curblk.y-2;
        j = 255;
        while( j-- ){
            Sleep(1);
            setfillcolor(EGERGB(j,j,j));
            for(k=0; k<4; k++){
                left = BX+(curblk.x + curblk.xy[k][0])*CS;
                top  = BY+(curblk.y + curblk.xy[k][1])*CS;
                bar(left+1,top+1,left+CS-1,top+CS-1);               //+,-1是为了不画到GRID上
            }
        }
        ClearCube();
        ShowCube(x,y);
        ChangeTimes--;
        ShowScore();
    }
    return 0;
}

int DelLine(){
    // 从最底层往上检查消行,若此次检查消除了行返回1,否则返回0;
    int x,i,j,flag,color,left,top;
    for( i=19; i>0; i--){          //控制行
        flag = 1;                   //默认为满行
        for( j=0; j<10; j++){       //控制列
            if( box[j][i]==0 ){
                flag = 0;
                break;
            }
        }
        if( flag ){                 //满行,需要消除,同时更新BOX状态
            j = 255;
            left = BX;
            top  = BY+i*CS;
            while( j-- ){
                Sleep(1);
                setfillcolor(EGERGB(j,j,j));
                bar(left+1,top+1,left+BSC-1,top+CS-1);
            }
            setcolor(EGERGB(80,80,80));             //Grid Color
            for( x=BX+CS; x<=BSC+BX; x+=CS){           //重新画竖线
                line(x,top+1,x,top+CS-1);
            }
            while( i>0 ){
                for( j=0; j<10; j++){
                    box[j][i] = box[j][i-1];
                    left = BX+j*CS;
                    top  = BY+i*CS;
                    color = getpixel(left+2,top-2);
                    setfillcolor(color);
                    bar(left+1,top+1,left+CS-1,top+CS-1);
                }
                i--;
            }
            count++;
            ChangeTimes++;

            return 1;
        }
    }
    return 0;
}

int ShowScore(){
    int completeness;                   //完成度
    setcolor(WHITE);
    xyprintf(330,480,"Score:");
    xyprintf(330,440,"Remain Changes       %d",ChangeTimes);
    rectangle(400,480,560,500);
    setfillcolor(GREEN);
    completeness = count*10 ;
//    count = 16;
    bar(400+1,480+1,400+completeness+1,500-1);
    if( count==16 ){
        Complete();
    }
    return 0;
}

int Complete(){
    char * str = (char*)malloc(sizeof(char)*5);
    inputbox_getline(" "," Play again ? (Y/N)",str,sizeof(str)/sizeof(*str));
//    while( *str!='Y' ){
//        inputbox_getline("WOCAO"," 你这就不玩了 ? (Y/N)",str,sizeof(str)/sizeof(*str));
//    }
    GameInitial();
    return 0;
}

int Restart(){
    cleardevice();
//    Help();
    GameInitial();
    return 0;
}

int SelfMadeBlocks(int n){       //当前所画砖块在数组b中的序号(指的是大序号,如b[0]~b[3]称为1号方块
    //自定砖块功能(鼠标操作)
    // Print Grid 4*4 (260,260,340,340)
    xyprintf(160,100,"设定一个自定义砖块(每通关一次可自选一个)");
    xyprintf(160,140,"选择任意四个方格作为砖块的形状,选好后按space确定");
    xyprintf(160,180,"左键单击选中，右键单击取消");

    int x,y;
    setcolor(WHITE);
    for( x=260; x<340; x+=CS ){
        line(x,260,x,340);
    }
    for( y=260; y<340; y+=CS ){
        line(260,y,340,y);
    }
    x = y = 260;
    rectangle(260,260,340,340);

    int coorX,coorY;                    //当前光标的坐标
    int blkcount,gridcount;
    int i,j,TempBox[4][4];
    // Initialize Box State
    for( i=0; i<4; i++){
        for( j=0; j<4; j++){
            TempBox[i][j] = 0;
        }
    }

    // Select Grids

    coorX = coorY = 0;
    blkcount = gridcount = 0;
    int ticks = 0;

    mouse_msg mos;
    while( blkcount < 1 ){
        mousepos(&x,&y);
//        xyprintf(0,0,"%d %d     %d %d",x,y,coorX,coorY);
        gridcount = 0;
        for( i=261; i<340; i+=CS){
            for( j=261; j<340; j+=CS){
                coorX = (i-260)/CS;
                coorY = (j-260)/CS;
                if( TempBox[coorX][coorY]==0 ){
                    setfillstyle(SOLID_FILL,BLACK);
                }
                else{
                    gridcount++;
                    setfillstyle(SOLID_FILL,LIGHTGREEN);
                }
                floodfill(i,j,WHITE);
            }
        }
        if( x>260 && x<340 && y>260 && y<340 ){
            if( TempBox[coorX][coorY]==0 )
                setfillstyle(SOLID_FILL, RED);
            else
                setfillstyle(SOLID_FILL, LIGHTGREEN);
            floodfill(x,y,WHITE);
        }
        if( mousemsg() ){                                       //检测到鼠标按下(此时x,y已经为鼠标坐标了)
            mos = getmouse();
            coorX = (x-260)/CS;
            coorY = (y-260)/CS;
            if( gridcount < 4 ){                                //可以画
                if( mos.flags == mouse_flag_left ){
                    TempBox[coorX][coorY] = 1;                  //改变当前方格的状态
                }else if(mos.flags == mouse_flag_right){
                    TempBox[coorX][coorY] = 0;
                }
            }
        }else{
            if( kbhit() ){
                key = getch();
                key = key & 0xff;
                if( key == SPACE && gridcount ==4 ){                //当前砖块画完了
                    blkcount++;
                }
            }
            Sleep(100);
        }
    }
    // Return Result
    n = n*4;
    for( i=0; i<4; i++){
        for( j=0; j<4; j++){
            b[n][i][j] = TempBox[i][j];
            b[n+1][i][3-j] = TempBox[i][j];
        }
    }
    n+=2;
    for( i=3; i>=0; i--){
        for( j=3; j>=0; j--){
            b[n][i][j] = TempBox[j][i];
            b[n+1][i][3-j] = TempBox[j][i];
        }
    }

    // Print Test
//    cleardevice();
//    n = 0;
//    for( i=0; i<4; i++){
//        for( j=0; j<4; j++){
//            xyprintf(i*20+100,j*20,"%d",b[n][i][j]);
//            xyprintf(i*20,j*20,"%d",b[n+1][i][j]);
//        }
//    }
//    getch();
//    n+=2;
//    for( i=3; i>=0; i--){
//        for( j=3; j>=0; j--){
//            xyprintf((3-i)*20+100,(3-j)*20,"%d",b[n][i][j]);
//            xyprintf((3-i)*20,(3-j)*20,"%d",b[n+1][i][j]);
//        }
//    }
//    getch();


    return 0;
}

int GameOver(){
    MusicStop();
    DeleteFile(ImagePath);
    DeleteFile(FilePath);
    xyprintf(280,200,"GAME OVER");
    return 0;
}

/* 定义方块(旋转的不算一种,因此个形状有4种)每个最大4*4 */

int b[BLKNUM][4][4] = {
		1, 1, 0, 0,
        1, 1, 0, 0,
        0, 0, 0, 0,
        0, 0, 0, 0,

        1, 1, 0, 0,
        1, 1, 0, 0,
        0, 0, 0, 0,
        0, 0, 0, 0,

        1, 1, 0, 0,
        1, 1, 0, 0,
        0, 0, 0, 0,
        0, 0, 0, 0,

        1, 1, 0, 0,
        1, 1, 0, 0,
        0, 0, 0, 0,
        0, 0, 0, 0,
// ---------以上为正方形,序号0~3---------//

       1, 1, 1, 1,
       0, 0, 0, 0,
       0, 0, 0, 0,
       0, 0, 0, 0,

       1, 0, 0, 0,
       1, 0, 0, 0,
       1, 0, 0, 0,
       1, 0, 0, 0,

       1, 1, 1, 1,
       0, 0, 0, 0,
       0, 0, 0, 0,
       0, 0, 0, 0,

       1, 0, 0, 0,
       1, 0, 0, 0,
       1, 0, 0, 0,
       1, 0, 0, 0,

//------------以上为直条,序号4~7---------------//

       1, 0, 0, 0,
       1, 1, 0, 0,
       0, 1, 0, 0,
       0, 0, 0, 0,

       0, 1, 1, 0,
       1, 1, 0, 0,
       0, 0, 0, 0,
       0, 0, 0, 0,

       1, 0, 0, 0,
       1, 1, 0, 0,
       0, 1, 0, 0,
       0, 0, 0, 0,

 	   0, 1, 1, 0,
       1, 1, 0, 0,
       0, 0, 0, 0,
       0, 0, 0, 0,
//-----------以上为Z形条A,序号8~11--------------//
       0, 1, 0, 0,
       1, 1, 0, 0,
       1, 0, 0, 0,
       0, 0, 0, 0,

       1, 1, 0, 0,
       0, 1, 1, 0,
       0, 0, 0, 0,
       0, 0, 0, 0,

       0, 1, 0, 0,
       1, 1, 0, 0,
       1, 0, 0, 0,
       0, 0, 0, 0,

       1, 1, 0, 0,
       0, 1, 1, 0,
       0, 0, 0, 0,
       0, 0, 0, 0,

//------------以上为Z形条B,序号12~15-------------//

       1, 1, 0, 0,
       0, 1, 0, 0,
       0, 1, 0, 0,
       0, 0, 0, 0,

       0, 0, 1, 0,
       1, 1, 1, 0,
       0, 0, 0, 0,
       0, 0, 0, 0,

       1, 0, 0, 0,
       1, 0, 0, 0,
       1, 1, 0, 0,
       0, 0, 0, 0,

       1, 1, 1, 0,
       1, 0, 0, 0,
       0, 0, 0, 0,
       0, 0, 0, 0,

//--------------以上为7形条A,序号16~19-------------//

       1, 1, 0, 0,
       1, 0, 0, 0,
       1, 0, 0, 0,
       0, 0, 0, 0,

       1, 1, 1, 0,
       0, 0, 1, 0,
       0, 0, 0, 0,
       0, 0, 0, 0,

       0, 1, 0, 0,
       0, 1, 0, 0,
       1, 1, 0, 0,
       0, 0, 0, 0,

       1, 0, 0, 0,
       1, 1, 1, 0,
       0, 0, 0, 0,
       0, 0, 0, 0,
//--------------以上为7形条B,序号为20~23--------//

       1, 1, 1, 0,
       0, 1, 0, 0,
       0, 0, 0, 0,
       0, 0, 0, 0,

       0, 1, 0, 0,
       1, 1, 0, 0,
       0, 1, 0, 0,
       0, 0, 0, 0,

       0, 1, 0, 0,
       1, 1, 1, 0,
       0, 0, 0, 0,
       0, 0, 0, 0,

       0, 1, 0, 0,
       0, 1, 1, 0,
       0, 1, 0, 0,
       0, 0, 0, 0,

//--------------以上为T形条,序号为24~27 --------//


       1, 0, 0, 0,
       0, 1, 0, 0,
       0, 0, 1, 0,
       0, 0, 0, 1,

       0, 0, 0, 1,
       0, 0, 1, 0,
       0, 1, 0, 0,
       1, 0, 0, 0,

       1, 0, 0, 0,
       0, 1, 0, 0,
       0, 0, 1, 0,
       0, 0, 0, 1,

       0, 0, 0, 1,
       0, 0, 1, 0,
       0, 1, 0, 0,
       1, 0, 0, 0,

//--------------以上为斜条,序号为28~31 --------//



       0, 1, 0, 0,
       1, 0, 1, 0,
       0, 1, 0, 0,
       0, 0, 0, 0,

       0, 1, 0, 0,
       1, 0, 1, 0,
       0, 1, 0, 0,
       0, 0, 0, 0,

       0, 1, 0, 0,
       1, 0, 1, 0,
       0, 1, 0, 0,
       0, 0, 0, 0,

       0, 1, 0, 0,
       1, 0, 1, 0,
       0, 1, 0, 0,
       0, 0, 0, 0,
//--------------以上为空心条,序号为32~35--------//

       0, 1, 0, 1,
       1, 0, 1, 0,
       0, 0, 0, 0,
       0, 0, 0, 0,

       1, 0, 1, 0,
       0, 1, 0, 1,
       0, 0, 0, 0,
       0, 0, 0, 0,

       0, 1, 0, 0,
       0, 0, 1, 0,
       0, 1, 0, 0,
       0, 0, 1, 0,

       0, 0, 1, 0,
       0, 1, 0, 0,
       0, 0, 1, 0,
       0, 1, 0, 0,
//--------------以上为蛇形条,序号为36~39--------//
};

