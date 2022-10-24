#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include <limits.h>
const int heightvalue[17]={10,10,40,60,80,90,90,90,90,60,50,45,40,30,20,15,15};//height權重
const int countvalue[17]={130,130,90,60,40,30,20,10,9,8,7,6,5,4,3,2};//各種類棋子剩餘數量權重
int doublecap = 0;//紀錄是否做double_cap
typedef struct{

    int color[9][9];//紀錄棋盤上顏色、層數、種類的位置
    int layer[9][9];
    int type[9][9];
    int eval[9][9];
    //int dir;
    int w_type_num[3];//白色黑色各種類剩餘個數
    int b_type_num[3];
    int w_val;
    int b_val;
    int yourcolor;//你的顏色


}position;

typedef struct{

    int max_val;//紀錄現在evaluatuion最大的直
    int dest_x;//紀錄現在evaluatuion最大的直的x,y
    int dest_y;
    int sour_x;
    int sour_y;

}evaluation;

//初始化遊戲，將三個TXT檔的資料讀進來
position * initialGame(char * argv[])
{
    //開檔
    FILE * fp;
    char *str;//存argv[]
    int *p;
    position * pos = malloc(sizeof(position));
    int i;
    if(strcmp(argv[1],"white")==0)
        pos->yourcolor = 1;
    if(strcmp(argv[1],"black")==0)
        pos->yourcolor = 2;
    for(i=0;i<3;i++)
    {
        pos->w_type_num[i]=0;
        pos->b_type_num[i]=0;
    }
    for(i = 0; i < 3; i++)
    {
        if(i == 0)
        {
            str = argv[3];
            p = &(pos ->color[0][0]);
        }

        if(i == 1)
        {
            str = argv[4];
            p = &(pos ->layer[0][0]);
        }
        if(i == 2)
        {
            str = argv[5];
            p = &(pos ->type[0][0]);
        }

        fp = fopen(str,"r");
        if(fp == NULL)
        {
            printf("file is not exist\n");
            system("pause");
            exit(0);
        }
        //讀檔寫入位置陣列

        char strline[30];
        int *temp = &(pos ->color[0][0]);
        while(!feof(fp))
        {
            fgets(strline,30,fp);
            char *test = strtok(strline, ",");

            while (test != NULL) {
                int num = atoi(test);
                *p =num;
                if(i == 2 && num!=-1)
                {
                    if(*temp == 1)
                    {
                        pos->w_type_num[num - 1]++;
                    }
                    if(*temp == 2)
                    {
                        pos->b_type_num[num - 1]++;
                    }

                }
                p++;
                temp++;
                test = strtok(NULL, ",");
            }

        }

        fclose(fp);
    }

    int j;
    pos->w_val=0;
    pos->b_val=0;
    for(i=0;i<9;i++)
    {
        for(j =0; j<9;j++)
        {
            pos->eval[i][j] = -1;
            if(pos->color[i][j]==0 || pos->color[i][j]==-1)
                continue;
            if(pos->color[i][j] == 1)
            {
                pos->w_val= pos->w_val+heightvalue[pos->layer[i][j]] * countvalue[pos->w_type_num[pos->type[i][j]-1]];
            }
            if(pos->color[i][j] == 2)
            {
                pos->b_val=pos->b_val+heightvalue[pos->layer[i][j]] * countvalue[pos->w_type_num[pos->type[i][j]-1]];
            }
        }
    }

    return pos;
}

//結束遊戲，將三個TXT檔案寫回去
void endgame(position * pos,char * argv[])
{
    FILE * fp;//開檔
    char *str;//存argv[]
    int *p;
    int i;
    for(i =0; i<3; i++)
    {
        if(i == 0)
        {
            str = argv[3];
            p = &(pos ->color[0][0]);
        }

        if(i == 1)
        {
            str = argv[4];
            p = &(pos ->layer[0][0]);
        }
        if(i == 2)
        {
            str = argv[5];
            p = &(pos ->type[0][0]);
        }
        fp = fopen(str,"w");
        if(fp == NULL)
        {
            printf("file is not exist\n");
            system("pause");
            exit(0);
        }
        //將位置陣列寫檔
        int i;
        for(i = 0; i<9; i++)
        {
            int j;
            for(j = 0; j<9; j++)
            {
                fprintf(fp,"%d",*p);
                p++;
                if(j == 8)
                    continue;
                fprintf(fp,",");
            }
            if(i == 8)
                continue;
            fprintf(fp,"\n");
        }
    }
}

