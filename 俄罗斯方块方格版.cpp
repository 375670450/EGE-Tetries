//#define _DEBUG
#include <graphics.h>
#include <time.h>
#include <stdio.h>
#include <conio.h>
#include <mmsystem.h>
#include <tchar.h>

#pragma message("����ɹ�")


/*  ���Ľ�:
    1.BGM,������Ч(�����޷�����?):ԭ��:mus.close()
    2.������Ч-���(�ı�EGERGB����)
    3.һ�䵽�׹���
    4.����:���ж������������ͬһ������(��ͬ����)���ѡ�����в�ͬ�ĺ���?
    5.restart����-���
    6.ע��setcolor��setfillcolor������
    7.��λ��ʾ-���
    8.��ѡש�鹦��(������)
    9.������ɫ,����
    10.���ʱ�ķ���- (����ɫ����ʱҪдsleep(1)
    11.���̣߳�
    12.ռ���ڴ�̫��
    13.Sleep�������Ϊˢ����?
    14.�ܷ������ݽṹ�Ż�CPUռ��?
    15.AI�ж����·��
    16.���������: while( kbhit() ) getch();       //�൱�����������
*/

#define BOX_R 20 /*��������*/
#define BOX_C 10 /*��������*/

#define CS 20 /*����Ĵ�С*/
#define BX 100 /*�������Ͻǵ�x����*/
#define BY 100 /*�������Ͻǵ�y����*/
#define BSR 20*20 /*�����Ĵ�С*/
#define BSC 10*20 /*�����Ĵ�С*/
#define INIT_X 80 /*�����ʼ��X����*/
#define INIT_Y 10 /*�����ʼ��Y����*/
#define BLKNUM 40 /*���������С(��ѡ�ķ�����̬����,��ת������µ�һ��)*/
#define CURBLKNUM 28 /*��ǰʹ�õķ������*/

/*����Ҫ��Ӧ�İ���(getch()�������ص����������ĺ���λ(ʮ������))*/
#define DOWN  VK_DOWN /*����*/
#define UP  VK_UP /*����*/
#define LEFT  VK_LEFT /*����*/
#define RIGHT  VK_RIGHT /*����*/
#define ESC  VK_ESCAPE /*�˳���*/
#define SPACE VK_SPACE /*�ո��*/
#define TAB VK_TAB
#define HOME VK_HOME
#define CONTROL VK_CONTROL
#define NUM1 VK_NUMPAD1
#define NUM2 VK_NUMPAD2
#define NUM3 VK_NUMPAD3
#define TIMER 0x1c          /*ʱ���жϺ�*/
#define _CPPARGS

/*�����Ǽ���ɨ����,TC�е�getkey()�����ŷ���)*/
//#define DOWN  0X5000  /*����*/
//#define UP  0X4800 /*����*/
//#define LEFT  0X4B00  /*����*/
//#define RIGHT  0X4D00 /*����*/
//#define ESC  0X11b   /*�˳���*/

#define LSCORE  100      /*����һ�����õķ���*/
#define SCX  200  /*��ʾ������λ�ã�X����*/
#define SCY  100  /*��ʾ������λ�ã�Y����*/

typedef struct {
    int x;      //���ϽǸ�������
    int y;
    int xy[4][2];    //ÿһ��ĸ�������(��Ϊÿ�ַ���̶���4�����ӹ���)
    int color;
}block;

extern int b[BLKNUM][4][4];             //������̬����,��ϸ������
block blk[BLKNUM];                      //��ѡ��������
int box[BOX_C][BOX_R];                  //�ݻ�ÿ������״̬


int X=INIT_X, Y=INIT_Y; /*����ĵ�ǰλ�ã�ȫ�ֱ���*/
int key,keyPress; /*���水����Ϣ��ȫ�ֱ���*/
int k,next,nextColor; /*��ǰ�ķ������ʹ���(1~24),ֻ�����ɺ���תʱ�ı�(��ȫ�ֱ����ͱ����˸��������ݽṹ����һ����ŵ��鷳*/
int count = 0;/*����������¼�Ѿ�����������*/
char score[100];
int stop = 0;
int alive = 0;
int MaxTicks = 250;
int CurBlkNum = CURBLKNUM;          /*��ǰʹ�õķ������*/

