#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <cctype>
#include <fstream> 
#include <string>
#include <sstream>

using namespace std;


//RSA算法所需参数
typedef struct  RSA_PARAM_Tag
{
    unsigned __int64    p, q;   //两个素数，不参与加密解密运算
    unsigned __int64    f;      //f=(p-1)*(q-1)，不参与加密解密运算
    unsigned __int64    n, e;   //公匙，n=p*q，gcd(e,f)=1
    unsigned __int64    d;      //私匙，e*d=1 (mod f)，gcd(n,d)=1'
    unsigned __int64    s;      //块长，满足2^s<=n的最大的s，即log2(n)
} RSA_PARAM;

//小素数表
const static long g_PrimeTable[]=
{

    3,5,7,11,13,17,19,23,29,31,37,41,43,

    47,53,59,61,67,71,73,79,83,89,97

};

const static long       g_PrimeCount=sizeof(g_PrimeTable) / sizeof(long);
const unsigned __int64  multiplier=12747293821;
const unsigned __int64  adder=1343545677842234541;//随机数类

//随机数生成
class       RandNumber
{
private:
   unsigned __int64    randSeed;
public:
    RandNumber(unsigned __int64 s=0);
    unsigned __int64    Random(unsigned __int64 n);
};

RandNumber::RandNumber(unsigned __int64 s)
{
    if(!s)
	{
        randSeed= (unsigned __int64)time(NULL);
    }
    else
    {
        randSeed=s;
    }
}

unsigned __int64 RandNumber::Random(unsigned __int64 n)
{
    randSeed=multiplier * randSeed + adder;
    return randSeed % n;
}static RandNumber   g_Rnd;

//模乘运算，返回值 x=a*b mod n
inline unsigned __int64 MulMod(unsigned __int64 a, unsigned __int64 b, unsigned __int64 n)
{
    return a * b % n;
}

//模幂运算，返回值 x=base^pow mod n
unsigned __int64 PowMod(unsigned __int64 &base, unsigned __int64 &pow, unsigned __int64 &n)
{
    unsigned __int64    a=base, b=pow, c=1;
    while(b)
    {
        while(!(b & 1))
        {
            b>>=1;            //a=a * a % n;    
			//函数看起来可以处理64位的整数，但由于这里a*a在a>=2^32时已经造成了溢出，因此实际处理范围没有64位
            a=MulMod(a, a, n);
        }        b--;        //c=a * c % n;        //这里也会溢出，若把64位整数拆为两个32位整数不知是否可以解决这个问题。

        c=MulMod(a, c, n);
    }    return c;
}

/*
Rabin-Miller素数测试，通过测试返回1，否则返回0。
n是待测素数。
*/

long RabinMillerKnl(unsigned __int64 &n)
{
    unsigned __int64    b, m, j, v, i;
    m=n-1;
    j=0;    //计算出m、j，使得n-1=m*2^j，其中m是正奇数，j是非负整数
    while(!(m & 1))
    {
        ++j;
        m>>=1;
    }    //随机取一个b，2<=b<n-1
    b=2 + g_Rnd.Random(n - 3);    //算v=b^m mod n
    v=PowMod(b, m, n);    //如果v==1，通过测试
    if(v == 1)
    {
        return 1;
    }    //令i=1
    i=1;    //v=n-1，通过测试
    while(v != n - 1)
    {
       //i==l，非素数，结束
        if(i == j)
        {
            return 0;
        }        //v=v^2 mod n，i=i+1
        unsigned __int64  xxx;
        int xxxx = 2;
        xxx = xxxx;
        v = PowMod(v, xxx, n);
        ++i;        //循环到5
    }    return 1;
}

/*
Rabin-Miller素数测试，循环调用核心loop次
全部通过返回1，否则返回0
*/

long RabinMiller(unsigned __int64 &n, long loop)
{
    //用小素数筛选一次，提高效率
    for(long i=0; i < g_PrimeCount; i++)
    {
        if(n % g_PrimeTable[i] == 0)
        {
            return 0;
        }
    }    //循环调用Rabin-Miller测试loop次，使得非素数通过测试的概率降为(1/4)^loop
    for(long i=0; i < loop; i++)
    {
        if(!RabinMillerKnl(n))
        {
            return 0;
        }
    }    return 1;
}