void freemem(position * pos)
{
    free(pos);
}

//檢測若可以執行完第一步驟吃子後，第二步驟再吃子便可直接勝利
void doublecap_detect_1(position * pos,int ori_x,int ori_y,int ori_i,int ori_j)
{
    int temp_type_count[3];//用來計數棋類type剩下個數
    if(pos ->yourcolor == 2)
    {
        int t;
        for(t = 0; t<3; t++)
        {
            temp_type_count[t] = pos->w_type_num[t];
        }
    }
    if(pos ->yourcolor == 1)
    {
        int t;
        for(t = 0; t<3; t++)
        {
            temp_type_count[t] = pos->b_type_num[t];
        }
    }
    temp_type_count[pos->type[ori_i][ori_j]-1]--;
    //試算吃一次的狀況
    int temp_color = pos ->color[ori_i][ori_j];
    int temp_layer = pos ->layer[ori_i][ori_j];
    int temp_type = pos ->type[ori_i][ori_j];
    pos ->color[ori_i][ori_j] = pos->yourcolor;
    pos ->layer[ori_i][ori_j] = pos->layer[ori_x][ori_y];
    pos ->type[ori_i][ori_j] = pos->type[ori_x][ori_y];
    pos ->color[ori_x][ori_y] = 0;
    pos ->layer[ori_x][ori_y] = 0;
    pos ->type[ori_x][ori_y] = 0;
    int x,y;//x,y為目前搜尋的位置從0.0搜到8,8//ycolor為你的顏色
    x = 0 ;
    y = 0 ;

    while(1)
    {
        int i,j;
        if(pos ->color[x][y] == pos->yourcolor)
        {
            int c;//記數表方向用
            for(c = 1; c<=6;c++)
            {
                i = x;
                j = y;
                while(1)
                {
                    if(c == 1)
                        i--;
                    if(c == 2)
                        j++;
                    if(c == 3)
                    {
                        i++;
                        j++;
                    }
                    if(c == 4)
                        i++;
                    if(c == 5)
                        j--;
                    if(c == 6)
                    {
                        i--;
                        j--;
                    }
                    //看是超出格子範圍
                    if( i<0 || j<0 || i>8 || j>8 || pos ->color[i][j] == -1)
                        break;
                    //路徑為0的點
                    if(pos ->color[i][j] == 0)
                        continue;
                        //capture所以撞到相同點代表不能走
                    if(pos ->color[i][j] == pos ->color[x][y])
                        break;
                        //capture所以撞到不同點代表可能可以走
                    if(pos ->color[i][j] != pos ->color[x][y] )
                    {
                            //測試高度
                        if(pos ->layer[x][y]>=pos ->layer[i][j])
                        {
                            if((temp_type_count[pos->type[i][j]-1]-1)<=0)
                            {
                                doublecap = 1;
                                temp_type_count[pos->type[ori_i][ori_j]-1]++;
                                pos ->color[ori_x][ori_y] = pos ->color[ori_i][ori_j];
                                pos ->layer[ori_x][ori_y] = pos ->layer[ori_i][ori_j];
                                pos ->type[ori_x][ori_y] = pos ->type[ori_i][ori_j];
                                pos ->color[ori_i][ori_j] = temp_color;
                                pos ->layer[ori_i][ori_j] = temp_layer;
                                pos ->type[ori_i][ori_j] = temp_type;
                                return;
                            }
                            break;
                        }
                            //高度小於所以不能走
                        else
                        {
                            break;
                        }
                    }
                }

            }
            y++;
            if(y>=9)
            {
                y=0;
                x++;
                if(x>=9)
                    x=0;
            }
            if(x == 0&&y == 0)
            break;
        }
        else{
            y++;
            if(y>=9)
            {
                y=0;
                x++;
                if(x>=9)
                    x=0;
            }
            if(x == 0&&y == 0)
                break;

        }
    }

    //回復
    temp_type_count[pos->type[ori_i][ori_j]-1]++;
    pos ->color[ori_x][ori_y] = pos ->color[ori_i][ori_j];
    pos ->layer[ori_x][ori_y] = pos ->layer[ori_i][ori_j];
    pos ->type[ori_x][ori_y] = pos ->type[ori_i][ori_j];
    pos ->color[ori_i][ori_j] = temp_color;
    pos ->layer[ori_i][ori_j] = temp_layer;
    pos ->type[ori_i][ori_j] = temp_type;
}

