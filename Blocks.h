#define BLKNUM 40 /*方块数组大小(备选的方块形态总数,旋转后的算新的一种)*/
#define CURBLKNUM 28 /*当前使用的方块个数*/

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

