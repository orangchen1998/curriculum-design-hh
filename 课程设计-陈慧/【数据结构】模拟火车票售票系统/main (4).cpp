#include <iostream> 
#include <fstream> 
#include<stdio.h>
#include <string.h> 
#include<time.h> 
using namespace std;  
void welcome();
//欢迎子函数
void choose();                           //选择子函数
void banciluru();                        //班次录入子函数
void liulanbancixinxi();                 //浏览班次信息子函数
void shoupiao();                         //售票子函数
void tuipiao();                          //退票子函数
void quit();                             //退出子函数
void chaxun();                           //查询子函数
int loading();                           //文件载入子函数
void adminLogin();
void userLogin();
void userRegister();
void adminMenu();
void userMenu();
void userChoose();
void adminChoose();
int fileExists(string filename);
const int MAX=20;                        //使数组最大值可调
const int N=50;
int ifAdmin=0;								//判断是进入管理员界面还是进入用户界面 
struct time {                            //时间结构体,为了与系统时间做比较.
	int hour;
	int minutes;
};
struct ticket {                          //车票结构体,包含车次,时间结构体,起点,终点,行车时间,额定载客量,已订票人数
	string Number;
	struct time Setout;
	char Qidian[MAX];
	char Zhongdian[MAX];
	float Lasttime;
	int Fixnumber;
	int Fixednumber;
} car[N];  //主函数
int main() {
	
	welcome();
	return 0;
}
//欢迎子函数
void welcome() {
	system("title 车票管理系统");//设置cmd窗口标题
    system("color 0A");
    
	cout<<"________________________________________________________"<<endl;
	cout<<"| 欢 * 迎 * 使 * 用 * 火 * 车 * 票 * 管 * 理 * 系 * 统 |"<<endl;
	cout<<"|                                                      |"<<endl;
	cout<<"|             本管理系统有以下功能:                    |"<<endl;
	cout<<"|                                                      |"<<endl;
	cout<<"|                 1 管理员登入                         |"<<endl;
	cout<<"|                 2 用户登录                           |"<<endl;
	cout<<"|                 3 用户注册                           |"<<endl;
	cout<<"|______________________________________________________|"<<endl;
	cout<<"请选择身份："<<endl;
	string user;
	cin>>user;
	if(user == "1"){
		adminLogin();
	}
	if(user == "2"){
		userLogin();
	}
	if(user == "3"){
		userRegister();
	}
	welcome();
	
	
}

void adminLogin(){
	string name;
	string pwd;
	cout<<"admin's username:";
	cin>>name;
	cout<<"admin's password:";
	cin>>pwd;//
	if(name == "admin"){
		if(pwd == "password"){
			ifAdmin=1;
			adminMenu();
		}
		else{
			cout << "用户名或密码错误，请重试" << endl;
			adminLogin();
		}
	}else{
		cout << "用户名或密码错误，请重试" << endl;
		adminLogin();
	}
}

void userLogin(){
	string username;
	string password;
	string s;
	cout << "请输入用户名:";
	cin >> username;
	cout << "请输入密码:";
	cin >>  password;
	ifstream userfile;
	
	userfile.open(username.c_str(),ios::in);
	while(getline(userfile,s)){
		if(password == s){
			userMenu();
		}else{
			cout << "用户名或密码错误";
			userLogin();
		}
	}
	welcome();

}

void userRegister(){
	string username;
	string password;

	cout << "your username:";
	cin >> username;
	cout << "your password:";
	cin >> password;
	  
	 /*判断用户是否已存在*/
	if(fileExists(username)){
		cout << "user has already exists"<<endl;
		cout << fileExists(username);
		
		welcome();
	}
	ofstream userfile;
	userfile.open(username.c_str(),ios::out);
	if(userfile<<password){
		ifAdmin=0; 
		userfile.close();
		cout << "register success";
		userLogin();
		
	}else{
		ifAdmin=0;
		userfile.close();
		cout << "register fail";
		welcome();
}
	userfile.close();
	
}
//选择子函数
void adminChoose() {
	string i;
	cin>>i;
	if(i=="1")
		banciluru();
	if(i=="2")
		liulanbancixinxi();
	if(i=="3")
		chaxun();
	if(i=="4")
		shoupiao();
	if(i=="5")
		tuipiao();
	if(i=="6")
		quit();
	else  {
		cout<<"输入有误!请在数字1-6之间重新您的输入选择!"<<endl;
		adminChoose();
	}
}