//檢測是否第二步驟吃子後可贏得勝利
void doublecap_detect_2(position * pos,int i,int j,int x,int y)
{
    int temp_type_count[3];//用來計數棋類type剩下個數
    if(pos ->yourcolor == 2)
    {
        int t;
        for(t = 0; t<3; t++)
        {
            temp_type_count[t] = pos->w_type_num[t];
        }
    }
    if(pos ->yourcolor == 1)
    {
        int t;
        for(t = 0; t<3; t++)
        {
            temp_type_count[t] = pos->b_type_num[t];
        }
    }

    if((temp_type_count[pos->type[i][j]-1]-1)<=0)//第二步驟可以吃就贏就直接吃
    {
        doublecap = 1;
        return;
    }

}

//若EVALUATION值相同則動損耗最小的那個子
int check_danger(position * pos,int ori_i,int ori_j,int x,int y,int flag)
{
    int record_color = pos ->color[ori_i][ori_j];
    int record_layer = pos ->layer[ori_i][ori_j];
    int record_type = pos ->type[ori_i][ori_j];

    pos ->color[ori_i][ori_j] = pos->yourcolor;
    if(flag == 1)
        pos ->layer[ori_i][ori_j] = pos->layer[x][y];
    if(flag == 2)
        pos ->layer[ori_i][ori_j] = pos->layer[x][y]+pos ->layer[ori_i][ori_j];

    pos ->type[ori_i][ori_j] = pos->type[x][y];
    pos ->color[x][y] = 0;
    pos ->layer[x][y] = 0;
    pos ->type[x][y] = 0;

    int min_danger = INT_MAX -1;
    int danger = INT_MAX -1;
    int i,j;
    int c;//記數確認方向用
    for(c =1; c<=6;c++)
    {
        i = ori_i;
        j = ori_j;

        while(1)
        {
            danger = INT_MAX-1;
            if(c == 1)
                i--;
            if(c == 2)
                j++;
            if(c == 3)
            {
                i++;
                j++;
            }
            if(c == 4)
                i++;
            if(c == 5)
                j--;
            if(c == 6)
            {
                i--;
                j--;
            }




            if( i<0 || j<0 || i>8 || j>8 || pos ->color[i][j] == -1)
                break;
            if(pos ->color[i][j] == 0)
                continue;
            if(pos ->color[i][j] == pos ->color[ori_i][ori_j])
                break;

            if(pos ->color[i][j] != pos ->color[ori_i][ori_j] )
            {
                if(pos ->layer[ori_i][ori_j]>pos ->layer[i][j])
                {
                    break;
                }
                else
                {
                    if(pos->yourcolor == 1)
                        danger = 0 - (heightvalue[pos->layer[ori_i][ori_j]] * countvalue[pos->w_type_num[pos->type[ori_i][ori_j]-1]]);
                    if(pos->yourcolor == 2)
                        danger = 0 - (heightvalue[pos->layer[ori_i][ori_j]] * countvalue[pos->b_type_num[pos->type[ori_i][ori_j]-1]]);
                    if(flag == 2)
                    {
                        pos ->color[x][y] = pos ->color[ori_i][ori_j];
                        if(flag==1)
                                pos ->layer[x][y] = pos ->layer[ori_i][ori_j];
                        if(flag==2)
                                pos ->layer[x][y] = pos ->layer[ori_i][ori_j] - record_layer;
                        pos ->type[x][y] = pos ->type[ori_i][ori_j];
                        pos ->color[ori_i][ori_j] = record_color;
                        pos ->layer[ori_i][ori_j] = record_layer;
                        pos ->type[ori_i][ori_j] = record_type;

                         min_danger = INT_MIN;

                         return min_danger;

                    }
                    if(danger < min_danger)
                        min_danger = danger;
                    break;
                }

            }

        }

    }

    pos ->color[x][y] = pos ->color[ori_i][ori_j];
    if(flag==1)
            pos ->layer[x][y] = pos ->layer[ori_i][ori_j];
    if(flag==2)
            pos ->layer[x][y] = pos ->layer[ori_i][ori_j] - record_layer;
    pos ->type[x][y] = pos ->type[ori_i][ori_j];
    pos ->color[ori_i][ori_j] = record_color;
    pos ->layer[ori_i][ori_j] = record_layer;
    pos ->type[ori_i][ori_j] = record_type;

    return min_danger;
}

