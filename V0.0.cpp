#include<iostream>
#include<vector>
#include<windows.h>
#include<time.h>
#include<cmath>
#include<algorithm>
using namespace std;
vector<string>text{/*defult text*//*0*/
"U    U      N    N       OOOO ",
"U    U      NN   N      OO  OO",
"U    U      N N  N      O    O",
"U    U      N  N N      O    O",
"UU  UU      N   NN      OO  OO",
" UUUU       N    N       OOOO ",
/*6*/"[Q] 玩家档案","[A] 经典模式","[S] 趣味模式","[D] 无尽模式","[F] 游戏设置",
/*11*/"错误","该功能未开放",
/*13*/"普 通 菜 单","[^] [v] [<] [>]","人 类 数 量","电 脑 数 量","电 脑 难 度","用 牌 套 数",
/*19*/"[A] 返回主页","[S] 开始游戏","[D] 高级设置",
/*22*/"〇","随 机","简 单","普 通","困 难",
/*27*/"高 级 菜 单","玩 家","人 类","电 脑","[A] 普通设置",
/*32*/"提示","玩家总人数不能小于2或大于14","若玩家之间出现空缺，将进行顺位调整\n是否开始游戏？",
/*35*/"牌数不足，启用一套","抽取","张牌，起始牌是","","所持牌数多余2张",
/*40*/"[ESC] 暂停游戏","U N O !","余 "," 张","余牌","前牌","顺序",
/*47*/"红","黄","绿","蓝","黑","跳过","反转","+2","转色","+4","逆时针","顺时针",
/*59*/"第 "," 页 / 共 "," 页","[<]","[>]","上一页","下一页","[A] 抽牌","[S] UNO!","[D] 检举","[Space] 出牌",
/*70*/"玩家","抽了","张牌","检举出 "," 位玩家没有UNO!","出牌不符要求","选择颜色：[Q]红色  [W]黄色  [E]绿色  [R]蓝色","抽到 "," ，是否出牌？[Y/N]",
"PPPPP         AA        U    U       SSSS       EEEEEE      DDDD  ",/*79*/
"P   PP       A  A       U    U      S    S      E           D   DD",
"P   PP      A    A      U    U       SS         EEEEE       D    D",
"PPPPP       AAAAAA      U    U         SS       EEEEE       D    D",
"P           A    A      UU  UU      S    S      E           D   DD",
"P           A    A       UUUU        SSSS       EEEEEE      DDDD  ",
/*85*/"[S] 再来一局","[D] 继续游戏",
/*87*/"牌  局  结  算","难  度 ：","平  均  分 ：","排名","玩家","原始分","赋分","积分","共  得  经  验 ：","共  得  积  分 ："
};
int inver=3;string outver="rebulid";
int gamesys[1]={5};
int optclass[4]={1,1,0,1},optadvan[16]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},optspe[1]={0};
vector<int>undrawcard,drawedcard;int drawtype,drawnum,dealer,lastone,page,gametype;bool clockwise;string message="";
struct player
{
	int type,lastcolor,misscolor;
	vector<int>holdcard;
	bool UNO;
}player[14];
void print(int times,int length,string left,char fill,string right,string middle,bool wrap)
{
	for(int a=0;a<times;a++)
	{
		cout<<left;
		if(middle.length()==0) for(int b=0;b<length-left.length()-right.length();b++) cout<<fill;
		else
		{
			for(int b=0;b<floor((length-middle.length())/2.0)-left.length();b++) cout<<fill;
			cout<<middle;
			for(int b=0;b<ceil((length-middle.length())/2.0)-right.length();b++) cout<<fill;
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
		if(ReadConsoleInput(handle,inrec,128,&byteread))
			for(DWORD i=0;i<byteread;i++)
				if(inrec[i].EventType==KEY_EVENT)
					if(inrec[i].Event.KeyEvent.bKeyDown)
					{
						WORD code=inrec[i].Event.KeyEvent.wVirtualKeyCode;
						int spec=inrec[i].Event.KeyEvent.uChar.AsciiChar;
						return spec==0?-code:code;
					}
}
string swap(int type,int input)
{
	switch(type)
	{
		case 0:if(input) return text[22]; else return "  ";break;
		case 1:if(input) return "<"; else return "";break;
		case 2:if(input) return ">"; else return "";break;
		case 3:return text[23+input];break;
		case 4:if(!input) return "";else return text[28+input];break;
		case 5:if(input==-1) return text[29]; if(input>0) return text[30];break;
		case 6:if(player[input].type==0) break; else if(player[input].UNO) return text[41]; else return text[42]+to_string(player[input].holdcard.size())+text[43];break;
		case 7:return to_string(undrawcard.size());break;
		case 8:{
			string firsthalf,secondhalf;
			if(input==999) return "";
			for(int a=1;a<=5;a++) if(input/100==a) firsthalf=text[46+a];
			if(input%100<10) secondhalf=to_string(input%100);
			else for(int a=10;a<15;a++) if(input%100==a) secondhalf=text[42+a];
			return firsthalf+"_"+secondhalf;
		}break;
		case 9:return text[57+clockwise];break;
	}
	return "";
}
string set(int type,int input1,int input2,int input3)
{
	switch(type)
	{
		case 0:return text[35];break;
		case 1:return text[36]+to_string(input1)+text[37]+swap(8,input2)+text[38];break;
		case 2:return text[70]+to_string((input1+1)/10)+to_string((input1+1)%10)+text[71]+to_string(input2)+text[72];break;
		case 3:return text[39];break;
		case 4:return text[73]+to_string(input1)+text[74];break;
		case 5:return text[75];break;
		case 6:return text[76];break;
		case 7:return text[77]+swap(8,input1)+text[78];break;
	}
	return "";
}
void next()
{
	do
	{
		dealer+=clockwise?1:-1;
		dealer=(dealer+14)%14;
	}while(!player[dealer].type);
}
bool judge(int cardcode)
{
	if(drawtype==0) if(cardcode/100==lastone/100||cardcode%100==lastone%100||cardcode/100==5) return 1;
	if(drawtype==1) if(cardcode%100==12||cardcode==514) return 1;
	if(drawtype==2) if(cardcode==514) return 1;
	return 0;
}
void home();
void archive();
void general();
void advance();
void prepare(bool advance);
void special();
void game();
void pause();
void ending();
void display(int optnum)
{
	for(int a=0;a<14;a++) if(player[a].holdcard.size()==0&&player[a].type!=0) ending();
	for(int a=0;a<14;a++) if((player[a].holdcard.size()>2&&player[a].type!=0)||player[a].type==0) player[a].UNO=0;
	system("cls");
	print(1,120,"+-"+text[40],'-',"+","",1);
	print(1,120,"|",' ',"|","",1);
	cout<<"|  ";print(6,19,"  /",'-',"\\  ","",0);cout<<"  |"<<endl;
	cout<<"|  ";for(int a=0;a<6;a++) print(1,19,"  |"+swap(0,!(dealer-a)),' ',text[28]+"0"+to_string(a+1)+"|  ","",0); cout<<"  |"<<endl;
	cout<<"|  ";for(int a=0;a<6;a++) print(1,19,"  |",' ',"|  ",swap(5,player[a].type),0); cout<<"  |"<<endl;
	cout<<"|  ";for(int a=0;a<6;a++) print(1,19,"  |",' ',"|  ",swap(6,a),0); cout<<"  |"<<endl;
	cout<<"|  ";print(6,19,"  \\",'-',"/  ","",0);cout<<"  |"<<endl;
	print(1,120,"|",' ',"|","",1);
	print(1,24,"|",' '," ","",0);print(1,72,"/",'-',"\\","",0);print(1,24," ",' ',"|","",1);
	print(1,25,"|    /",'-',"\\    |","",0);print(1,70," ",' '," "," ",0);print(1,25,"|    /",'-',"\\    |","",1);
	print(1,25,"|    |"+swap(0,!(dealer-13)),' ',text[28]+to_string(14)+"|    |","",0);for(int a=0;a<3;a++){print(1,24-2*(a%2)," ",' '," ",text[44+a]+": "+swap(7+a,lastone),0);}print(1,25,"|    |"+swap(0,!(dealer-6)),' ',text[28]+"0"+to_string(7)+"|    |","",1);//24,22,24
	print(1,25,"|    |",' ',"|    |",swap(5,player[13].type),0);print(1,70," ",' '," "," ",0);print(1,25,"|    |",' ',"|    |",swap(5,player[6].type),1);
	print(1,25,"|    |",' ',"|    |",swap(6,13),0);print(1,70," ",' '," ",message+(drawnum?(" (+"+to_string(drawnum)+")"):""),0);print(1,25,"|    |",' ',"|    |",swap(6,6),1);
	print(1,25,"|    \\",'-',"/    |","",0);print(1,70," ",' '," "," ",0);print(1,25,"|    \\",'-',"/    |","",1);
	print(1,24,"|",' '," ","",0);print(1,72,"\\",'-',"/","",0);print(1,24," ",' ',"|","",1);
	print(1,120,"|",' ',"|","",1);
	cout<<"|  ";print(6,19,"  /",'-',"\\  ","",0);cout<<"  |"<<endl;
	cout<<"|  ";for(int a=12;a>6;a--) print(1,19,"  |"+swap(0,!(dealer-a)),' ',text[28]+to_string((a+1)/10)+to_string((a+1)%10)+"|  ","",0); cout<<"  |"<<endl;
	cout<<"|  ";for(int a=12;a>6;a--) print(1,19,"  |",' ',"|  ",swap(5,player[a].type),0); cout<<"  |"<<endl;
	cout<<"|  ";for(int a=12;a>6;a--) print(1,19,"  |",' ',"|  ",swap(6,a),0); cout<<"  |"<<endl;
	cout<<"|  ";print(6,19,"  \\",'-',"/  ","",0);cout<<"  |"<<endl;
	print(1,120,"|",' ',"|","",1);
	print(1,120,"+",'-',"+","",1);
	print(1,120,"|",' ',"|",player[dealer].type==-1?(text[59]+to_string(page+1)+text[60]+to_string(int(ceil(player[dealer].holdcard.size()/10.0)))+text[61]):"",1);
	print(1,10,"|",' '," ",text[62],0);for(int a=1;a<11;a++) print(1,10," ",' '," ","["+((a-1==optnum)?text[22]:to_string(a%10))+"]",0); print(1,10," ",' ',"|",text[63],1);
	print(1,10,"|",' '," ",text[64],0);for(int a=0;a<10;a++) print(1,10," ",' '," ",(page*10+a<player[dealer].holdcard.size())?(swap(8,player[dealer].type==-1?player[dealer].holdcard[page*10+a]:999)):"",0); print(1,10," ",' ',"|",text[65],1);
	print(1,120,"|",' ',"|","",1);
	print(1,120,"|",' ',"|",text[66]+"          "+text[67]+"          "+text[68],1);
	print(1,120,"|",' ',"|",text[69],1);
	print(1,120,"+",'-',"+","",0);
	message="";
}
int main()
{
	srand(time(0));
	system("mode con cols=121 lines=31");
	SetWindowLongPtrA(GetConsoleWindow(),GWL_STYLE,GetWindowLongPtrA(GetConsoleWindow(),GWL_STYLE)&~WS_SIZEBOX&~WS_MAXIMIZEBOX);
	home();
	return 0;
}
void home()
{
	while(true)
	{
		system("cls");
		print(1,120,"+-"+text[6],'-',"+","",1);
		print(5,120,"|",' ',"|","",1);
		for(int a=0;a<6;a++) print(1,120,"|",' ',"|",text[a],1);
		print(5,120,"|",' ',"|","",1);
		for(int a=0;a<4;a++)
		{
			print(1,120,"|",' ',"|",text[a+7],1);
			print(1,120,"|",' ',"|","",1);
		}
		print(4,120,"|",' ',"|","",1);
		print(1,120,"+",'-',outver+"-+","",0);
		switch(keydetect())
		{
			case 'A':optspe[0]=0;general();break;
			case 'S':MessageBox(NULL,text[12].data(),text[11].data(),MB_OK|MB_ICONSTOP);break;
			case 'D':MessageBox(NULL,text[12].data(),text[11].data(),MB_OK|MB_ICONSTOP);break;
			case 'F':MessageBox(NULL,text[12].data(),text[11].data(),MB_OK|MB_ICONSTOP);break;
			case 'Q':MessageBox(NULL,text[12].data(),text[11].data(),MB_OK|MB_ICONSTOP);break;
		}
	}
}
void general()
{
	int optnum=0;
	while(true)
	{
		system("cls");
		optclass[0]=(optclass[0]+15)%15;optclass[1]=(optclass[1]+15)%15;optclass[2]=(optclass[2]+4)%4;optclass[3]=!optclass[3]?1:optclass[3];
		print(1,120,"+",'-',"+","",1);
		print(5,120,"|",' ',"|","",1);
		print(1,120,"|",' ',"|",text[13],1);
		print(1,120,"|",' ',"|",text[14],1);
		print(4,120,"|",' ',"|","",1);
		for(int a=0;a<4;a++)
		{
			print(1,61,"|",' ',swap(0,optnum==a)+text[15+a]+": ","",0);
			print(1,15,swap(1,optnum==a),' ',swap(2,optnum==a),a==2?swap(3,optclass[2]):to_string(optclass[a]),0);
			print(1,44,"",' ',"|","",1);
			print(1,120,"|",' ',"|","",1);
		}
		print(3,120,"|",' ',"|","",1);
		print(1,120,"|",' ',"|",text[19]+"  "+text[20]+"  "+text[21],1);
		print(5,120,"|",' ',"|","",1);
		print(1,120,"+",'-',"+","",0);
		switch(keydetect())
		{
			case -38:optnum=(optnum+4-1)%4;break;
			case -40:optnum=(optnum+4+1)%4;break;
			case -37:optclass[optnum]--;break;
			case -39:optclass[optnum]++;break;
			case 'A':return;break;
			case 'S':prepare(gametype=0);break;
			case 'D':advance();break;
		}
	}
}
void advance()
{
	int optnum=0;
	while(true)
	{
		system("cls");
		for(int a=0;a<14;a++) optadvan[a]=(optadvan[a]+3)%3;
		optadvan[14]=(optadvan[14]+4)%4;optadvan[15]=!optadvan[15]?1:optadvan[15];
		print(1,120,"+",'-',"+","",1);
		print(1,120,"|",' ',"|","",1);
		print(1,120,"|",' ',"|",text[27],1);
		print(1,120,"|",' ',"|",text[14],1);
		print(2,120,"|",' ',"|","",1);
		for(int a=1;a<8;a++)
		{
			print(1,21,"|",' ',swap(0,a-1==optnum)+text[28]+"0"+to_string(a)+": ","",0);
			print(1,15,swap(1,a-1==optnum),' ',swap(2,a-1==optnum),swap(4,optadvan[a-1]),0);
			print(1,4," ",' ',"|","",0);
			print(1,21,"|",' ',swap(0,a-1==optnum-7)+text[28]+to_string((a+7)/10)+to_string((a+7)%10)+": ","",0);
			print(1,15,swap(1,a-1==optnum-7),' ',swap(2,a-1==optnum-7),swap(4,optadvan[a+6]),0);
			print(1,4," ",' ',"|","",0);
			if(a==3) {print(1,21,"|",' ',swap(0,optnum==14)+text[17]+": ","",0);print(1,15,swap(1,optnum==14),' ',swap(2,optnum==14),swap(3,optadvan[14]),0);print(1,4," ",' ',"|","",1);}
			else if(a==5) {print(1,21,"|",' ',swap(0,optnum==15)+text[18]+": ","",0);print(1,15,swap(1,optnum==15),' ',swap(2,optnum==15),to_string(optadvan[15]),0);print(1,4," ",' ',"|","",1);}
			else print(1,40,"|",' ',"|","",1);
			for(int b=0;b<2;b++) if(a!=7){print(3,40,"|",' ',"|","",0);cout<<endl;}
		}
		print(2,120,"|",' ',"|","",1);
		print(1,120,"|",' ',"|",text[31]+"  "+text[20],1);
		print(1,120,"|",' ',"|","",1);
		print(1,120,"+",'-',"+","",0);
		switch(keydetect())
		{
			case -38:optnum=(optnum+16-1)%16;break;
			case -40:optnum=(optnum+16+1)%16;break;
			case -37:optadvan[optnum]--;break;
			case -39:optadvan[optnum]++;break;
			case 'A':return;break;
			case 'S':prepare(gametype=1);break;
		}
	}
}
void special()
{

}
void addcard(int time)
{
	for(int a=0;a<time;a++)
		for(int b=1;b<=4;b++)
		{
			undrawcard.push_back(b*100);
			for(int c=0;c<2;c++)
			{
				for(int d=1;d<=9;d++) undrawcard.push_back(b*100+d);
				for(int d=0;d<3;d++) undrawcard.push_back(b*100+10+d);
				undrawcard.push_back(513+c);
			}
		}
	message=set(0,0,0,0);
}
bool disdrawcard(int cardcode)
{
	bool yes=0;
	if(player[dealer].type==-1)
	{
		message=set(7,cardcode,0,0);
		display(-1);
		while(true)
		{
			switch(keydetect())
			{
				case 'Y':message=set(8,dealer,0,0);return 1;break;
				case 'N':return 0;break;
			}
		}
	}else
	{
		if(player[dealer].type==1) if(rand()%2) yes=1; else yes=0;
		if(player[dealer].type==2) yes=1;
		int nextone=dealer+clockwise?1:-1;
		while(player[dealer].type==0) nextone+=clockwise?1:-1;
		if(player[dealer].type==3) if(player[nextone].type==-1||player[dealer].UNO) yes=1;
		else if(player[nextone].type!=-1&&player[nextone].UNO&&cardcode%100<10&&cardcode/100==player[nextone].lastcolor) yes=1;
		else if(player[nextone].type!=-1&&!player[nextone].UNO) yes=1;
		else yes=0;
	}
	if(yes) {player[dealer].lastcolor=cardcode/100;message=set(8,dealer,0,0);return 1;} else return 0;
}
int changecolor()
{
	player[dealer].misscolor=lastone/100;
	if(player[dealer].type==-1)
	{
		message=set(6,0,0,0);
		display(-1);
		message="";
		while(true)
		{
			switch(keydetect())
			{
				case 'Q':return player[dealer].lastcolor=1;
				case 'W':return player[dealer].lastcolor=2;
				case 'E':return player[dealer].lastcolor=3;
				case 'R':return player[dealer].lastcolor=4;
			}
		}
	}
	else
	{
		int raw[4]={},sorted[4]={},biggest;
		for(int a=0;a<player[dealer].holdcard.size();a++)
		{
			int temp=player[dealer].holdcard[a];
			if(temp/100<5) if(temp%100<10) raw[temp/100-1]+=temp%100;
			else raw[temp/100-1]+=20;
		}
		for(int a=0;a<4;a++) sorted[a]=raw[a];
		sort(sorted,sorted+4);
		for(int a=0;a<4;a++) if(raw[a]==sorted[3]) {biggest=a+1;break;}
		if(player[dealer].type==1) return player[dealer].lastcolor=rand()%4+1;
		if(player[dealer].type==2) return player[dealer].lastcolor=biggest;
		int nextone=dealer+clockwise?1:-1;
		while(player[nextone].type==0) nextone+=clockwise?1:-1;
		if(player[dealer].type==3) if(player[nextone].type==-1&&player[nextone].UNO) return player[dealer].lastcolor=player[nextone].misscolor;
		else if(player[nextone].type!=-1&&player[nextone].UNO) return player[dealer].lastcolor=player[nextone].lastcolor;
		return player[dealer].lastcolor=biggest;
	}
	return 0;
}
void dealing(int ID,int dealnum)
{
	message=set(2,ID,dealnum,0);
	if(dealnum>undrawcard.size())
	{
		for(int a=0;a<drawedcard.size();a++) undrawcard.push_back(drawedcard[a]);
		drawedcard.clear();
		while(dealer>undrawcard.size()) addcard(1);
	}
	if(dealnum==1)
	{
		int temp=rand()%undrawcard.size();
		int tempcard=undrawcard[temp];
		undrawcard.erase(undrawcard.begin()+temp);
		bool valid=0;bool useit=0;
		if(judge(tempcard)) {valid=1;useit=disdrawcard(tempcard);} else player[ID].misscolor=lastone/100;
		if(!valid||!useit) player[ID].holdcard.push_back(tempcard);
		if(useit)
		{
			if(tempcard%100<10) ;
			if(tempcard%100==10) next();
			if(tempcard%100==11) clockwise=!clockwise;
			if(tempcard%100==12) {drawtype=1;drawnum+=2;}
			lastone=tempcard;
			if(tempcard==513) {lastone=changecolor()*100+98;}
			if(tempcard==514) {lastone=changecolor()*100+98;drawtype=2;drawnum+=4;}
		}
	}
	else for(int a=0;a<dealnum;a++)
	{
		int b=rand()%undrawcard.size();
		player[ID].holdcard.push_back(undrawcard[b]);
		undrawcard.erase(undrawcard.begin()+b);
	}
	sort(player[ID].holdcard.begin(),player[ID].holdcard.end());
	drawtype=0;drawnum=0;
	next();
}
void prepare(bool advance)
{
	int cardset=1;
	for(int a=0;a<14;a++) {player[a].type=0;player[a].UNO=0;}
	if(advance==0)
	{
		if(optclass[0]+optclass[1]<2||optclass[0]+optclass[1]>14) {MessageBox(NULL,text[33].data(),text[32].data(),MB_OK|MB_ICONWARNING);return;}
		int human=optclass[0],robot=optclass[1];
		for(int a=0;a<optclass[0]+optclass[1];a++) if(rand()%(human+robot)<human) {player[a].type=-1,human--;} else {player[a].type=5,robot--;}
		cardset=optclass[3];
		for(int a=0;a<14;a++) if(player[a].type==5) if(optclass[2]!=0) player[a].type=optclass[2]; else player[a].type=rand()%3+1;
	}
	if(advance==1)
	{
		int zero=0;
		for(int a=0;a<14;a++) if(optadvan[a]==0) zero++;
		if(zero>12) {MessageBox(NULL,text[33].data(),text[32].data(),MB_OK|MB_ICONWARNING);return;}
		for(int a=13;a>=0;a--) if(optadvan[a]!=0) if(zero+a!=13) {if(MessageBox(NULL,text[34].data(),text[32].data(),MB_YESNO|MB_ICONINFORMATION)==IDNO) return; else break;} else break;
		for(int a=0;a<13;a++) for(int b=0;b<13;b++) if(optadvan[b]==0) {optadvan[b]=optadvan[b+1];optadvan[b+1]=0;}
		for(int a=0;a<14;a++) if(optadvan[a]==1) player[a].type=-1; else player[a].type=5;
		cardset=optadvan[15];
		for(int a=0;a<14;a++) if(player[a].type==5) if(optadvan[14]!=0) player[a].type=optadvan[14]; else player[a].type=rand()%3+1;
	}
	undrawcard.clear();drawedcard.clear();
	for(int a=0;a<14;a++) player[a].holdcard.clear();
	if(optspe[0]==1)
	{
		special();
		/**/
	}
	addcard(cardset);
	for(int a=0;a<14;a++) if(player[a].type!=0) dealing(a,7);
	int startdraw=0;
	do
	{
		if(undrawcard.size()==0) addcard(1);
		int a=rand()%undrawcard.size();
		lastone=undrawcard[a];
		undrawcard.erase(undrawcard.begin()+a);
		startdraw++;
		message=set(1,startdraw,lastone,0);
	}while(lastone/100==5||lastone%100>=10);
	dealer=0;clockwise=1;page=0;drawtype=0;drawnum=0;
	game();
}
void sue()
{
	int num=0;
	for(int a=0;a<14;a++) if(player[a].type!=0&&player[a].UNO==0&&player[a].holdcard.size()<2&&a!=dealer) {dealing(a,2);num++;}
	if(num>0) message=set(4,num,0,0);
}
void discard(int code)
{
	int card=player[dealer].holdcard[code];
	if(judge(card)) ; else{message=set(5,0,0,0);return;}
	player[dealer].holdcard.erase(player[dealer].holdcard.begin()+code);
	if(card%100<10) ;
	if(card%100==10) next();
	if(card%100==11) clockwise=!clockwise;
	if(card%100==12) {drawtype=1;drawnum+=2;}
	lastone=card;
	player[dealer].lastcolor=card/100;
	if(card==513) {lastone=changecolor()*100+98;}
	if(card==514) {lastone=changecolor()*100+98;drawtype=2;drawnum+=4;}
	drawedcard.push_back(card);
	next();
	bool keepgoing=0;
	if(drawtype!=0)
	{
		for(int a=0;a<player[dealer].holdcard.size();a++)
		{
			if(drawtype==1&&(player[dealer].holdcard[a]%100==12||player[dealer].holdcard[a]==514)) keepgoing=1;
			if(drawtype==2&&player[dealer].holdcard[a]==514) keepgoing=1;
		}
	}
	else keepgoing=1;
	if(!keepgoing) dealing(dealer,drawnum);
}
void robotdiscard()
{
	int diffi=player[dealer].type,validnum=0,validcode[player[dealer].holdcard.size()]={};
	int sametype=-1,biggest=-1,special=-1,discode;
	if(player[dealer].holdcard.size()<=2)
	{
		if(diffi==1) player[dealer].UNO=rand()%2;
		if(diffi==2) player[dealer].UNO=rand()%10>1?1:0;
		if(diffi==2) player[dealer].UNO=rand()%100>1?1:0;
	}
	if(diffi==1) if(rand()%2) sue();
	if(diffi==2) if(rand()%10>1) sue();
	if(diffi==3) if(rand()%100>1) sue();
	for(int a=0;a<player[dealer].holdcard.size();a++)
	{
		int tempcard=player[dealer].holdcard[a];
		if(judge(tempcard))
		{
			validcode[validnum]=a;
			if(tempcard%100==lastone%100&&tempcard/100!=5) sametype=validnum;
			if(biggest==-1) {if(tempcard%100<10) biggest=validnum;}
			else if(tempcard%100>player[dealer].holdcard[validcode[biggest]]&&tempcard%100<10) biggest=validnum;
			if(tempcard%100>9) special=validnum;
			validnum++;
		}
	}
	int nextone=dealer+clockwise?1:-1;
	while(player[nextone].type==0) nextone+=clockwise?1:-1;
	if(validnum==0) dealing(dealer,1);
	else switch(diffi)
	{
		case 3:if(player[dealer].UNO) ;else if(player[nextone].type==-1&&player[nextone].UNO) {discode=special;break;}
		else if(player[nextone].type>0&&player[nextone].UNO) if(special/100==5) {discode=special;break;} else {dealing(dealer,1);break;}
		case 2:if(biggest!=-1) {discode=biggest;break;} else if(sametype!=-1) {discode=sametype;break;}
		case 1:discode=rand()%validnum;break;
	}
	if(discode==-1) discode=rand()%validnum;
	if(validnum!=0) discard(validcode[discode]);
}
void game()
{
	int optnum=0;
	while(true)
	{
		if(player[dealer].type==-1) display(optnum);
		if(player[dealer].type>0) {robotdiscard();display(optnum);Sleep(500);continue;}
		switch(keydetect())
		{
			case -37:
			case -33:page=int(page-1+ceil(player[dealer].holdcard.size()/10.0))%int(ceil(player[dealer].holdcard.size()/10.0));break;
			case -39:
			case -34:page=int(page+1+ceil(player[dealer].holdcard.size()/10.0))%int(ceil(player[dealer].holdcard.size()/10.0));break;
			case -36:page=0;break;
			case -35:page=ceil(player[dealer].holdcard.size()/10.0)-1;break;
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
			case 'S':if(player[dealer].holdcard.size()<=2) player[dealer].UNO=1; else message=set(3,0,0,0);break;
			case 'D':sue();break;
			case 32:discard(page*10+optnum);break;
			case 27:pause();break;
		}
	}
}
void pause()
{
	while(true)
	{
		system("cls");
		print(1,120,"+",'-',"+","",1);
		print(7,120,"|",' ',"|","",1);
		for(int a=0;a<6;a++) print(1,120,"|",' ',"|",text[79+a],1);
		print(7,120,"|",' ',"|","",1);
		print(1,120,"|",' ',"|",text[19]+"          "+text[85]+"          "+text[86],1);
		print(7,120,"|",' ',"|","",1);
		print(1,120,"+",'-',"+","",0);
		switch(keydetect())
		{
			case 'A':home();break;
			case 'S':prepare(gametype);break;
			case 'D':return;break;
		}
	}
}
void ending()
{
	int valid=0,robot=0,totaldiffi=0;
	for(int a=0;a<14;a++)
		if(player[a].type!=0)
		{
			valid++;
			if(player[a].type>0) {robot++;totaldiffi+=player[a].type;}
		}
	double avediffi=double(totaldiffi?totaldiffi:0)/double(robot?robot:1);
	int rawscore[valid],tempscore[valid],rescore[valid],totalscore=0;
	for(int a=0;a<valid;a++)
	{
		rawscore[a]=0,rescore[a]=0;
		for(int b=0;b<player[a].holdcard.size();b++)
		{
			if(player[a].holdcard[b]%100<10) rawscore[a]-=player[a].holdcard[b]%100;
			else if(player[a].holdcard[b]/100==5) rawscore[a]-=50;
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
		if(player[a].type==-1) {totalpoints+=points[a];totalEXP+=EXP[a];}
	}
	while(true)
	{
		system("cls");
		print(1,120,"+",'-',"+","",1);
		print(1,120,"|",' ',"|","",1);
		print(1,120,"|",' ',"|",text[87],1);
		print(1,120,"|",' ',"|","",1);
		print(1,60,"|",' ',"",text[88]+to_string(avediffi),0);print(1,60,"",' ',"|",text[89]+to_string(avescore),1);
		print(1,120,"|",' ',"|","",1);
		print(5,24,"+",'-',"+","",0);cout<<endl;
		for(int a=0;a<5;a++) print(1,24,"|",' ',"|",text[90+a],0); cout<<endl;
		print(5,24,"+",'-',"+","",0);cout<<endl;
		bool showed[valid]={};
		for(int a=valid-1;a>=0;a--)
		{
			for(int b=0;b<valid;b++) if(tempscore[a]==rawscore[b]&&!showed[b])
			{
				print(1,24,"|",' ',"|",to_string(temprank[a]),0);
				print(1,24,"|",' ',"|",text[91]+to_string((b+1)/10)+to_string((b+1)%10)+"("+swap(5,player[b].type)+")",0);
				print(1,24,"|",' ',"|",to_string(rawscore[b]),0);
				print(1,24,"|",' ',"|",to_string(rescore[b]),0);
				print(1,24,"|",' ',"|",to_string(points[b]),1);
				showed[b]=1;
			}
		}
		print(5,24,"+",'-',"+","",0);cout<<endl;
		print(1,120,"|",' ',"|","",1);
		print(1,60,"|",' ',"",text[95]+to_string(totalEXP),0);print(1,60,"",' ',"|",text[96]+to_string(totalpoints),1);
		print(1,120,"|",' ',"|","",1);
		print(1,60,"|",' ',"",text[19],0);print(1,60,"",' ',"|",text[86],1);
		print(1,120,"|",' ',"|","",1);
		print(1,120,"+",'-',"+","",0);
		switch(keydetect())
		{
			case 'A':home();break;
			case 'S':prepare(gametype);break;
		}
	}
}
void archive()
{

}