void userChoose(){
	string i;
	cin>>i;
	if(i=="1")
		liulanbancixinxi();
	if(i=="2")
		chaxun();
	if(i=="3")
		shoupiao();
	if(i=="4")
		tuipiao();
	if(i=="5")
		quit();
	else  {
		cout<<"输入有误!请在数字1-5之间重新您的输入选择!"<<endl;
		userChoose();
	}
}
//班次录入子函数
void banciluru() {
	string t="1";        //浏览班次子函数
	ofstream outfile;
	outfile.open("班次信息.txt",ios::app);
	for(int i=0; t=="1"; i++)  {
		cout<<"请输入需要添加的火车班次:"<<endl;
		cin>>car[i].Number;
		cout<<"请输入此班次发车时间(格式:先输入小时,回车,再输入分钟):"<<endl;
		cin>>car[i].Setout.hour;
		cin>>car[i].Setout.minutes;
		cout<<"请输入出发站:"<<endl;
		cin>>car[i].Qidian;
		cout<<"请输入终点站:"<<endl;
		cin>>car[i].Zhongdian;
		cout<<"请输入行车时间:"<<endl;
		cin>>car[i].Lasttime;
		cout<<"请输入额定载客量:"<<endl;
		cin>>car[i].Fixnumber;
		cout<<"请输入已订票人数:"<<endl;
		cin>>car[i].Fixednumber;
		outfile<<car[i].Number<<" "<<car[i].Setout.hour<<" "<<car[i].Setout.minutes<<" "<<car[i].Qidian<<" "<<car[i].Zhongdian<<" "<<car[i].Lasttime<<" "<<car[i].Fixnumber<<" "<<car[i].Fixednumber<<endl;
		cout<<"是否继续录入?(是请输1,否请输任意键返回主菜单)"<<endl;
		cin>>t;
		if(t!="1")   {
			if(ifAdmin==1){
				adminMenu();
			}else{
				userMenu();
			}
		}
	}
	outfile.close();
	cout<<"输入回车键返回"<<endl;
	getchar();
	getchar();
	if(ifAdmin==1){
		adminMenu();
	}else{
		userMenu();
	}
}
void liulanbancixinxi() {
	int c;
	c=loading();
	cout<<"班次"<<" "<<"发车时间"<<" "<<"起点站"<<" "<<"终点站"<<" "<<"行车时间"<<" "<<"额定载客量"<<" "<<"已订票人数"<<endl;
	for(int i=0; i<c; i++)  {
		cout<<car[i].Number<<"     "<<car[i].Setout.hour<<":"<<car[i].Setout.minutes<<"    "<<car[i].Qidian<<"    "<<car[i].Zhongdian<<"      "<<car[i].Lasttime<<"      "<<car[i].Fixnumber<<"      "<<car[i].Fixednumber<<endl;
		time_t tval;
		struct tm *now;
		tval = time(NULL);
		now = localtime(&tval);
		if((now->tm_hour==car[i].Setout.hour&&now->tm_min>car[i].Setout.minutes)||(now->tm_hour>car[i].Setout.hour))    cout<<"此车已发出"<<endl;
	}
	cout<<"输入回车键返回"<<endl;
	getchar();
	getchar();
	if(ifAdmin==1){
		adminMenu();
	}else{
		userMenu();
	}
}   //车次查询子函数

void Numbersearch();
void Finalsearch();
void chaxun() {
	 
	string t;
    cout<<"________________________________________________________"<<endl;
	cout<<"| 欢 * 迎 * 使 * 用 * 火 * 车 * 票 * 管 * 理 * 系 * 统 |"<<endl;
	cout<<"|                                                      |"<<endl;
	cout<<"|                                                      |"<<endl;
	cout<<"|                                                      |"<<endl;
	cout<<"|                 1.按班次查询                         |"<<endl;
	cout<<"|                 2.按终点站查询                       |"<<endl;
	cout<<"|                                                      |"<<endl;
	cout<<"|请输入您想选择的查询方法前的数字:                     |"<<endl;
	cout<<"|                                                      |"<<endl;
	cout<<"|______________________________________________________|"<<endl;
	cin>>t;
	if(t=="1")   Numbersearch();
	if(t=="2")   Finalsearch();
	if(t!="1"||t!="2")  {
		cout<<"输入有误,请重新输入"<<endl;
		chaxun();
	}
}