//計算位置0,0到8,8的EVALUATION
evaluation * search_board(position * pos,int flag,char * argv[])
{
    evaluation * eval = malloc(sizeof(evaluation));
    int x,y;//x,y為目前搜尋的位置從0.0搜到8,8//ycolor為你的顏色
    x = 0 ;
    y = 0 ;
    int ori_x,ori_y;//記錄一開始的位置(這裡從0,0開始搜)//若是搜完回到0,0代表沒有可走的
    ori_x = x;
    ori_y = y;
    eval->max_val = INT_MIN;//紀錄evaluation fun的結果，我們動值最大的的那個棋子
    //int dir = 0;//紀錄方向
    eval->sour_x = -1;
    eval->sour_y = -1;
    eval->dest_x = -1;
    eval->dest_y = -1;
    int last_danger = INT_MIN;
    while(1)
    {
        //printf("####################\n");
        //printf("source %d %d\n",x,y);
        int i,j;
        if(pos ->color[x][y] == pos->yourcolor)
        {
            int c;//記數表方向用
            for(c = 1; c<=6;c++)
            {
                i = x;
                j = y;
                while(1)
                {
                    if(c == 1)
                        i--;
                    if(c == 2)
                        j++;
                    if(c == 3)
                    {
                        i++;
                        j++;
                    }
                    if(c == 4)
                        i++;
                    if(c == 5)
                        j--;
                    if(c == 6)
                    {
                        i--;
                        j--;
                    }
                    //看是超出格子範圍
                    if( i<0 || j<0 || i>8 || j>8 || pos ->color[i][j] == -1)
                        break;
                    //路徑為0的點
                    if(pos ->color[i][j] == 0)
                        continue;
                    if(flag == 1)
                    {
                         //capture所以撞到相同點代表不能走
                        if(pos ->color[i][j] == pos ->color[x][y])
                            break;
                        //capture所以撞到不同點代表可能可以走
                        if(pos ->color[i][j] != pos ->color[x][y] )
                        {
                            //測試高度
                            if(pos ->layer[x][y]>=pos ->layer[i][j])
                            {
                                if(strcmp(argv[2],"2")==0)
                                {
                                    doublecap_detect_2(pos,i,j,x,y);
                                    if(doublecap == 1)
                                    {
                                        eval->max_val = INT_MAX;
                                        eval->sour_x = x;
                                        eval->sour_y = y;
                                        eval->dest_x = i;
                                        eval->dest_y = j;
                                        return eval;
                                    }
                                    break;
                                }
                                else
                                {
                                    doublecap_detect_1(pos,x,y,i,j);
                                    if(doublecap == 1)
                                    {
                                        eval->max_val = INT_MAX;
                                        eval->sour_x = x;
                                        eval->sour_y = y;
                                        eval->dest_x = i;
                                        eval->dest_y = j;
                                        return eval;
                                    }
                                    int tmp_eval = 0;
                                    int a=0,b=0;
                                    int sub;

                                    //看吃子為白或黑
                                    if(pos->yourcolor == 1)
                                    {
                                        sub = heightvalue[pos->layer[i][j]] * countvalue[pos->b_type_num[pos->type[i][j]-1]];
                                    }
                                    else
                                    {
                                        sub = heightvalue[pos->layer[i][j]] * countvalue[pos->w_type_num[pos->type[i][j]-1]];
                                    }

                                    tmp_eval = sub;
                                    int danger = check_danger(pos,i,j,x,y,1);
                                    //測試有沒有比當前的evaluation function大
                                    if(tmp_eval>eval->max_val)
                                    {
                                        last_danger = danger;
                                        eval->max_val = tmp_eval;
                                        eval->sour_x = x;
                                        eval->sour_y = y;
                                        eval->dest_x = i;
                                        eval->dest_y = j;
                                       // dir = 1;
                                    }
                                    //若是evaluation function相同則走較不危險的
                                    if(tmp_eval == eval->max_val && danger > last_danger)
                                    {
                                     //   printf("danandlastdan %d , %d",danger,last_danger);
                                        last_danger = danger;
                                        eval->max_val = tmp_eval;
                                        eval->sour_x = x;
                                        eval->sour_y = y;
                                        eval->dest_x = i;
                                        eval->dest_y = j;
                                    }
                                    break;
                                }

                            }
                            //高度小於所以不能走
                            else
                            {
                                break;
                            }

                        }
                    }
                    if(flag == 2)
                    {
                         //enhence所以撞到不相同點代表不能走
                        if(pos ->color[i][j] != pos ->color[x][y])
                        break;
                        //enhence所以撞到相同點代表可能可以走
                        if(pos ->color[i][j] == pos ->color[x][y] )
                        {

                                int tmp_eval = 0;
                                int a=0,b=0;
                                int sub;
                                //看吃子為白或黑
                                if(pos->yourcolor == 1)
                                {
                                    sub = heightvalue[pos->layer[i][j]+pos->layer[x][y]] * countvalue[pos->w_type_num[pos->type[x][y]-1]]
                                        - heightvalue[pos->layer[i][j]] * countvalue[pos->w_type_num[pos->type[i][j]-1]] ;
                                }
                                else
                                {
                                    sub = heightvalue[pos->layer[i][j]+pos->layer[x][y]] * countvalue[pos->w_type_num[pos->type[x][y]-1]]
                                        - heightvalue[pos->layer[i][j]] * countvalue[pos->w_type_num[pos->type[i][j]-1]] ;
                                }

                                tmp_eval = sub;

                                int danger = check_danger(pos,i,j,x,y,2);
                                if(danger == INT_MIN)
                                    tmp_eval = INT_MIN;


                                //測試有沒有比當前的evaluation function大
                                if(tmp_eval>eval->max_val)
                                {
                                    last_danger = danger;
                                    eval->max_val = tmp_eval;
                                    eval->sour_x = x;
                                    eval->sour_y = y;
                                    eval->dest_x = i;
                                    eval->dest_y = j;
                                   // dir = 1;
                                }
                                //若是evaluation function相同則走較不危險的
                                if(tmp_eval == eval->max_val && danger > last_danger)
                                {
                                 //   printf("danandlastdan %d , %d",danger,last_danger);
                                    last_danger = danger;
                                    eval->max_val = tmp_eval;
                                    eval->sour_x = x;
                                    eval->sour_y = y;
                                    eval->dest_x = i;
                                    eval->dest_y = j;
                                }
                                break;
                        }
                    }
                }

            }
            y++;
            if(y>=9)
            {
                y=0;
                x++;
                if(x>=9)
                    x=0;
            }
            if(ori_x == x&&ori_y == y)
            break;
        }
        else{
            y++;
            if(y>=9)
            {
                y=0;
                x++;
                if(x>=9)
                    x=0;
            }
            if(ori_x == x&&ori_y == y)
                break;

        }

    }

    return eval;
}

