#include <cstdio>
#include <windows.h>
#include <conio.h>
#include <process.h>

#define Weigh 75
#define Heigh 25
#define BeeLine 3
#define BulletMaxAmount 10

#define Upper 72
#define Downer 80
#define Lefter 75
#define Righter 77
#define ESC 27
#define B 98

const int GameSpeed=200;
const int BeeLoop=10;
const int CarSize=4;
const int BeeSize=2;

bool Printing;

int BeeMaxAmount;
int Life;
bool Finish;
bool Win;
int JudgeMap[Heigh][Weigh]={0};
void Kill(int);
void gotoxy(int x,int y){
	COORD c; c.X=x;c.Y=y;
	SetConsoleCursorPosition (GetStdHandle(STD_OUTPUT_HANDLE),c);
}
void setcolor(int fg,int bg){
	WORD wc;
	HANDLE h=GetStdHandle(STD_OUTPUT_HANDLE);
	bg<<=4;
	wc=(fg & 0x000f )|(bg & 0x00f0);
	SetConsoleTextAttribute(h,wc);
}
struct Bullet{
	int x,y;
	int vx,vy;
	int Owner;
	bool inUse;
	Bullet(){x=0,y=0,vx=0,vy=0,Owner=0,inUse=false;}
	void Show(){
		while(Printing);
		Printing=true;
		gotoxy(x,y);
		printf("+");
		Printing=false;
	}
	void Run(){
		gotoxy(x,y);
		JudgeMap[y][x]=0;
		printf(" ");
		int x0,x1,y0,y1;
		if(vx>0){x0=x;x1=x+vx;}
		else{x0=x+vx;x1=x;}
		if(vy>0){y0=y;y1=y+vy;}
		else{y0=y+vy;y1=y;}
		if(x0<0)x0=0;
		if(x1>=Weigh)x1=Weigh-1;
		if(y0<0)y0=0;
		if(y1>=Heigh)y1=Heigh-1;
		for(int i=y0;i<=y1;i++){
			for(int j=x0;j<=x1;j++){
				if(JudgeMap[i][j]&&JudgeMap[i][j]==2&&Owner==3){
					Life--;
					inUse=false;
					break;
				}
				if(JudgeMap[i][j]&&JudgeMap[i][j]>=3&&Owner==2){
					Kill(JudgeMap[i][j]-3);
					inUse=false;
					break;
				}
			}
		}
		x=x+vx;
		y=y+vy;
		if(x<0||x>Weigh)inUse=false;
		if(y<0||y>Heigh)inUse=false;
		if(inUse)Show();
	}
}BulletSet[BulletMaxAmount];
struct Car{
	int x;
	void Show(){
		while(Printing);
		Printing=true;
		gotoxy(0,Heigh-1);
		for(int i=0;i!=Weigh;i++)printf("%c",i<=x+CarSize&&i>=x-CarSize?'#':' ');
		for(int i=0;i!=Weigh;i++)JudgeMap[Heigh-1][i]=i<=x+CarSize&&i>=x-CarSize?2:0;
		Printing=false;
	}
	void Left(){
		x-=3;
		Show();
	}
	void Right(){
		x+=3;
		Show();
	}
	void Attack(){
		for(int i=0;i!=BulletMaxAmount;i++){
			if(!BulletSet[i].inUse){
				BulletSet[i].x=x;
				BulletSet[i].y=Heigh-1;
				BulletSet[i].Owner=2;
				BulletSet[i].vx=0;
				BulletSet[i].vy=-3;
				BulletSet[i].inUse=true;
				break;
			}
		}
	}
}MyCar;
struct Bee{
	int index;
	int x,y;
	bool isDead;
	Bee(int _x=0,int _y=0){x=_x;y=_y;isDead=false;}
	void Show(){
		while(Printing);
		Printing=true;
		for(int i=0;i<BeeSize;i++){
			gotoxy(x,y+i);
			int Loop=BeeSize;
			while(Loop--){
				printf("*");
				JudgeMap[y+i][x+Loop]=3+index;
			}
		}
		Printing=false;
	}
	void Destroy(){
		while(Printing);
		Printing=true;
		for(int i=0;i<BeeSize;i++){
			gotoxy(x,y+i);
			int Loop=BeeSize;
			while(Loop--){
				printf(" ");
				JudgeMap[y+i][x+Loop]=0;
			}
		}
		x=-1;
		y=-1;
		isDead=true;
		Printing=false;
	}
	void Right(){
		x++;
		Show();
		int Loop=BeeSize;
		while(Loop--){
			while(Printing);
			Printing=true;
			gotoxy(x-1,y+Loop);
			JudgeMap[y+Loop][x-1]=0;
			printf(" ");
			Printing=false;
		}
	}
	void Left(){
		x--;
		Show();
		int Loop=BeeSize;
		while(Loop--){
			while(Printing);
			Printing=true;
			gotoxy(x+BeeSize,y+Loop);
			printf(" ");
			JudgeMap[y+Loop][x+BeeSize]=0;
			Printing=false;
		}
	}
	void Attack(){
		if(isDead)return;
		for(int i=0;i!=BulletMaxAmount;i++){
			if(!BulletSet[i].inUse){
				BulletSet[i].x=x;
				BulletSet[i].y=y;
				BulletSet[i].vx=(MyCar.x-x)/5;
				BulletSet[i].vy=(Heigh-1-y)/5;
				BulletSet[i].Owner=3;
				BulletSet[i].inUse=true;
				break;
			}
		}
	}
}BeeSet[100];
void Kill(int index){
	BeeSet[index].Destroy();
}