/*随机生成一个bits位(二进制位)的素数，最多32位*/
unsigned __int64 RandomPrime(char bits)
{
    unsigned __int64    base;
    do
    {
       base= (unsigned long)1 << (bits - 1);   //保证最高位是1
       base+=g_Rnd.Random(base);               //再加上一个随机数
        base|=1;    //保证最低位是1，即保证是奇数
    } while(!RabinMiller(base, 30));    //进行拉宾－米勒测试30次
    return base;    //全部通过认为是素数
}


/*欧几里得法求最大公约数*/
unsigned __int64 EuclidGcd(unsigned __int64 &p, unsigned __int64 &q)
{
    unsigned __int64    a=p > q ? p : q;
    unsigned __int64    b=p < q ? p : q;
    unsigned __int64    t;
    if(p == q)
    {
        return p;   //两数相等，最大公约数就是本身
    }
    else
    {
        while(b)    //辗转相除法，gcd(a,b)=gcd(b,a-qb)
        {
            a=a % b;
            t=a;
            a=b;
            b=t;
        }        return a;
    }

}

/*Stein法求最大公约数*/
unsigned __int64 SteinGcd(unsigned __int64 &p, unsigned __int64 &q)
{
    unsigned __int64    a=p > q ? p : q;
    unsigned __int64    b=p < q ? p : q;
    unsigned __int64    t, r=1;
    if(p == q)
    {
       return p;           //两数相等，最大公约数就是本身
    }
    else
    {
        while((!(a & 1)) && (!(b & 1)))
        {
            r<<=1;          //a、b为偶数时，gcd(a,b)=2*gcd(a/2,b/2)
            a>>=1;
            b>>=1;
        }       
		if(!(a & 1))
        {
            t=a;            //a为偶数，交换a，b
            a=b;
            b=t;
        }       
		do
        {
            while(!(b & 1))
            {
                b>>=1;      //b为偶数，a为奇数时，gcd(b,a)=gcd(b/2,a)
            }           
			if(b < a)
            {
                t=a;        //b小于a，交换a，b
                a=b;
                b=t;
            }           
			b=(b - a) >> 1; //b、a都是奇数，gcd(b,a)=gcd((b-a)/2,a)
        } while(b);
        return r * a;
    }
}

/*
已知a、b，求x，满足a*x =1 (mod b)
相当于求解a*x-b*y=1的最小整数解
*/

unsigned __int64 Euclid(unsigned __int64 &a, unsigned __int64 &b)
{
    unsigned __int64    m, e, i, j, x, y;
    long                xx, yy;
    m=b;e=a;x=0;y=1;xx=1;yy=1;
    while(e)
    {
        i=m / e;j=m % e;
       m=e;e=j;j=y;y*=i;
        if(xx == yy)
        {
            if(x > y)
                y=x - y;
            else{
                y-=x;
                yy=0;
            }
        }
        else
        {
            y+=x;
           xx=1 - xx;
           yy=1 - yy;
        }       
		x=j;
    }    
    if(xx == 0)
        x=b - x;
    return x;
}

/*
随机产生一个RSA加密参数
*/
RSA_PARAM RsaGetParam(void)
{
    RSA_PARAM           Rsa={ 0 };//自定义的RSA参数类型
    unsigned __int64    t;
    Rsa.p=RandomPrime(16);          //随机生成两个素数
    Rsa.q=RandomPrime(16);
    Rsa.n=Rsa.p * Rsa.q;
    Rsa.f=(Rsa.p - 1) * (Rsa.q - 1);
    do
    {
        Rsa.e=g_Rnd.Random(65536);  //小于2^16，65536=2^16
        Rsa.e|=1;                   //保证最低位是1，即保证是奇数，因f一定是偶数，要互素，只能是奇数
    } while(SteinGcd(Rsa.e, Rsa.f) != 1);    
    Rsa.d=Euclid(Rsa.e, Rsa.f);
    Rsa.s=0;
    t=Rsa.n >> 1;
    while(t)
    {
        Rsa.s++;                    //s=log2(n)
        t>>=1;
    }    
    return Rsa;
}