void freeeval(evaluation * eval)
{
    free(eval);
}

void printtest(evaluation * eval)
{
    printf("doublecap =  %d  \n",doublecap);
    printf("source_x : %d \n",eval->sour_x);
    printf("source_x : %d \n",eval->sour_y);
    printf("dest_x : %d \n",eval->dest_x);
    printf("dest_y : %d \n",eval->dest_y);
}

//吃子
position * cap(position * pos,char * argv[])
{
    //開始蒐每個點的6個方向
    evaluation * eval;
    eval = search_board(pos,1,argv);
    //printtest(eval);
    if(eval->max_val!=INT_MIN)
    {
        pos ->color[eval->dest_x][eval->dest_y] = pos->yourcolor;
        pos ->layer[eval->dest_x][eval->dest_y] = pos->layer[eval->sour_x][eval->sour_y];
        pos ->type[eval->dest_x][eval->dest_y] = pos->type[eval->sour_x][eval->sour_y];
        pos ->color[eval->sour_x][eval->sour_y] = 0;
        pos ->layer[eval->sour_x][eval->sour_y] = 0;
        pos ->type[eval->sour_x][eval->sour_y] = 0;
    }
    else
    {
        printf("------------cannot capture-------------\n");
    }
    freeeval(eval);
    return pos;
}