void Numbersearch() {//班次查询子函数
	loading();     
	string SearchNumber;
	cout<<"请输入您需要查找的班次号:"<<endl;
	cin>>SearchNumber;
	cout<<"班次"<<" "<<"发车时间"<<" "<<"起点站"<<" "<<"终点站"<<" "<<"行车时间"<<" "<<"额定载客量"<<" "<<"已订票人数"<<endl;
	for(int len=1;len<(sizeof(car)/sizeof(car[0]));len++){
		if(car[len-1].Number == SearchNumber){
			cout<<car[len-1].Number<<"     "<<car[len-1].Setout.hour<<":"<<car[len-1].Setout.minutes<<"     "<<car[len-1].Qidian<<"   "<<car[len-1].Zhongdian<<"     "<<car[len-1].Lasttime<<"     "<<car[len-1].Fixnumber<<"        "<<car[len-1].Fixednumber<<endl;		
		}
	}
	cout<<"输入回车键返回"<<endl;
	getchar();
	getchar();
	if(ifAdmin==1){
		adminMenu();
	}else{
		userMenu();
	}
}
void Finalsearch() {//终点站查询子函数
	int R;
	R=loading();
	int i;
	char Address[MAX];
	cout<<"请输入终点站名称:"<<endl;
	cin>>Address;
	cout<<"班次"<<" "<<"发车时间"<<" "<<"起点站"<<" "<<"终点站"<<" "<<"行车时间"<<" "<<"额定载客量"<<" "<<"已订票人数"<<endl;
	for(i=0; i<R; i++)  {
		if((strcmp(Address,car[i].Zhongdian))==0)    
		cout<<car[i].Number<<"     "<<car[i].Setout.hour<<":"<<car[i].Setout.minutes<<"    "<<car[i].Qidian<<"    "<<car[i].Zhongdian<<"     "<<car[i].Lasttime<<"      "<<car[i].Fixnumber<<"          "<<car[i].Fixednumber<<endl;
	}
	cout<<"输入回车键返回"<<endl;
	getchar();
	getchar();
	if(ifAdmin==1){
		adminMenu();
	}else{
		userMenu();
	}
}
    ；void shoupiao() {
	int R;
	R=loading();
	int i=0;
	string SearchNumber;
	cout<<"请输入要订购的车票的班次（发车前一个小时无法售票）:"<<endl;
	cin>>SearchNumber;
	time_t tval;
	struct tm *now;
	tval = time(NULL);
	now = localtime(&tval);
	
	for(int len=1;len<(sizeof(car)/sizeof(car[0]));len++){
		if(car[len-1].Number == SearchNumber){
			if(car[len-1].Setout.hour-now->tm_hour==1) {                   //发车前一个小时无法售票 
			    if(now->tm_min<car[len-1].Setout.minutes){
				    if(car[len-1].Fixednumber<car[len-1].Fixnumber)   {
					(car[len-1].Fixednumber)++;
					ofstream outfile;
					outfile.open("班次信息.txt",ios::out);
					for(i=0; i<R; i++)     {
						outfile<<car[i].Number<<" "<<car[i].Setout.hour<<" "<<car[i].Setout.minutes<<" "<<car[i].Qidian<<" "<<car[i].Zhongdian<<" "<<car[i].Lasttime<<" "<<car[i].Fixnumber<<" "<<car[i].Fixednumber<<endl;
					}
					outfile.close();
					cout<<"你的订票成功，请按时上车，谢谢使用!"<<endl;
				    }          else cout<<"对不起，今天的这趟车的票已卖完,请明天再来，谢谢合作"<<endl;
			    }
			    else cout<<"对不起，今天的这趟车已过了售票时间,请明天再来，谢谢合作"<<endl; 
			}  
			if(car[len-1].Setout.hour-now->tm_hour>=2||car[len-1].Setout.hour-now->tm_hour==0&&car[len-1].Setout.minutes-now->tm_min>30、) {
				if(car[len-1].Fixednumber<car[len-1].Fixnumber)  {
					(car[len-1].Fixednumber)++;
					ofstream outfile;
					outfile.open("班次信息.txt",ios::out);
					for(i=0; i<R; i++)     {
						outfile<<car[i].Number<<" "<<car[i].Setout.hour<<" "<<car[i].Setout.minutes<<" "<<car[i].Qidian<<" "<<car[i].Zhongdian<<" "<<car[i].Lasttime<<" "<<car[i].Fixnumber<<" "<<car[i].Fixednumber<<endl;
					}
					outfile.close();
					cout<<"你的订票成功，请按时上车，谢谢使用!"<<endl;
				}         
				else cout<<"对不起，今天的这趟车的票已卖完,请明天再来，谢谢合作"<<endl;
			}
			 
			
		    else	cout<<"对不起，今天的这趟车已过了售票时间,请明天再来，谢谢合作"<<endl;
		
		
			
			cout<<"输入回车键返回"<<endl;
			getchar();
			getchar();
		}
	}
	
	if(ifAdmin==1){
		adminMenu();
	}else{
		userMenu();
	}
}   //退订火车票子函数
void tuipiao() {
	int R;
	R=loading();
	int i=0;
	string SearchNumber;
	cout<<"请输入要退购的车票的班次:"<<endl;
	cin>>SearchNumber;
	time_t tval;
	struct tm *now;
	tval = time(NULL);
	now = localtime(&tval);
	for(int len=1;len<(sizeof(car)/sizeof(car[0]));len++){
		if(car[len-1].Number == SearchNumber){
				if((now->tm_hour==car[len-1].Setout.hour&&now->tm_min<car[len-1].Setout.minutes)||(now->tm_hour<car[len-1].Setout.hour))  {
					if(car[len-1].Fixednumber>=1)    {
						car[len-1].Fixednumber-=1;
						ofstream outfile;
						outfile.open("班次信息.txt",ios::out);
						for(i=0; i<R; i++)     {
							outfile<<car[i].Number<<" "<<car[i].Setout.hour<<" "<<car[i].Setout.minutes<<" "<<car[i].Qidian<<" "<<car[i].Zhongdian<<" "<<car[i].Lasttime<<" "<<car[i].Fixnumber<<" "<<car[i].Fixednumber<<endl;
						}
						outfile.close();
						cout<<"退订车票成功，谢谢使用!"<<endl;
					}           else cout<<"对不起,今天的这趟车的票尚未卖出,无法完成退票!"<<endl;
				} else cout<<"对不起，今天的这趟车已发出,无法完成退票!"<<endl;
				cout<<"输入回车键返回"<<endl;
				getchar();
				getchar();
		}
	}
	
	if(ifAdmin==1){
		adminMenu();
	}else{
		userMenu();
	}
}
//退出子函数
void quit() {
	cout<<"*********************谢谢使用本系统，欢迎下次继续使用***********************"<<endl;
	welcome() ;
}
//文件载入子函数
int loading() {
	int i;
	ifstream infile;
	infile.open("班次信息.txt",ios::in);
	if(!infile)  {
		cout<<"请查看原文件是否存在!"<<endl;
		cout<<"按任意键返回";
		getchar();
		getchar();
		if(ifAdmin==1){
			adminMenu();
		}else{
			userMenu();
		}
	}
	for(i=0; !infile.eof(); i++)  {
		infile>>car[i].Number>>car[i].Setout.hour>>car[i].Setout.minutes>>car[i].Qidian>>car[i].Zhongdian>>car[i].Lasttime>>car[i].Fixnumber>>car[i].Fixednumber;
	}
	return i-1;
}
int fileExists(string filename){
	fstream _file;
	_file.open(filename.c_str(),ios::in);
	if(!_file){
		_file.close();
		return 0;
    }else{
    	_file.close();
		return 1;
    }
}

