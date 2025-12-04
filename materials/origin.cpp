#include<bits/stdc++.h>
#include<windows.h>
//#include<conio.h>
using namespace std;
string text[100]={
/*0-9*/		"[Q] 玩家档案","[A] 经典模式","[S] 趣味模式","[D] 无限模式","[F] 游戏设置","错误","该功能未开放","普  通  菜  单","人 类 数 量","电 脑 数 量",
/*10-19*/	"电 脑 难 度","用 牌 套 数","[A] 返回主页","[S] 开始游戏","[D] 高级设置","随 机","简 单","普 通","困 难","高  级  菜  单",
/*20-29*/	"玩 家","人 类","电 脑","[A] 普通设置","玩家总人数不能小于2或大于14","提示","若玩家之间出现空缺，将进行顺位调整\n是否开始游戏？","所需牌数超过可用牌数，是否使用额外牌组？\n此操作可能导致程序崩溃，否则提前结束牌局。","余牌","前牌",
/*30-39*/	"顺序","红","黄","绿","蓝","黑","跳过","反转","+2","转色",
/*40-49*/	"+4","顺时针","逆时针","余","张","第","页","共","页","抽牌",
/*50-59*/	"UNO!","检举","出牌","抽了","张牌作起始牌，现在起始牌是","","玩家","抽了","张牌","所持牌数2张以内才可喊UNO",
/*60-69*/	"检举出","位玩家没有喊UNO","所出的牌不符合要求","上一页","下一页","请选择颜色：[Q]红色  [W]黄色  [E]绿色  [R]蓝色","你抽到了 "," ，是否出牌？[Y/N]","牌  局  结  算","难  度 ： ",
/*70-79*/	"平  均  分 ： ","排名","玩家","原始分","赋分","积分","（电脑）","共  得  经  验 ： ","共  得  积  分 ： ","[S] 再来一局",
/*80-89*/	"抽并出了一张牌","[ESC] 暂停游戏","[D] 继续游戏"};
int optnum=0,optclass[4]={1,1,0,1},optadvan[16]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},optspe[1]={0},playertype[14]={0,0,0,0,0,0,0,0,0,0,0,0,0,0},robottype[14]={0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int notdealnum,notdealcard[2000],holdnum[14],holdcard[14][2000],playednum=0,playedcard[2000],rule[1]={0};bool UNO[14]={0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int drawtype=0,drawnum=0,dealer,previous,page,againtype;bool clockwise;string message="";
int lastcolor[14]={},misscolor[14]={};
void print(int times,int length,string left,char filling,string right,string middle,bool wrap)
{
	for(int a=0;a<times;a++)
	{
		cout<<left;
		if(middle.length()==0) for(int b=0;b<length-left.length()-right.length();b++) cout<<filling;
		else
		{
			for(int b=0;b<floor((length-middle.length())/2.0)-left.length();b++) cout<<filling;
			cout<<middle;
			for(int b=0;b<ceil((length-middle.length())/2.0)-right.length();b++) cout<<filling;
		}
		cout<<right;
		if(wrap==true) cout<<endl;
	}
}
int keydetect()
{
    HANDLE handle=GetStdHandle(STD_INPUT_HANDLE);
    INPUT_RECORD inrec[128];
    DWORD byteread;
    while(true)
    {
        if(ReadConsoleInput(handle,inrec,128,&byteread))
        {
            for(DWORD i=0;i<byteread;i++)
            {
                if(inrec[i].EventType==KEY_EVENT)
                {
                    if(inrec[i].Event.KeyEvent.bKeyDown)
                    {
                        WORD code=inrec[i].Event.KeyEvent.wVirtualKeyCode;
                        int spec=inrec[i].Event.KeyEvent.uChar.AsciiChar;
                        return spec==0?-code:code;
                    }
                }
            }
        }
    }
}
string swap(int type,int input)
{
	switch(type)
	{
		case 1:if(input==0) return "■"; else return "□";break;
		case 2:if(input==0) return "<"; else return " ";break;
		case 3:if(input==0) return ">"; else return " ";break;
		case 4:return to_string(input);break;
		case 5:return to_string(input);break;
		case 6:for(int a=0;a<4;a++) if(input==a) return text[15+a];break;
		case 7:return to_string(input);break;
		case 8:if(input==0) return "";if(input==1) return text[21];if(input==2) return text[22];
		case 9:return to_string(notdealnum);break;
		case 10:
		{
			string firsthalf,secondhalf;
			if(input==999) return "";
			for(int a=1;a<=5;a++) if(input/100==a) firsthalf=text[30+a];
			if(input%100<10) secondhalf=to_string(input%100);
			else for(int a=10;a<15;a++) if(input%100==a) secondhalf=text[26+a];
			return firsthalf+"_"+secondhalf;
		}break;
		case 11:return clockwise?text[41]:text[42];break;
		case 12:if(UNO[input]==1) return "U N O"; else if(playertype[input]!=0) return text[43]+" "+to_string(holdnum[input])+" "+text[44]; else return "";break;
		case 13:if(playertype[input]==1) return text[45]+" "+to_string(page+1)+" "+text[46]+"/"+text[47]+" "+to_string(int(ceil(holdnum[input]/10.0)))+" "+text[48]; else return "";
		case 14:if(optnum-input==-1) return "■"; else return to_string(input%10);
	}
}
void messageset(int type,int input1,int input2)
{
	switch(type)
	{
		case 1:message=text[53]+to_string(input1)+text[54]+swap(10,input2)+text[55];break;
		case 2:message=text[56]+to_string((input1+1)/10)+to_string((input1+1)%10)+text[57]+to_string(input2)+text[58];break;
		case 3:message=text[59];break;
		case 4:if(!(playertype[dealer]==2&&input1==0)) message=text[60]+to_string(input1)+text[61];break;
		case 5:message=text[62];break;
		case 6:message=text[65];break;
		case 7:message=text[66]+swap(10,input1)+text[67];break;
		case 8:message=text[56]+to_string((input1+1)/10)+to_string((input1+1)%10)+text[80];break;
	}
}
void next()
{
	do
	{
		dealer+=clockwise?1:-1;
		dealer=(dealer+14)%14;
	}while(playertype[dealer]==0);
}
bool judge(int cardcode)
{
	if(drawtype==0) if(cardcode/100==previous/100||cardcode%100==previous%100||cardcode/100==5) return 1;
	if(drawtype==1) if(cardcode%100==12||cardcode==514) return 1;
	if(drawtype==2) if(cardcode==514) return 1;
	return 0;
}
void home();
void general_menu(bool special);
void advance_menu(bool special);
void archivemanege();
void prepare(bool advance,bool special);
void special();
void game(bool special);
void pause();
void discard(int playerID,int cardcode);
void robotdiscard();
void ending();
void display()
{
	for(int a=0;a<14;a++) if(holdnum[a]==0&&playertype[a]!=0) ending();
	for(int a=0;a<14;a++) if((holdnum[a]>2&&playertype[a]!=0)||playertype[a]==0) UNO[a]=0;
	system("cls");
	print(1,120,"+-"+text[81],'-',"+","",1);
	print(1,120,"|",' ',"|","",1);
	cout<<"|  ";print(6,19,"  /",'-',"\\  ","",0);cout<<"  |"<<endl;
	cout<<"|  ";for(int a=0;a<6;a++) print(1,19,"  |"+swap(1,dealer-a),' ',text[20]+"0"+to_string(a+1)+"|  ","",0); cout<<"  |"<<endl;
	cout<<"|  ";for(int a=0;a<6;a++) print(1,19,"  |",' ',"|  ",swap(8,playertype[a]),0); cout<<"  |"<<endl;
	cout<<"|  ";for(int a=0;a<6;a++) print(1,19,"  |",' ',"|  ",swap(12,a),0); cout<<"  |"<<endl;
	cout<<"|  ";print(6,19,"  \\",'-',"/  ","",0);cout<<"  |"<<endl;
	print(1,120,"|",' ',"|","",1);
	print(1,24,"|",' '," ","",0);print(1,72,"/",'-',"\\","",0);print(1,24," ",' ',"|","",1);
	print(1,25,"|    /",'-',"\\    |","",0);print(1,70," ",' '," "," ",0);print(1,25,"|    /",'-',"\\    |","",1);
	print(1,25,"|    |"+swap(1,dealer-13),' ',text[20]+to_string(14)+"|    |","",0);for(int a=0;a<3;a++){print(1,24-2*(a%2)," ",' '," ",text[28+a]+": "+swap(9+a,previous),0);}print(1,25,"|    |"+swap(1,dealer-6),' ',text[20]+"0"+to_string(7)+"|    |","",1);//24,22,24
	print(1,25,"|    |",' ',"|    |",swap(8,playertype[13]),0);print(1,70," ",' '," "," ",0);print(1,25,"|    |",' ',"|    |",swap(8,playertype[6]),1);
	print(1,25,"|    |",' ',"|    |",swap(12,13),0);print(1,70," ",' '," ",message,0);print(1,25,"|    |",' ',"|    |",swap(12,6),1);
	print(1,25,"|    \\",'-',"/    |","",0);print(1,70," ",' '," "," ",0);print(1,25,"|    \\",'-',"/    |","",1);
	print(1,24,"|",' '," ","",0);print(1,72,"\\",'-',"/","",0);print(1,24," ",' ',"|","",1);
	print(1,120,"|",' ',"|","",1);
	cout<<"|  ";print(6,19,"  /",'-',"\\  ","",0);cout<<"  |"<<endl;
	cout<<"|  ";for(int a=12;a>6;a--) print(1,19,"  |"+swap(1,dealer-a),' ',text[20]+to_string((a+1)/10)+to_string((a+1)%10)+"|  ","",0); cout<<"  |"<<endl;
	cout<<"|  ";for(int a=12;a>6;a--) print(1,19,"  |",' ',"|  ",swap(8,playertype[a]),0); cout<<"  |"<<endl;
	cout<<"|  ";for(int a=12;a>6;a--) print(1,19,"  |",' ',"|  ",swap(12,a),0); cout<<"  |"<<endl;
	cout<<"|  ";print(6,19,"  \\",'-',"/  ","",0);cout<<"  |"<<endl;
	print(1,120,"|",' ',"|","",1);
	print(1,120,"+",'-',"+","",1);
	print(1,120,"|",' ',"|",swap(13,dealer),1);
	print(1,10,"|",' '," ","[←]",0);for(int a=1;a<11;a++) print(1,10," ",' '," ","["+swap(14,a)+"]",0); print(1,10," ",' ',"|","[→]",1);
	print(1,10,"|",' '," ",text[63],0);for(int a=0;a<10;a++) print(1,10," ",' '," ",swap(10,playertype[dealer]==1?holdcard[dealer][page*10+a]:999),0); print(1,10," ",' ',"|",text[64],1);
	print(1,120,"|",' ',"|","",1);
	print(1,120,"|",' ',"|","[A] "+text[49]+"  [S] "+text[50]+"  [D] "+text[51],1);
	print(1,120,"|",' ',"|","[Space]"+text[52],1);
	print(1,120,"+",'-',"+","",0);
}
int main()
{
	srand(time(0));
	system("mode con cols=121 lines=31");
	SetWindowLongPtrA(GetConsoleWindow(), GWL_STYLE, GetWindowLongPtrA(GetConsoleWindow(),GWL_STYLE)& ~WS_SIZEBOX & ~WS_MAXIMIZEBOX);
	home();
	return 0;
}
void home()
{
	while(true)
	{
		system("cls");
		print(1,120,"+-"+text[0],'-',"+","",1);
		print(5,120,"|",' ',"|","",1);
		print(1,120,"|",' ',"|","U    U      N    N       OOOO ",1);
		print(1,120,"|",' ',"|","U    U      NN   N      OO  OO",1);
		print(1,120,"|",' ',"|","U    U      N N  N      O    O",1);
		print(1,120,"|",' ',"|","U    U      N  N N      O    O",1);
		print(1,120,"|",' ',"|","UU  UU      N   NN      OO  OO",1);
		print(1,120,"|",' ',"|"," UUUU       N    N       OOOO ",1);
		print(5,120,"|",' ',"|","",1);
		for(int a=1;a<=4;a++)
		{
			print(1,120,"|",' ',"|",text[a],1);
			print(1,120,"|",' ',"|","",1);
		}
		print(4,120,"|",' ',"|","",1);
		print(1,120,"+-"+text[4],'-',"Ver.0.0_0-+","",0);
		switch(keydetect())
		{
			case 'A':optnum=0;general_menu(0);break;
			case 'S':MessageBox(NULL,text[6].data(),text[5].data(),MB_OK|MB_ICONSTOP);break;
			case 'D':MessageBox(NULL,text[6].data(),text[5].data(),MB_OK|MB_ICONSTOP);break;
			case 'F':MessageBox(NULL,text[6].data(),text[5].data(),MB_OK|MB_ICONSTOP);break;
			case 'Q':MessageBox(NULL,text[6].data(),text[5].data(),MB_OK|MB_ICONSTOP);break;
		}
	}
}
void general_menu(bool special)
{
	while(true)
	{
		system("cls");
		optclass[0]=(optclass[0]+15)%15;optclass[1]=(optclass[1]+15)%15;optclass[2]=(optclass[2]+4)%4;optclass[3]=(optclass[3]+11)%11;
		print(1,120,"+",'-',"+","",1);
		print(3,120,"|",' ',"|","",1);
		print(1,120,"|",' ',"|",text[7],1);
		print(1,120,"|",' ',"|","[↑][↓][←][→]",1);
		for(int a=8;a<12;a++)
		{
			print(3,120,"|",' ',"|","",1);
			print(1,60,"|",' ',swap(1,a-8-optnum)+text[a]+":","",0);
			print(1,15,swap(2,a-8-optnum),' ',swap(3,a-8-optnum),swap(a-4,optclass[a-8]),0);
			print(1,45," ",' ',"|","",1);
		}
		print(3,120,"|",' ',"|","",1);
		print(1,120,"|",' ',"|",text[12]+"  "+text[13]+"  "+text[14],1);
		print(3,120,"|",' ',"|","",1);
		print(1,120,"+",'-',"+","",0);
		switch(keydetect())
		{
			case -38:optnum=(optnum+4-1)%4;break;
			case -40:optnum=(optnum+4+1)%4;break;
			case -37:optclass[optnum]--;break;
			case -39:optclass[optnum]++;break;
			case 'A':return;break;
			case 'S':againtype=0;prepare(0,special);break;
			case 'D':optnum=0;advance_menu(special);break;
		}
	}
}
void advance_menu(bool special)
{
	while(true)
	{
		system("cls");
		for(int a=0;a<14;a++) optadvan[a]=(optadvan[a]+3)%3;
		optadvan[14]=(optadvan[14]+4)%4;optadvan[15]=(optadvan[15]+11)%11;
		print(1,120,"+",'-',"+","",1);
		print(1,120,"|",' ',"|","",1);
		print(1,120,"|",' ',"|",text[19],1);
		print(1,120,"|",' ',"|","[↑][↓][←][→]",1);
		print(2,120,"|",' ',"|","",1);
		for(int a=1;a<8;a++)
		{
			print(1,20,"|",' ',swap(1,a-optnum-1)+text[20]+"0"+to_string(a)+":","",0);
			print(1,15,swap(2,a-optnum-1),' ',swap(3,a-optnum-1),swap(8,optadvan[a-1]),0);
			print(1,5," ",' ',"|","",0);
			print(1,20,"|",' ',swap(1,a-optnum+6)+text[20]+to_string((a+7)/10)+to_string((a+7)%10)+":","",0);
			print(1,15,swap(2,a-optnum+6),' ',swap(3,a-optnum+6),swap(8,optadvan[a+6]),0);
			print(1,5," ",' ',"|","",0);
			if(a==3) {print(1,20,"|",' ',swap(1,optnum-14)+text[10]+":","",0);print(1,15,swap(2,optnum-14),' ',swap(3,optnum-14),swap(6,optadvan[14]),0);print(1,5," ",' ',"|","",1);}
			else if(a==5) {print(1,20,"|",' ',swap(1,optnum-15)+text[11]+":","",0);print(1,15,swap(2,optnum-15),' ',swap(3,optnum-15),swap(7,optadvan[15]),0);print(1,5," ",' ',"|","",1);}
			else print(1,40,"|",' ',"|","",1);
			for(int b=0;b<2;b++) if(a!=7){print(3,40,"|",' ',"|","",0);cout<<endl;}
		}
		print(2,120,"|",' ',"|","",1);
		print(1,120,"|",' ',"|",text[23]+"  "+text[13],1);
		print(1,120,"|",' ',"|","",1);
		print(1,120,"+",'-',"+","",0);
		switch(keydetect())
		{
			case -38:optnum=(optnum+16-1)%16;break;
			case -40:optnum=(optnum+16+1)%16;break;
			case -37:optadvan[optnum]--;break;
			case -39:optadvan[optnum]++;break;
			case 'A':optnum=0;return;break;
			case 'S':againtype=1;prepare(1,special);break;
		}
	}
}
void special()
{

}
void addcard(int time)
{
	for(int z=0;z<time;z++)
	for(int a=0;a<4;a++)
	{
		notdealcard[notdealnum+25*a]=(a+1)*100;
		for(int b=1;b<=9;b++) {notdealcard[notdealnum+25*a+2*b-1]=(a+1)*100+b;notdealcard[notdealnum+25*a+2*b]=(a+1)*100+b;}
		for(int b=0;b<3;b++) {notdealcard[notdealnum+25*a+2*b+19]=(a+1)*100+10+b;notdealcard[notdealnum+25*a+2*b+20]=(a+1)*100+10+b;}
		for(int b=0;b<2;b++) {notdealcard[notdealnum+2*a+100]=513;notdealcard[notdealnum+2*a+101]=514;}
	}
	notdealnum+=108*time;
}
bool disdrawcard(int playerID,int cardcode)
{
	bool yes=-1;
	if(playertype[playerID]==1)
	{
		messageset(7,cardcode,0);
		display();
		while(true)
		{
			switch(keydetect())
			{
				case 'Y':messageset(8,playerID,0);return 1;break;
				case 'N':return 0;break;
			}
		}
	}else
	{
		if(robottype[playerID]==1) if(rand()%2) yes=1; else yes=0;
		if(robottype[playerID]==2) yes=1;
		int nextone=dealer+clockwise?1:-1;
		while(playertype[nextone]==0) nextone+=clockwise?1:-1;
		if(robottype[playerID]==3) if(playertype[nextone]==1||UNO[playerID]) yes=1;
		else if(playertype[nextone]==2&&UNO[nextone]&&cardcode%100<10&&cardcode/100==lastcolor[nextone]) yes=1;
		else if(playertype[nextone]==2&&!UNO[nextone]) yes=1;
		else yes=0;
	}
	if(yes) {lastcolor[playerID]=cardcode/100;messageset(8,playerID,0);return 1;} else return 0;
}
int changecolor()
{
	misscolor[dealer]=previous/100;
	if(playertype[dealer]==1)
	{
		messageset(6,0,0);
		display();
		message="";
		while(true)
		{
			switch(keydetect())
			{
				case 'Q':return lastcolor[dealer]=1;
				case 'W':return lastcolor[dealer]=2;
				case 'E':return lastcolor[dealer]=3;
				case 'R':return lastcolor[dealer]=4;
			}
		}
	}
	else
	{
		int raw[4]={},sorted[4]={},biggest;
		for(int a=0;a<holdnum[dealer];a++)
		{
			int temp=holdcard[dealer][a];
			if(temp/100<5) if(temp%100<10) raw[temp/100-1]+=temp%100;
			else raw[temp/100-1]+=20;
		}
		for(int a=0;a<4;a++) sorted[a]=raw[a];
		sort(sorted,sorted+4);
		for(int a=0;a<4;a++) if(raw[a]==sorted[3]) {biggest=a+1;break;}
		if(robottype[dealer]==1) return lastcolor[dealer]=rand()%4+1;
		if(robottype[dealer]==2) return lastcolor[dealer]=biggest;
		int nextone=dealer+clockwise?1:-1;
		while(playertype[nextone]==0) nextone+=clockwise?1:-1;
		if(robottype[dealer]==3) if(playertype[nextone]==1&&UNO[nextone]) return lastcolor[dealer]=misscolor[nextone];
		else if(playertype[nextone]==2&&UNO[nextone]) return lastcolor[dealer]=lastcolor[nextone];
		else return lastcolor[dealer]=biggest;
	}
}
void dealing(int playerID,int dealnum)
{
	messageset(2,playerID,dealnum);
	if(dealnum>notdealnum)
	{
		for(int a=0;a<playednum;a++) notdealcard[notdealnum+a]=playedcard[a];
		notdealnum+=playednum;playednum=0;
		for(int a=0;a<2000;a++) playedcard[a]=999;
		if(dealnum>notdealnum) if(MessageBox(NULL,text[27].data(),text[25].data(),MB_YESNO|MB_ICONWARNING)==IDYES) while(dealnum>notdealnum) addcard(1);
		else ending();
	}
	sort(notdealcard,notdealcard+2000);
	if(dealnum==1)
	{
		int temp=rand()%notdealnum;
		int tempcard=notdealcard[temp];
		notdealnum--;notdealcard[temp]=999;
		sort(notdealcard,notdealcard+2000);
		bool valid=0;bool useit=0;
		if(judge(tempcard)) {valid=1;useit=disdrawcard(playerID,tempcard);} else misscolor[playerID]=previous/100;
		if(!valid||!useit) {holdcard[playerID][holdnum[playerID]]=tempcard;holdnum[playerID]++;}
		if(useit)
		{
			if(tempcard%100<10) ;
			if(tempcard%100==10) next();
			if(tempcard%100==11) clockwise=!clockwise;
			if(tempcard%100==12) {drawtype=1;drawnum+=2;}
			previous=tempcard;
			if(tempcard==513) {previous=changecolor()*100+98;}
			if(tempcard==514) {previous=changecolor()*100+98;drawtype=2;drawnum+=4;}
		}
	}
	else for(int a=0;a<dealnum;a++)
	{
		int b=rand()%notdealnum;
		holdcard[playerID][holdnum[playerID]]=notdealcard[b];
		notdealcard[b]=999;
		notdealnum--;
		holdnum[playerID]++;
		sort(notdealcard,notdealcard+2000);
	}
	sort(holdcard[playerID],holdcard[playerID]+2000);
	drawtype=0;drawnum=0;
	next();
}
void prepare(bool advance,bool special)
{
	int cardset=1;
	for(int a=0;a<14;a++) {playertype[a]=0;UNO[a]=0;}
	if(advance==0)
	{
		if(optclass[0]+optclass[1]<2||optclass[0]+optclass[1]>14) {MessageBox(NULL,text[24].data(),text[25].data(),MB_OK|MB_ICONWARNING);return;}
		int human=optclass[0],robot=optclass[1];
		for(int a=0;a<optclass[0]+optclass[1];a++) if(rand()%(human+robot)<human) {playertype[a]=1,human--;} else {playertype[a]=2,robot--;}
		cardset=optclass[3];
		for(int a=0;a<14;a++) if(optclass[2]!=0) robottype[a]=optclass[2]; else robottype[a]=rand()%3+1;
	}
	if(advance==1)
	{
		int zero=0;
		for(int a=0;a<14;a++) if(optadvan[a]==0) zero++;
		if(zero>12) {MessageBox(NULL,text[24].data(),text[25].data(),MB_OK|MB_ICONWARNING);return;}
		for(int a=13;a>=0;a--) if(optadvan[a]!=0) if(zero+a!=13) {if(MessageBox(NULL,text[26].data(),text[25].data(),MB_YESNO|MB_ICONINFORMATION)==IDNO) return; else break;} else break;
		for(int a=0;a<13;a++) for(int b=0;b<13;b++) if(optadvan[b]==0) {optadvan[b]=optadvan[b+1];optadvan[b+1]=0;}
		for(int a=0;a<14;a++) playertype[a]=optadvan[a];
		cardset=optadvan[15];
		for(int a=0;a<14;a++) if(optadvan[14]!=0) robottype[a]=optadvan[14]; else robottype[a]=rand()%3+1;
	}
	notdealnum=0;
	for(int a=0;a<14;a++) holdnum[a]=0; playednum=0;
	for(int a=0;a<2000;a++)
	{
		notdealcard[a]=999;playedcard[a]=999;
		for(int b=0;b<14;b++) holdcard[b][a]=999;
	}
	if(special==1)
	{
		optspe[0]=1;
	}
	addcard(cardset);
	for(int a=0;a<14;a++) if(playertype[a]!=0) dealing(a,7);
	int startdraw=0;
	do
	{
		if(notdealnum==0) if(MessageBox(NULL,text[27].data(),text[25].data(),MB_YESNO|MB_ICONWARNING)==IDYES) addcard(1); else ending();
		int a=rand()%notdealnum;
		previous=notdealcard[a];
		notdealcard[a]=999;
		notdealnum--;startdraw++;
		messageset(1,startdraw,previous);
	}while(previous/100==5||previous%100>9);
	dealer=0;clockwise=1;page=0;optnum=0;drawtype=0;drawnum=0;game(special);
}
void sue()
{
	int b=0;
	for(int a=0;a<14;a++) if(playertype[a]!=0&&UNO[a]==0&&holdnum[a]<2&&a!=dealer) {dealing(a,2);b++;}
	messageset(4,b,0);
}
void game(bool special)
{
	while(true)
	{
		if(drawnum!=0) message=message+" (+"+to_string(drawnum)+")";
		if(playertype[dealer]==1) display();message="";
		if(playertype[dealer]==2) {robotdiscard();display();Sleep(500);continue;}
		switch(keydetect())
		{
			case -37:
			case -33:page=int(page-1+ceil(holdnum[dealer]/10.0))%int(ceil(holdnum[dealer]/10.0));break;
			case -39:
			case -34:page=int(page+1+ceil(holdnum[dealer]/10.0))%int(ceil(holdnum[dealer]/10.0));break;
			case -36:page=0;break;
			case -35:page=ceil(holdnum[dealer]/10.0)-1;break;
			case '1':optnum=0;break;
			case '2':optnum=1;break;
			case '3':optnum=2;break;
			case '4':optnum=3;break;
			case '5':optnum=4;break;
			case '6':optnum=5;break;
			case '7':optnum=6;break;
			case '8':optnum=7;break;
			case '9':optnum=8;break;
			case '0':optnum=9;break;
			case 'A':dealing(dealer,drawtype?drawnum:1);break;
			case 'S':if(holdnum[dealer]<=2) UNO[dealer]=1; else messageset(3,0,0);break;
			case 'D':sue();break;
			case 32:discard(dealer,page*10+optnum);break;
			case 27:pause();break;
		}
	}
}
void discard(int playerID,int cardcode)
{
	int card=holdcard[playerID][cardcode];
	if(judge(card)) ; else{messageset(5,0,0);return;}
	if(card%100<10) ;
	if(card%100==10) next();
	if(card%100==11) clockwise=!clockwise;
	if(card%100==12) {drawtype=1;drawnum+=2;}
	previous=card;
	lastcolor[playerID]=card/100;
	if(card==513) {previous=changecolor()*100+98;}
	if(card==514) {previous=changecolor()*100+98;drawtype=2;drawnum+=4;}
	playedcard[playednum]=card;
	playednum++;holdnum[playerID]--;
	holdcard[playerID][cardcode]=999;
	sort(playedcard,playedcard+2000);
	sort(holdcard[playerID],holdcard[playerID]+2000);
	next();
	bool keepgoing=0;
	if(drawtype!=0)
	{
		for(int a=0;a<holdnum[dealer];a++)
		{
			if(drawtype==1&&(holdcard[dealer][a]%100==12||holdcard[dealer][a]==514)) keepgoing=1;
			if(drawtype==2&&holdcard[dealer][a]==514) keepgoing=1;
		}
	}
	else keepgoing=1;
	if(!keepgoing) dealing(dealer,drawnum);
}
void robotdiscard()
{
	int diffi=robottype[dealer],validnum=0,validcode[holdnum[dealer]]={};
	int sametype=-1,biggest=-1,special=-1,discode;
	if(holdnum[dealer]<=2)
	{
		if(diffi==1) UNO[dealer]=rand()%2;
		if(diffi==2) UNO[dealer]=rand()%10>1?1:0;
		if(diffi==2) UNO[dealer]=rand()%100>1?1:0;
	}
	if(diffi==1) if(rand()%2) sue();
	if(diffi==2) if(rand()%10>1) sue();
	if(diffi==3) if(rand()%100>1) sue();
	for(int a=0;a<holdnum[dealer];a++)
	{
		int tempcard=holdcard[dealer][a];
		if(judge(tempcard))
		{
			validcode[validnum]=a;
			if(tempcard%100==previous%100&&tempcard/100!=5) sametype=validnum;
			if(biggest==-1) if(tempcard%100<10) biggest=validnum;
			if(tempcard%100>holdcard[dealer][validcode[biggest]]&&tempcard%100<10) biggest=validnum;
			if(tempcard%100>9) special=validnum;
			validnum++;
		}
	}
	int nextone=dealer+clockwise?1:-1;
	while(playertype[nextone]==0) nextone+=clockwise?1:-1;
	if(validnum==0) dealing(dealer,1);
	else switch(diffi)
	{
		case 3:if(UNO[dealer]) ;else if(playertype[nextone]==1&&UNO[nextone]) {discode=special;break;}
		else if(playertype[nextone]==2&&UNO[nextone]) if(special/100==5) {discode=special;break;} else {dealing(dealer,1);break;}
		case 2:if(biggest!=-1) {discode=biggest;break;} else if(sametype!=-1) {discode=sametype;break;}
		case 1:discode=rand()%validnum;break;
	}
	if(discode==-1) discode=rand()%validnum;
	if(validnum!=0) discard(dealer,validcode[discode]);
}
void ending()
{
	int valid=0,robot=0,totaldiffi=0;
	for(int a=0;a<14;a++)
	{
		if(playertype[a]!=0)
		{
			valid++;
			if(playertype[a]==2) {robot++;totaldiffi+=robottype[a];}
		}
	}
	double avediffi=double(totaldiffi?totaldiffi:0)/double(robot?robot:1);
	int rawscore[valid],tempscore[valid],rescore[valid],totalscore=0;
	for(int a=0;a<valid;a++)
	{
		rawscore[a]=0,rescore[a]=0;
		for(int b=0;b<holdnum[a];b++)
		{
			if(holdcard[a][b]%100<10) rawscore[a]-=holdcard[a][b]%100;
			else if(holdcard[a][b]/100==5) rawscore[a]-=50;
			else rawscore[a]-=20;
		}
		totalscore+=rawscore[a];
		tempscore[a]=rawscore[a];
	}
	double avescore=double(totalscore)/double(valid);
	sort(tempscore,tempscore+valid);
	int temprank[valid],points[valid],EXP[valid],totalpoints=0,totalEXP=0;
	for(int a=0;a<valid;a++)
	{
		temprank[a]=valid-a;
		if(tempscore[a-1]==tempscore[a]) temprank[a-1]=temprank[a];
		rescore[a]=round(rawscore[a]-avescore);
		points[a]=round(rescore[a]*pow(robot,avediffi));
		EXP[a]=round((rawscore[a]-tempscore[0])*totaldiffi);
		if(playertype[a]==1) {totalpoints+=points[a];totalEXP+=EXP[a];}
	}
	while(true)
	{
		system("cls");
		print(1,120,"+",'-',"+","",1);
		print(1,120,"|",' ',"|","",1);
		print(1,120,"|",' ',"|",text[68],1);
		print(1,120,"|",' ',"|","",1);
		print(1,60,"|",' ',"",text[69]+to_string(avediffi),0);print(1,60,"",' ',"|",text[70]+to_string(avescore),1);
		print(1,120,"|",' ',"|","",1);
		print(5,24,"+",'-',"+","",0);cout<<endl;
		for(int a=0;a<5;a++) print(1,24,"|",' ',"|",text[71+a],0); cout<<endl;
		print(5,24,"+",'-',"+","",0);cout<<endl;
		bool showed[valid]={};
		for(int a=valid-1;a>=0;a--)
		{
			for(int b=0;b<valid;b++) if(tempscore[a]==rawscore[b]&&!showed[b])
			{
				print(1,24,"|",' ',"|",to_string(temprank[a]),0);
				print(1,24,"|",' ',"|",text[56]+to_string((b+1)/10)+to_string((b+1)%10)+(playertype[b]==2?text[76]:""),0);
				print(1,24,"|",' ',"|",to_string(rawscore[b]),0);
				print(1,24,"|",' ',"|",to_string(rescore[b]),0);
				print(1,24,"|",' ',"|",to_string(points[b]),1);
				showed[b]=1;
			}
		}
		print(5,24,"+",'-',"+","",0);cout<<endl;
		print(1,120,"|",' ',"|","",1);
		print(1,60,"|",' ',"",text[77]+to_string(totalEXP),0);print(1,60,"",' ',"|",text[78]+to_string(totalpoints),1);
		print(1,120,"|",' ',"|","",1);
		print(1,60,"|",' ',"",text[12],0);print(1,60,"",' ',"|",text[79],1);
		print(1,120,"|",' ',"|","",1);
		print(1,120,"+",'-',"+","",1);
		switch(keydetect())
		{
			case 'A':home();break;
			case 'S':prepare(againtype,optspe[0]);break;
		}
	}
}
void pause()
{
	system("cls");
	print(1,120,"+",'-',"+","",1);
	print(4,120,"|",' ',"|","",1);
	print(1,120,"|",' ',"|","PPPP        A       U   U      SSSS     EEEEE",1);
	print(1,120,"|",' ',"|","P   P      A A      U   U     S         E    ",1);
	print(1,120,"|",' ',"|","PPPP      A   A     U   U      SSS      EEEEE",1);
	print(1,120,"|",' ',"|","P         AAAAA     U   U         S     E    ",1);
	print(1,120,"|",' ',"|","P         A   A      UUU      SSSS      EEEEE",1);
	print(4,120,"|",' ',"|","",1);
	print(1,120,"|",' ',"|",text[12],1);
	print(4,120,"|",' ',"|","",1);
	print(1,120,"|",' ',"|",text[79],1);
	print(4,120,"|",' ',"|","",1);
	print(1,120,"|",' ',"|",text[82],1);
	print(4,120,"|",' ',"|","",1);
	print(1,120,"+",'-',"+","",1);
	while(true)
	{
		switch(keydetect())
		{
			case 'A':home();break;
			case 'S':prepare(againtype,optspe[0]);break;
			case 'D':return ;
		}
	}
}
void archivemanage()
{

}