void GameJudge(){
	if(Life<0){
		Win=false;
		Finish=true;
	}
	for(int i=0;i!=BeeMaxAmount;i++){
		if(!BeeSet[i].isDead)return;
	}
	Win=true;
	Finish=true;
}
void UserInputControl(void* args){
	char ch;
	while(ch!=ESC&&!Finish){
		while(Printing);
		ch=getch();
		if(ch==Lefter && MyCar.x-CarSize/2>0)MyCar.Left();
		if(ch==Righter && MyCar.x+CarSize/2<Weigh)MyCar.Right();
		if(ch==B)MyCar.Attack();
		Sleep(10);
	}
	Finish=true;
}
void RunBullet(){
	for(int i=0;i!=BulletMaxAmount;i++){
		if(BulletSet[i].inUse){
			BulletSet[i].Run();
		}
	}
}
void RunBee(int Direction){
	for(int i=0;i!=BeeMaxAmount;i++){
		if(!BeeSet[i].isDead){
			if(Direction)
				BeeSet[i].Left();
			else
				BeeSet[i].Right();
		}
	}
}
void TimeLoop(bool next){
	static int LoopAmount=0;
	static int BeeDirection=0;
	if(!(++LoopAmount%BeeLoop)){
		BeeDirection=BeeDirection?0:1;
		LoopAmount=0;
		while(Printing);
		Printing=true;
		system("cls");
		Printing=false;
	}
	if(!(LoopAmount%5)){
		int CrazyBee=rand()%BeeMaxAmount;
		BeeSet[CrazyBee].Attack();
	}
	RunBee(BeeDirection);
	RunBullet();
	MyCar.Show();
	GameJudge();
	Sleep(GameSpeed);
	if(next)TimeLoop(next&&(!Finish));
}
void initialize(){
	Life=3;
	Finish=false;
	Win=false;
	_beginthread(UserInputControl, 0, NULL ); 


	{ //Print Bee~~~
		int Loop=0;
		int Now=0;
		while(Loop<BeeLine){
			int LineAmount=Weigh/(2*BeeSize)-1-2*Loop;
			for(int i=0;i!=LineAmount;i++){
				BeeSet[Now].x=2*(1+i+Loop)*BeeSize;
				BeeSet[Now].y=BeeSize*(1+2*Loop);
				BeeSet[Now].index=Now;
				BeeSet[Now++].Show();
			}
			BeeMaxAmount+=LineAmount;
			Loop++;
		}
	}
	MyCar.x=Weigh/2;
	MyCar.Show();

}
void test(){

}
int main(){
	initialize();
	TimeLoop(true);
	system("cls");
	gotoxy(0,0);
	printf("%s\n",Win?"Congratulations!":"Sorry for Lose.");
	getchar();
	return 0;
}