void adminMenu(){
	cout<<"________________________________________________________"<<endl;
	cout<<"| 欢 * 迎 * 使 * 用 * 火 * 车 * 票 * 管 * 理 * 系 * 统 |"<<endl;
	cout<<"|                                                      |"<<endl;
	cout<<"|             本管理系统有以下功能:                    |"<<endl;
	cout<<"|                                                      |"<<endl;
	cout<<"|                 1 班次录入                           |"<<endl;
	cout<<"|                 2 浏览班次信息                       |"<<endl;
	cout<<"|                 3 查询火车信息                       |"<<endl;
	cout<<"|                 4 购买火车票                         |"<<endl;
	cout<<"|                 5 退订火车票                         |"<<endl;
	cout<<"|                 6 退出系统                           |"<<endl;
	cout<<"|______________________________________________________|"<<endl;
	cout<<"请输入所要执行功能前的数字: " << endl;
	adminChoose();
}

void userMenu(){
	cout<<"________________________________________________________"<<endl;
	cout<<"| 欢 * 迎 * 使 * 用 * 火 * 车 * 票 * 管 * 理 * 系 * 统 |"<<endl;
	cout<<"|                                                      |"<<endl;
	cout<<"|             本管理系统有以下功能:                    |"<<endl;
	cout<<"|                                                      |"<<endl;
	cout<<"|                 1 浏览班次信息                       |"<<endl;
	cout<<"|                 2 查询火车信息                       |"<<endl;
	cout<<"|                 3 购买火车票                         |"<<endl;
	cout<<"|                 4 退订火车票                         |"<<endl;
	cout<<"|                 5 退出系统                           |"<<endl;
	cout<<"|______________________________________________________|"<<endl;
	cout<<"请输入所要执行功能前的数字: "<<endl;
	userChoose();	
}