int dstX,dstY;
int res;                /*��¼��ǰAI�Ƿ��ҵ���Ŀ�ĵ�*/
int AIMode = 0;
int ChangeTimes;        /*���Ա任�Ĵ���*/
block curblk;           /*��ǰ�����ķ���(ʡȥ����ָ��)*/
MUSIC mus;
char FilePath[_MAX_PATH];
char ImagePath[_MAX_PATH];


/*
LPSTR��32bitָ��ָ��һ���ַ�����ÿ���ַ�ռ1�ֽ�
LPCSTR:32-bitָ��ָ��һ�����ַ�����ÿ���ַ�ռ1�ֽ�
LPCTSTR:32-bitָ��ָ��һ�����ַ���,ÿ�ַ�����ռ1�ֽڻ�2�ֽڣ�ȡ����Unicode�Ƿ���
LPTSTR:32-bitָ��ÿ�ַ�����ռ1�ֽڻ�2�ֽڣ�ȡ����Unicode�Ƿ���
*/
int ExtractResource(LPCTSTR strDstFile, LPCTSTR strResType, LPCTSTR strResName);    //�ͷŴ���EXE�еĶ�ý����Դ
int LoadMp3();                      //��ȡMP3�ļ�
int LoadBmp();                      //��ȡbmp�ļ�
int MusicPlay();                    //��������
int MusicStop();                    //��������
int Help();                         //��������
int InitBox();                      //��ʼ������
int ShowBox();                      //��ʾ����
int InitBlock();                    //��ʼ������
int InitFonts();                    //��ʼ������
int ShowCube();                     //����һ���µķ���
int ShowCube(int x,int y);          //��ĳһ��λ�ò���һ���µķ���
int GameInitial();                  //��Ϸ��ʼ��
int DrawCube();                     //����һ������
int ClearCube();                    //����һ������
int isOk(block nextblk);            //������һ�����������,�������״̬�����ж��Ƿ�ɽ�����һ��
int MoveDown();                     //�����ƶ�
int MoveLeft();                     //�����ƶ�
int MoveRight();                    //�����ƶ�
int Rotate();                       //��ת
int TurnNext();                     //(ԭ���淨)ֱ�ӱ��γ���һ��Ҫ�����ķ���
int DelLine();                      //����һ��
int ShowScore();                    //��ʾ����
int Complete();                     //�������
int Restart();                      //���¿�ʼ
int SelfMadeBlocks(int n);          //(ԭ���淨)�Զ���һ������
int GameOver();                     //��Ϸʧ��
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