/*
拉宾－米勒测试
*/

void TestRM(void)
{
    unsigned long   k=0;
   cout << "拉宾－米勒测试\n" << endl;
    for(unsigned __int64 i=4197900001; i < 4198000000; i+=2)
    {
        if(RabinMiller(i, 30))
        {
            k++;
            cout<<i<<endl;
        }
    }   
	cout << "Total: " << k << endl;
}

//密钥安全性检测
int fun(int k)
{        
    int count=1;//私钥5位以下弱

        if(k>5) 
			count++;//私钥6-7位中
       if(k>7) 
			count++;//私钥8-9位强
       if(k>9)
			count++;//私钥10位以上非常好
  
       return count;
}

//判断整数位数
int get_length(char x[])
{
	int leng=0,i;
	string str;
     str = x;
	stringstream ss;
	ss<<str;
    ss>>i;

	while(i)
	{
		i/=10;
		leng++;
	}
	return leng;
}

//解密函数
void decode(char cipher[])
{
int s[256],t[256],i,j,temp;
char k[256];//用户输入的密钥
char plaintext[1024];
cout<<"\n请您输入秘密密钥:";
cin>>k;

//初始化S
for(i=0;i<256;i++)    {
s[i]=i;
t[i]=k[i%strlen(k)];
}

//S的初始置换，生成最终s数组
j=0;
for( i=0;i<256;i++) //密钥长度可变
{
j=(j+s[i]+t[i])%256;
temp=s[i];
s[i]=s[j];
s[j]=temp;
}

int m,n,key[256],q;
m=n=0;
cout<<"\n经本程序解密得到的明文是："<<endl;

//流产生
for(i=0;i<strlen(cipher);i++){
m=(m+1)% 256;
n=(n+s[n])% 256;
temp=s[m];
s[m]=s[n];
s[n]=temp;
q=(s[m]+s[n])%256;
key[i]=s[q];
plaintext[i]=cipher[i]^key[i]; //对密文字符进行解密
cout<<plaintext[i];
}
cout<<endl;
}

ofstream fout1("密钥保存.txt");     //创建一个密钥保存.txt的文件
int baocun(char x[],char y[],int kkk)
{
	fout1.open( "密钥保存.txt", ios::app ); 
	fout1 << kkk <<"      "<< x<<"     "<< y<<"\n";
	fout1.close();
	return kkk++;
}

string ReadText(int line)
{
	ifstream inff("密钥保存.txt") ;
	string strVec[100];     //文本中总共有10行
	int i = 0;
	while (!inff.eof())
	{
		string inbuf;
		getline(inff, inbuf, '\n');
		strVec[i] = inbuf;
		i = i + 1;
	}
	return strVec[line - 1];
}

string CharToStr(char * contentChar)//字符串转string类型
{
	string tempStr;
	for (int i=0;contentChar[i]!='\0';i++)
	{
		tempStr+=contentChar[i];
	}
	return tempStr;
}