//併子
position * en(position * pos,char * argv[])
{
    //開始蒐每個點的6個方向
    evaluation * eval;
    eval = search_board(pos,1,argv);
    if(doublecap == 1)
    {
        //printf("do cap\n");
        //printtest(eval);
        pos ->color[eval->dest_x][eval->dest_y] = pos->yourcolor;
        pos ->layer[eval->dest_x][eval->dest_y] = pos->layer[eval->sour_x][eval->sour_y];
        pos ->type[eval->dest_x][eval->dest_y] = pos->type[eval->sour_x][eval->sour_y];
        pos ->color[eval->sour_x][eval->sour_y] = 0;
        pos ->layer[eval->sour_x][eval->sour_y] = 0;
        pos ->type[eval->sour_x][eval->sour_y] = 0;
        freeeval(eval);
        return pos;
    }
    if(doublecap == 0)
    {
        //printf("do enhence\n");
        eval = search_board(pos,2,argv);
        //printtest(eval);
    }

    if(eval->max_val!=INT_MIN)
    {
        pos ->color[eval->dest_x][eval->dest_y] = pos->yourcolor;
        pos ->layer[eval->dest_x][eval->dest_y] = pos ->layer[eval->dest_x][eval->dest_y]+pos->layer[eval->sour_x][eval->sour_y];
        pos ->type[eval->dest_x][eval->dest_y] = pos->type[eval->sour_x][eval->sour_y];
        pos ->color[eval->sour_x][eval->sour_y] = 0;
        pos ->layer[eval->sour_x][eval->sour_y] = 0;
        pos ->type[eval->sour_x][eval->sour_y] = 0;
    }
    else
    {
        printf("------------cannot enhence-------------\n");
    }
    freeeval(eval);
    return pos;
}

position * gamemove(position * pos,char * argv[],int argc)
{
    srand(time(NULL));
    if(strcmp(argv[2],"1")==0)
    {
        pos = cap(pos,argv);
    }
    if(strcmp(argv[2],"2")==0)
    {
        pos = en(pos,argv);
    }

    return pos;
}

int main(int argc, char * argv[])
{

    //a white 1 chessColor.txt chessLayer.txt chessType.txt stepLog.txt
    //a white 2 chessColor.txt chessLayer.txt chessType.txt stepLog.txt
    //a black 1 chessColor.txt chessLayer.txt chessType.txt stepLog.txt
    //a black 2 chessColor.txt chessLayer.txt chessType.txt stepLog.txt
    position *pos;
    pos = initialGame(argv);
    pos = gamemove(pos,argv,argc);
    endgame(pos,argv);
    freemem(pos);

    return 0;
}
