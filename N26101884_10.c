#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include <limits.h>
const int heightvalue[17]={10,10,40,60,80,90,90,90,90,60,50,45,40,30,20,15,15};//height�v��
const int countvalue[17]={130,130,90,60,40,30,20,10,9,8,7,6,5,4,3,2};//�U�����Ѥl�Ѿl�ƶq�v��
int doublecap = 0;//�����O�_��double_cap
typedef struct{

    int color[9][9];//�����ѽL�W�C��B�h�ơB��������m
    int layer[9][9];
    int type[9][9];
    int eval[9][9];
    //int dir;
    int w_type_num[3];//�զ�¦�U�����Ѿl�Ӽ�
    int b_type_num[3];
    int w_val;
    int b_val;
    int yourcolor;//�A���C��


}position;

typedef struct{

    int max_val;//�����{�bevaluatuion�̤j����
    int dest_x;//�����{�bevaluatuion�̤j������x,y
    int dest_y;
    int sour_x;
    int sour_y;

}evaluation;

//��l�ƹC���A�N�T��TXT�ɪ����Ū�i��
position * initialGame(char * argv[])
{
    //�}��
    FILE * fp;
    char *str;//�sargv[]
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
        //Ū�ɼg�J��m�}�C

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

//�����C���A�N�T��TXT�ɮ׼g�^�h
void endgame(position * pos,char * argv[])
{
    FILE * fp;//�}��
    char *str;//�sargv[]
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
        //�N��m�}�C�g��
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

//�˴��Y�i�H���槹�Ĥ@�B�J�Y�l��A�ĤG�B�J�A�Y�l�K�i�����ӧQ
void doublecap_detect_1(position * pos,int ori_x,int ori_y,int ori_i,int ori_j)
{
    int temp_type_count[3];//�Ψӭp�ƴ���type�ѤU�Ӽ�
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
    //�պ�Y�@�������p
    int temp_color = pos ->color[ori_i][ori_j];
    int temp_layer = pos ->layer[ori_i][ori_j];
    int temp_type = pos ->type[ori_i][ori_j];
    pos ->color[ori_i][ori_j] = pos->yourcolor;
    pos ->layer[ori_i][ori_j] = pos->layer[ori_x][ori_y];
    pos ->type[ori_i][ori_j] = pos->type[ori_x][ori_y];
    pos ->color[ori_x][ori_y] = 0;
    pos ->layer[ori_x][ori_y] = 0;
    pos ->type[ori_x][ori_y] = 0;
    int x,y;//x,y���ثe�j�M����m�q0.0�j��8,8//ycolor���A���C��
    x = 0 ;
    y = 0 ;

    while(1)
    {
        int i,j;
        if(pos ->color[x][y] == pos->yourcolor)
        {
            int c;//�O�ƪ��V��
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
                    //�ݬO�W�X��l�d��
                    if( i<0 || j<0 || i>8 || j>8 || pos ->color[i][j] == -1)
                        break;
                    //���|��0���I
                    if(pos ->color[i][j] == 0)
                        continue;
                        //capture�ҥH����ۦP�I�N���ਫ
                    if(pos ->color[i][j] == pos ->color[x][y])
                        break;
                        //capture�ҥH���줣�P�I�N��i��i�H��
                    if(pos ->color[i][j] != pos ->color[x][y] )
                    {
                            //���հ���
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
                            //���פp��ҥH���ਫ
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

    //�^�_
    temp_type_count[pos->type[ori_i][ori_j]-1]++;
    pos ->color[ori_x][ori_y] = pos ->color[ori_i][ori_j];
    pos ->layer[ori_x][ori_y] = pos ->layer[ori_i][ori_j];
    pos ->type[ori_x][ori_y] = pos ->type[ori_i][ori_j];
    pos ->color[ori_i][ori_j] = temp_color;
    pos ->layer[ori_i][ori_j] = temp_layer;
    pos ->type[ori_i][ori_j] = temp_type;
}

//�˴��O�_�ĤG�B�J�Y�l��iĹ�o�ӧQ
void doublecap_detect_2(position * pos,int i,int j,int x,int y)
{
    int temp_type_count[3];//�Ψӭp�ƴ���type�ѤU�Ӽ�
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

    if((temp_type_count[pos->type[i][j]-1]-1)<=0)//�ĤG�B�J�i�H�Y�NĹ�N�����Y
    {
        doublecap = 1;
        return;
    }

}

//�YEVALUATION�ȬۦP�h�ʷl�ӳ̤p�����Ӥl
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
    int c;//�O�ƽT�{��V��
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

//�p���m0,0��8,8��EVALUATION
evaluation * search_board(position * pos,int flag,char * argv[])
{
    evaluation * eval = malloc(sizeof(evaluation));
    int x,y;//x,y���ثe�j�M����m�q0.0�j��8,8//ycolor���A���C��
    x = 0 ;
    y = 0 ;
    int ori_x,ori_y;//�O���@�}�l����m(�o�̱q0,0�}�l�j)//�Y�O�j���^��0,0�N��S���i����
    ori_x = x;
    ori_y = y;
    eval->max_val = INT_MIN;//����evaluation fun�����G�A�ڭ̰ʭȳ̤j�������ӴѤl
    //int dir = 0;//������V
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
            int c;//�O�ƪ��V��
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
                    //�ݬO�W�X��l�d��
                    if( i<0 || j<0 || i>8 || j>8 || pos ->color[i][j] == -1)
                        break;
                    //���|��0���I
                    if(pos ->color[i][j] == 0)
                        continue;
                    if(flag == 1)
                    {
                         //capture�ҥH����ۦP�I�N���ਫ
                        if(pos ->color[i][j] == pos ->color[x][y])
                            break;
                        //capture�ҥH���줣�P�I�N��i��i�H��
                        if(pos ->color[i][j] != pos ->color[x][y] )
                        {
                            //���հ���
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

                                    //�ݦY�l���թζ�
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
                                    //���զ��S�����e��evaluation function�j
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
                                    //�Y�Oevaluation function�ۦP�h�������M�I��
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
                            //���פp��ҥH���ਫ
                            else
                            {
                                break;
                            }

                        }
                    }
                    if(flag == 2)
                    {
                         //enhence�ҥH���줣�ۦP�I�N���ਫ
                        if(pos ->color[i][j] != pos ->color[x][y])
                        break;
                        //enhence�ҥH����ۦP�I�N��i��i�H��
                        if(pos ->color[i][j] == pos ->color[x][y] )
                        {

                                int tmp_eval = 0;
                                int a=0,b=0;
                                int sub;
                                //�ݦY�l���թζ�
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


                                //���զ��S�����e��evaluation function�j
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
                                //�Y�Oevaluation function�ۦP�h�������M�I��
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

//�Y�l
position * cap(position * pos,char * argv[])
{
    //�}�l�`�C���I��6�Ӥ�V
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

//�֤l
position * en(position * pos,char * argv[])
{
    //�}�l�`�C���I��6�Ӥ�V
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