// �Զ�����AI��δ���
int VirtualRotate(block * pdstblk,int * pcurnum){
    int type = *pcurnum/4;              //��ǰ����Ĵ������
    int num  = *pcurnum%4;              //��ǰ�����ڴ����е����
    *pcurnum = (num+1)%4;             //�ѷ����Ϊͬһ�����һ��
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
            if( key==SPACE ){                   //�����ϸ���,���⴦��
                if( TempBox[coorX][coorY] == 1 ){           //�Ѿ�������

                }else{                          //δ����

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

//-------------------------------------����MAIN����-------------------------------------------//

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
            while( DelLine() );             //��������ֱ��DelLine����0
            ShowCube();
            ShowScore();
            while( kbhit() ) getch();       //�൱�����������
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
            key = key & 0xff;                       //��ΪVK_UP��Щ����ֻȡ16�����еĺ���λ
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

//-------------------------------------����MAIN����-------------------------------------------//

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
 /*��ȡָ��ģ���е���Դ�ļ�(ͨ������������)
 ������
		strDstFile:		Ŀ���ļ�������ȡ����Դ�����������
		strResType:		��Դ���ͣ�
		strResName:		��Դ���ƣ�
 ����ֵ��
		true: ִ�гɹ���
		false: ִ��ʧ�ܡ�
*/
    HANDLE hFile = ::CreateFile(strDstFile, GENERIC_WRITE, NULL, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_TEMPORARY, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
		return 0;
    //���Ҳ���ȡ�ļ�,����(����)���ڴ�,������Դ��С
/* FindResource������һ��ָ����ģ���ж�λ��ָ������Դ��
     HRSRC FindResource(
     HMODULE hModule,        //����������Դ��ģ����������ǳ�����������ΪNULL
     LPCTSTR lpName,         //��������Դ���ƻ���ԴID
     LPCTSTR lpType          //��Դ���ͣ�������Ҳ���������Լ�ָ������Դ����
     );
     LoadResource��������ָ������Դ���ص��ڴ浱�У�
     HGLOBAL LoadResource(
     HMODULE hModule,                //ģ������ͬ��
     HRSRC hResInfo          //��Ҫ���ص���Դ���������Ҳ����FindResource�ķ���ֵ
     );
*/
    HRSRC hRes   = ::FindResource(NULL,strResName,strResType);
    HGLOBAL hMem = ::LoadResource(NULL,hRes);
    DWORD dwSize = ::SizeofResource(NULL,hRes);
    //д���ļ�
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
    char s[300];        //������ɵ���ʱ�ļ�������·��,�Ա�׼�������һ���ַ�����(ʵ���ô�������,����ֻ��FilePath
    ::GetTempPath(_MAX_PATH,FilePath);      //��ȡһ�������ͷ���ʱ�ļ���Ŀ¼
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
    if (mus.GetPlayStatus() == MUSIC_MODE_STOP) { // ����ֹͣ�˾����²���
            // Play����������1��ʾ��ʼ���ŵĵط�������2�ǽ����ĵط�
            // ���������������д���͵�ǰλ�ÿ�ʼ����ֻ���һ����ʾ���ŵ�����Ϊֹ
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
    for( y=BY; y<=BSR+BY; y+=CS){           //����
        line(BX,y,BX+BSC,y);
    }
    for( x=BX; x<=BSC+BX; x+=CS){           //����
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
    strcpy(fonts.lfFaceName,"΢���ź�");
    fonts.lfQuality = ANTIALIASED_QUALITY;
    setfont(&fonts);
    return 0;
}

int InitBlock(){                            //��ʼ����ѡ��������
    int i,j,k,blks;
    for( blks=0; blks<BLKNUM; blks++){
        blk[blks].x = 4;                          //��ʼ��ÿ������ĳ�ʼ����
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

int ShowCube(){                                     //����һ�������ͬʱ,�����Ͻ���ʾ��һ���������̬
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
        bar(left+1,top+1,left+CS-1,top+CS-1);               //+,-1��Ϊ�˲�����GRID��
    }

    // update a new block
    curblk = blk[k];
    if( !isOk(curblk) ){                            //����
        DrawCube();
        Complete();
    }
    DrawCube();
    alive = 1;
    return 0;
}

int ShowCube(int x,int y){                                     //����һ�������ͬʱ,�����Ͻ���ʾ��һ���������̬
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
        bar(left+1,top+1,left+CS-1,top+CS-1);               //+,-1��Ϊ�˲�����GRID��
    }
    // update a new block
    curblk = blk[k];
    curblk.x = x;
    curblk.y = y-1;
    if( !isOk(curblk) ){                            //����
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

int DrawCube(){                    //ֻ�贫��Ҫ���ķ������(ָ�봫��?)
    int left,top,k;
    setfillcolor(curblk.color);
    for(k=0; k<4; k++){
        box[curblk.x + curblk.xy[k][0]][curblk.y + curblk.xy[k][1]] = 1;
        left = BX+(curblk.x + curblk.xy[k][0])*CS;
        top  = BY+(curblk.y + curblk.xy[k][1])*CS;
//        xyprintf(500,k*20,"%d  %d",curblk.x + curblk.xy[k][0],curblk.y + curblk.xy[k][1]);
//        xyprintf(0,k*20,"left = %d, top = %d",left,top);
        bar(left+1,top+1,left+CS-1,top+CS-1);               //+,-1��Ϊ�˲�����GRID��
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
        bar(left+1,top+1,left+CS-1,top+CS-1);               //+,-1��Ϊ�˲�����GRID��
    }
    return 0;
}

int isOk(block nextblk){         //������һ�����������,�������״̬�����ж��Ƿ�ɽ�����һ��
    int flag = 1;               //Ĭ�Ͽ��ƶ�
    int i;
    for( i=0; i<4; i++){
        if( nextblk.x + nextblk.xy[i][0] >9 || nextblk.x + nextblk.xy[i][0] < 0
            || nextblk.y + nextblk.xy[i][1] > 19 ){                                          //Խ��

            flag = 0;
            break;
        }else if( box[nextblk.x + nextblk.xy[i][0]][nextblk.y + nextblk.xy[i][1]] ){            //���ϰ�
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
    int type = k/4;              //��ǰ����Ĵ������
    int num  = k%4;              //��ǰ�����ڴ����е����
    num = (num+1)%4;             //�ѷ����Ϊͬһ�����һ��
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
                bar(left+1,top+1,left+CS-1,top+CS-1);               //+,-1��Ϊ�˲�����GRID��
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
    // ����ײ����ϼ������,���˴μ���������з���1,���򷵻�0;
    int x,i,j,flag,color,left,top;
    for( i=19; i>0; i--){          //������
        flag = 1;                   //Ĭ��Ϊ����
        for( j=0; j<10; j++){       //������
            if( box[j][i]==0 ){
                flag = 0;
                break;
            }
        }
        if( flag ){                 //����,��Ҫ����,ͬʱ����BOX״̬
            j = 255;
            left = BX;
            top  = BY+i*CS;
            while( j-- ){
                Sleep(1);
                setfillcolor(EGERGB(j,j,j));
                bar(left+1,top+1,left+BSC-1,top+CS-1);
            }
            setcolor(EGERGB(80,80,80));             //Grid Color
            for( x=BX+CS; x<=BSC+BX; x+=CS){           //���»�����
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
    int completeness;                   //��ɶ�
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
//        inputbox_getline("WOCAO"," ����Ͳ����� ? (Y/N)",str,sizeof(str)/sizeof(*str));
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

int SelfMadeBlocks(int n){       //��ǰ����ש��������b�е����(ָ���Ǵ����,��b[0]~b[3]��Ϊ1�ŷ���
    //�Զ�ש�鹦��(������)
    // Print Grid 4*4 (260,260,340,340)
    xyprintf(160,100,"�趨һ���Զ���ש��(ÿͨ��һ�ο���ѡһ��)");
    xyprintf(160,140,"ѡ�������ĸ�������Ϊש�����״,ѡ�ú�spaceȷ��");
    xyprintf(160,180,"�������ѡ�У��Ҽ�����ȡ��");

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

    int coorX,coorY;                    //��ǰ��������
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
        if( mousemsg() ){                                       //��⵽��갴��(��ʱx,y�Ѿ�Ϊ���������)
            mos = getmouse();
            coorX = (x-260)/CS;
            coorY = (y-260)/CS;
            if( gridcount < 4 ){                                //���Ի�
                if( mos.flags == mouse_flag_left ){
                    TempBox[coorX][coorY] = 1;                  //�ı䵱ǰ�����״̬
                }else if(mos.flags == mouse_flag_right){
                    TempBox[coorX][coorY] = 0;
                }
            }
        }else{
            if( kbhit() ){
                key = getch();
                key = key & 0xff;
                if( key == SPACE && gridcount ==4 ){                //��ǰש�黭����
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

/* ���巽��(��ת�Ĳ���һ��,��˸���״��4��)ÿ�����4*4 */

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
// ---------����Ϊ������,���0~3---------//

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

//------------����Ϊֱ��,���4~7---------------//

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
//-----------����ΪZ����A,���8~11--------------//
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

//------------����ΪZ����B,���12~15-------------//

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

//--------------����Ϊ7����A,���16~19-------------//

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
//--------------����Ϊ7����B,���Ϊ20~23--------//

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

//--------------����ΪT����,���Ϊ24~27 --------//


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

//--------------����Ϊб��,���Ϊ28~31 --------//



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
//--------------����Ϊ������,���Ϊ32~35--------//

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
//--------------����Ϊ������,���Ϊ36~39--------//
};