int main(void)
{ 
	int c=0;
	RSA_PARAM           r;
	char miyao[50]={0};//随机选取的秘钥，存放在该数组中
	string lineData ;
	string tmpLine ;
	char siyao[50]={0},gongyao[50]={0};
	string in_1;
	char pSrc[100]={0};
	const unsigned long n = sizeof(pSrc);
	unsigned char       *q, pDec[n];
	unsigned __int64    pEnc[n];
	int kk,kkkk;
	int yonghu=0,sc=0;
  
	int i=0,j=0,si=0;
    int length ;
	int g;
	char ciphertext1[1024]={0},ciphertext2[1024]={0};
	char k[256]={0};//将用户输入的密钥存放入K数组中，密钥长度最大为256位
	int temp,s[256]={0},t[256]={0};

	ofstream fout("密钥库.txt");     //创建一个data.txt的文件
	for(int i=0;i<=100;i++)
	{
		r = RsaGetParam();
		fout << r.e << "," << r.d << endl;  //将变量的值写入文件
	}
	fout.close();                  //关闭文件
  
	do{
		system("cls");
	cout << "--------------------------------------------------------\n"  ;
    cout << "                  欢迎进入密钥管理系统\n\n";
	cout << "----------by：B16070404陈慧+B16070405胡雪然-------------\n\n";
	cout << "           1.密钥分发+密钥安全性评判 \n\n";
	cout << "           2.密钥加密存储 \n\n";
	cout << "           3.查看系统用户保密密钥           \n\n";
	cout << "           4.解密用户保密密钥           \n\n";
	cout << "           5.密钥应用：使用密钥对进行加密解密           \n\n";
	cout << "           6.删除用户信息           \n\n";
	cout << "           0.退出 \n\n";
	cout << "--------------------------------------------------------\n\n";
	cout <<"请在0-6中选择以回车键结束：";
	cin >>c;

	ifstream inff("密钥库.txt") ;
	switch(c)
	{
	  case 1:
		    system("cls");
	//在文件中随机选择一对秘钥
			yonghu++;
			i=1;
			srand((unsigned int)time(NULL)) ;
			while (getline(inff, tmpLine))
			{
				
				if (rand()%i == 0)
					lineData = tmpLine ;
				++ i ;
			}
			inff.close() ;

			strcpy(miyao, lineData.c_str());
			length = sizeof(miyao)/sizeof(char);
			i=0;
			while (i<length) 
			{
				while ( miyao[i]!=',' && i==j)
				{
					gongyao[i]=miyao[i];
					i++;
					j++;

				}
			siyao[i-j]=miyao[i+1];
			i++;
			}//实现公钥私钥分别放入数组

		  cout << "\n系统给您分发的秘钥对为（公钥，私钥）: ("<<gongyao<<","<<siyao<<")"<<endl;

			si=get_length(siyao);
			cout<<"\n经长度判断，密码强度为：";
			switch(fun(si))
			{
			case 1:cout<<"弱 ★"<<endl;break;
			case 2:cout<<"中 ★ ★"<<endl;break;
			case 3:cout<<"强 ★ ★ ★"<<endl;break;
			case 4:cout<<"非常好 ★ ★ ★ ★"<<endl;break;
			}
		
			cout<<"\n\n注：强度检验规则为：\n";
			cout<<"        私钥位数5位以下弱★\n";
			cout<<"                6-7位中 ★ ★\n";
			cout<<"                8-9位强★ ★ ★\n";
			cout<<"                10位以上非常好★ ★ ★ ★";

			cout <<"\n\n按任意键返回主菜单，0键退出\n";
			cin>>c;
			break;

	  case 2:
		  system("cls");
		  	cout << "\n密钥的加密保存和管理—RC4算法    \n";

		
		cout<<"请您输入秘密密钥:";
		cin>>k;


		//公钥加密gongyao
		//初始化S
		for( i=0;i<256;i++) 
		{
			s[i]=i;
			t[i]=k[i%strlen(k)]; //用K对临时向量T进行赋值
		}
		//S的初始置换
		j=0; 
		for(i=0;i<256;i++)
		{
			j=(j+s[i]+t[i])%256;
			temp=s[i];
			s[i]=s[j];
			s[j]=temp;
		}

		cout<<"\n\n公钥加密得到的密文为:";
		int m,n,key[256],ff;
		m=n=0;
		//流产生
		 g=get_length(gongyao);
		for(i=0;i<g;i++)//对明文字符进行加密
		{
			m=(m+1)% 256;
			n=(n+s[n])% 256;
			temp=s[m];
			s[m]=s[n];
			s[n]=temp;
			ff=(s[m]+s[n])%256;
			key[i]=s[ff];
			ciphertext1[i]=gongyao[i]^key[i];//加密过程，K值与明文的下一字节作异或。
			cout<<ciphertext1[i];
		}
			ciphertext1[i]='\0';
			cout<<endl;

		//私钥加密r.d

        cout<<"\n私钥加密得到的密文为:";
		int l;
		m=n=0;

		//初始化S
		for(i=0;i<256;i++) 
		{
			s[i]=i;
			t[i]=k[i%strlen(k)]; //用K对临时向量T进行赋值
		}
		//S的初始置换
		j=0; 
		for( i=0;i<256;i++)
		{
			j=(j+s[i]+t[i])%256;
			temp=s[i];
			s[i]=s[j];
			s[j]=temp;
		}
		//流产生
		 si=get_length(siyao);
		 i=0;
		for(  i;i<si;i++)//对明文字符进行加密
		{
		m=(m+1)% 256;
		n=(n+s[n])% 256;
		temp=s[m];
		s[m]=s[n];
		s[n]=temp;
		ff=(s[m]+s[n])%256;
		key[i]=s[ff];
		ciphertext2[i]=siyao[i]^key[i];//加密过程，K值与明文的下一字节作异或。
		cout<<ciphertext2[i];
		}
		ciphertext2[i]='\0';

		if(yonghu==1)
		{
		ofstream fout1("密钥保存.txt");     //创建一个密钥保存.txt的文件
		}
		else
		{
		fout1.open( "密钥保存.txt", ios::app ); 
		}
		fout1 << yonghu <<"      "<< ciphertext1<<"     "<< ciphertext2<<"\n";
		fout1.close();
		cout<<"\n\n第"<<yonghu<<"位用户系统分配的秘钥已经加密存储";
		cout<<endl;

		  cout <<"\n\n按任意键返回主菜单，0键退出";
		  cin>>c;
		  break;

		case 3:
			system("cls");
			cout<<"查看用户加密密钥,请输入用户编号:";
			cin>>kkkk;
			cout <<"\n用户编号  公钥加密密文  私钥加密密文"<<endl;
			cout <<ReadText(kkkk)<<endl;
			cout <<"\n\n按任意键返回主菜单，0键退出";
			cin>>c;
			break;

		case 4:
			system("cls");
			cout<<"\n公钥解密得";
			decode(ciphertext1);//解密函数
			cout<<"\n私钥解密得";
			decode(ciphertext2);//解密函数
			cout <<"\n\n按任意键返回主菜单，0键退出";
			cin>>c;
			break;

	  case 5:
		  system("cls");
		  cout << "\nRSA算法加解密应用： \n";
		  cout << "请输入待加密的内容（支持字母、汉字、以及其他符号和下划线）：\n";
		fflush(stdin);
		scanf("%[^\n]s",pSrc);
		q = (unsigned char*)pSrc;
		cout << "\n\n加密（密文ASCII码）:\n";
		for (unsigned long i = 0; i < n; i++)
		{
			unsigned __int64  xxx;
			unsigned __int64 xxxx = q[i];
			xxx = xxxx;
			pEnc[i] = PowMod(xxx, r.e, r.n);
			cout << hex << pEnc[i] << " ";
		}    
		cout << "\n\n";    
		cout << "解密（明文ASCII码）:\n";
		for (unsigned long i = 0; i < n; i++)
		{
			pDec[i] = PowMod(pEnc[i], r.d, r.n);
			cout << hex << (unsigned long)pDec[i] << " ";
		}    
		cout << "\n\n";
		cout << "解密后的文档：\n";
		cout << (char *)pDec << endl;

		  cout <<"\n\n按任意键返回主菜单，0键退出";
		  cin>>c;
		  break;


	  case 6:
		  {
		  cout <<"请输入您要删除的用户编号";
		  cin>>sc;
		  ifstream in;
	      in.open("密钥保存.txt");
	
		string strFileData = "";
		int line = 1;
		char lineData[1024] = {0};
		while(in.getline(lineData, sizeof(lineData)))
		{
			if (line == sc)
			{
				strFileData += "\n";
			}
			else
			{
				strFileData += CharToStr(lineData);
				strFileData += "\n";
			}
			line++;
		}
		in.close();
			ofstream out;
	out.open("密钥保存.txt");
	out.flush();
	out<<strFileData;
	out.close();
		  cout <<"\n\n按任意键返回主菜单，0键退出";
		  cin>>c;
		  break;
		  }
	}
	}while (c!=0);

	cout <<"谢谢使用，再见！"; 
	system ("pause");
}


 

