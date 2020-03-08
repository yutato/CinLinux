#include <stdio.h>
#include <stdbool.h>
#include <time.h>

#define DAYS_PER_WEEK	7
#define WEEK_HEADER "日 一 二 三 四 五 六"
// #define WEEK_HEADER "日  一  二  三  四  五  六"
int  year_of_birth,month_of_birth,day_of_birth;
int  days_in_month[]={0,31,28,31,30,31,30,31,31,30,31,30,31};
char *num_in_cn[10] = {"〇","一","二","三","四","五","六","七","八","九"};

// 判断是否为闰年(true)
bool isleapyear(int year)
{
    return (year% 4 == 0 && year%100 != 0 || year%400 == 0);
}

// 返回为一年中的第几天
int day_of_year(int y, int m, int d)
{
	int days  = 0;
	for(int month=1;month<m;month++){
		days += days_in_month[month];
	}

	if(isleapyear(y) && m > 2)
		return days + d + 1;
	else
		return days + d;
}

// 返回从公元元年元月元日的天数
int daycode(int y, int m, int d)
{
	int days = day_of_year(y,m,d);
	int tmp_y = y -1;
	return days + tmp_y*365 + tmp_y/4 - tmp_y/100 + tmp_y/400;
}

// 计算某一天是周几(0:星期日;1:星期一;...;6:星期六)
int weeklycode(int y,int m, int d)
{
	return daycode(y,m,d) % DAYS_PER_WEEK;
}

int gettm(int mode){
	time_t timep;
    struct  tm *p;
    time(&timep);
    p = gmtime(&timep);
	if(0==mode)
    	return p->tm_year+1900;
	if(1==mode)
		return p->tm_mon+1;
	if(2==mode)
		return p->tm_mday; 
}

// 打印一个月的日历
void pltmonth(int y, int m, int d)
{
	int daysinmonth[]={0,31,28,31,30,31,30,31,31,30,31,30,31};
	if(isleapyear(y))
		daysinmonth[2] = 29;

	printf("        %4d年%2d月\n",y,m);
	printf("%s\n",WEEK_HEADER);

	for(int i=0;i<weeklycode(y,m,1);i++)
		printf("    ");
	
	for(int i=1;i<=daysinmonth[m];i++){
		int w = weeklycode(y,m,i);
		if(6==w%DAYS_PER_WEEK){
			if(y==gettm(0) && m==gettm(1) && i==gettm(2))
				printf("\e[7m%2d\e[0m\n",i);
			else
				printf("%2d\n",i);
		}
		else{
			if(y==gettm(0) && m==gettm(1) && i==gettm(2))
				printf("\e[7m%2d\e[0m  ",i);	
			else
				printf("%2d  ",i);
		}
	}
	printf("\n");
}

void pltweek(int y, int m, int w)
{
	int first_day_of_month = weeklycode(y, m, 1);
    int i;
    //第一周特殊处理 
    if(1 == w){   
		for(i = 0; i < first_day_of_month; i++)
            printf("   ");
        
        for (i = 1; i + first_day_of_month < 8; i++)
            if(m==gettm(1) && i==gettm(2))
				printf("\e[1;42m%2d\e[0m ",i);
			else if(m<gettm(1) || m==gettm(1) && i<gettm(2))
				printf("\033[30m%2d\e[0m ",i);
			else
				printf("%2d ",i);
    }
	else{
		// 计算该月第week周的星期天是几号
		int first_date_of_week = (w-2) * 7 + (7 - first_day_of_month + 1);
    
		//最多打印七天，且最大值不能超过这个月的天数
		int daysinmonth[]={0,31,28,31,30,31,30,31,31,30,31,30,31};

		if(isleapyear(y))
			daysinmonth[2] = 29;
		int total_days = daysinmonth[m]; 
		if(first_date_of_week)
			for (i = 0; i < 7; i++){
				int now = i + first_date_of_week;   //当前打印的日期
				if (now > total_days)
					printf("   ");   //日期超过了，就打印空格 
				else 
					if(m==gettm(1) && now==gettm(2))
						printf("\e[1;42m%2d\e[0m ",now);
					else if(m<gettm(1) || m==gettm(1) && now<gettm(2))
						printf("\033[30m%2d\e[0m ",now);
					else
						printf("%2d ",now); 
			}
	} 
	
}

void pltcal(void)
{
	int y = gettm(0);
	int m = gettm(1);
	int d = gettm(2);
	int daysinmonth[]={0,31,28,31,30,31,30,31,31,30,31,30,31};

	if(isleapyear(y))
		daysinmonth[2] = 29;

	// printf("\t\t\t      %4d年\n",y);
	printf("----------------------------------------------------------------\n");
	int i, j, k;
    
    for(i = 0; i < 4; i++)
    {      
        for(j = 1; j <= 5; j++)
        {    
            if( 1 == j)
            {
                for(k = 1; k <= 3; k++)
                {
                    int month = i * 3 + k;
                    switch(month)
                    {
                        case 1: printf("\t\t一月");    break;
                        case 2: printf("\t      二月");     break;
                        case 3: printf("\t\t    三月");  break;
                        case 4: printf("\t\t四月");    break;
                        case 5: printf("\t      五月");     break;
                        case 6: printf("\t\t    六月");  break;
                        case 7: printf("\t\t七月");    break;
                        case 8: printf("\t      八月");     break;
                        case 9: printf("\t\t    九月");  break;
                        case 10: printf("\t\t十月");   break;
                        case 11: printf("\t    十一月");    break;
                        case 12: printf("\t\t  十二月"); break;
                    }
                    printf("     ");        
                }
				printf("\n%s  ",WEEK_HEADER);
                printf("%s  ", WEEK_HEADER);
                printf("%s\n", WEEK_HEADER);
            }
            for (k = 1; k <= 3; k++)
            {    
                pltweek(y, i * 3 + k, j);
                printf(" ");
                if (3 == k) printf("\n");
            }
            // printf("\n");
        }
		printf("\n");
    } 
    
}

void pltyear(int year)
{
    time_t timep;
    struct  tm *p;
    time(&timep);
    p = gmtime(&timep);
    int y_now = p->tm_year+1900;    
    for(int y=year_of_birth;y <= year;y++){
        if(!((y-year_of_birth)%11))
            printf("\n");
        if(y < y_now)
            printf("\033[30m" "%d" "\033[0m",y);
        else if(y == y_now)
            printf("\033[1;42m" "%d" "\033[0m",y);
        else 
            printf("%d",y);
        printf("  ");
    } 
    printf("\n");
}

void plttitle(int dl)
{
	printf("\n\t\t\t*死亡日历*\n");
	printf("\t\t\t\t       距您出生已经过去了%5d天\n",dl);
	printf("----------------------------------------------------------------");
}

int main(void)
{
	printf("输入您的出生年月日(如：1990 1 1)：");
	scanf("%d %d %d",&year_of_birth,&month_of_birth,&day_of_birth);
	int dayoflife = daycode(gettm(0),gettm(1),gettm(2))-daycode(year_of_birth,month_of_birth,day_of_birth);
	plttitle(dayoflife);
	pltyear(year_of_birth+100);
	pltcal();
}
