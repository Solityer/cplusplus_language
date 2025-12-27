#include "string.h"

void test_string()
{
	string s1;
	string s2("hello");
	cin >> s1;
	cout << s1 << endl;
	cout << s2 << endl;

	string ret1 = s1 + s2;
	cout << ret1 << endl;
	string ret2 = s1 + "pzh";
	cout << ret2 << endl;
}

void func(const string& s)
{
	string::const_iterator it = s.begin();
	while (it != s.end()) 
	{
		//*it += 1;  //错误	C3892	“it” : 不能给常量赋值	string
		cout << *it << " ";
		++it;
	}
}

int main()
{
	test_string();

	//GBK
	char str1[] = "apple";
	//GBK编码，一个中文字符占2个字节；UTF-8编码，一个中文字符占3个字节
	char str2[] = "小透明";
	cout << sizeof(str2) << endl;
	str2[3]++;
	cout << str2 << endl;
	str2[3]--;
	cout << str2 << endl;

	//无参string
	string s1;
	string s2("hello solity");
	cout << s2 << endl;
	string s3 = "hello solity\0";
	cout << s3 << endl;
	string s4(s3,0,4);  //从第0个字符往后的4个字符(前闭后开)
	cout << s4 << endl;
	string s5(s3,2);  //从s3的第二个字符往后的所有字符
	cout << s5 << endl;
	string s6("hello solity", 4);  //前4个字符
	cout << s6 << endl;
	string s7(10, '*');  //10个*（指定个数个指定符号）
	cout << s7 << endl;

	string s8("hello solity");
	cout << s8.size() << endl;  //size()实际字符数量，随字符串修改实时变化
	cout << s8.length() << endl;
	cout << s8.max_size() << endl; //实际可以的最大长度
	//capacity()可存储的最大字符数	只增不减，除非手动调整
	cout << s8.capacity() << endl;  //容量大小

	string s9("hello solity");
	s9.push_back(' ');
	s9.push_back('!'); //push_back插入一个字符
	s9.append("wolrd");   //append插入字符串
	/*简化插入操作*/
	s9 += ' ';
	s9 += '!';
	s9 += "world";
	cout << s9 << endl;

	//只改变容量  扩容
	string s10("hello solity");
	s10.reserve(100); //预留至少100个字符的空间
	cout << s10.size() << endl;
	cout << s10.capacity() << endl;

	//扩容＋初始化
	string s11("hello solity");
	s11.resize(100);
	cout << s11.size() << endl;
	cout << s11.capacity() << endl;

	//普通迭代器
	//正向
	string s12("hello solity");
	string::iterator it = s12.begin();
	while (it != s12.end())
	{
		cout << *it << " ";
		++it;
	}
	cout << endl;
	//反向
	string::reverse_iterator rit = s11.rbegin();
	while (rit != s11.rend())
	{
		cout << *rit << " ";
		++rit;
	}
	cout << endl;

	//const迭代器
	func(s12);
	cout << endl;

	//at   //很少用
	//与operator[]类似
	//s12[200];  //直接终止程序
	//s12.at(200);  //抛异常

	/*insert*/
	string s13("hello solity");
	s13.insert(0,"pzh"); //不能插入单个字符
	cout << s13 << endl;
	s13.insert(3, 1, ' ');  //插入单个字符解决方法  第3个位置插入1个字符
	s13.insert(3, " ");  //这样也可以
	cout << s13 << endl;

	/*earse*/
	string s14("hello solity");
	s14.erase(5, 1);  //第五个位置删除一个字符
	cout << s14 << endl;
	s14.erase(s14.begin() + 5);
	cout << s14 << endl;

	/*replace*/
	string s15("solitypzh");
	s15.replace(5, 1,"%d%");  //第5个位置开始的1个字符替换为%d%
	cout << s15 << endl;

	/*find*/
	string s16("hello world solity pzh");
	size_t pos = s16.find(' ');  //找到返回第一次匹配的位置，未找到就返回npos
	if(pos != string::npos)
	{
		s16.replace(pos, 1, "%d%");
	}
	cout << s16 << endl;

	string s17("hello world solity pzh");
	size_t pos1 = s17.find(' ');  //找到返回第一次匹配的位置，未找到就返回npos
	while(pos1 != string::npos)
	{
		s17.replace(pos, 1, "%d%");
		pos1 = s17.find(' ',pos1 + 3); //下次从pos+3的位置开始找（默认从头开始）
	}
	cout << s17 << endl;

	string s18("hello world solity pzh");
	size_t num = 0;
	for (auto ch : s18)
	{
		if (ch == ' ')
			++num;
	}
	s18.reserve(s18.size() + 3 * num);
	size_t pos2 = s18.find(' ');
	while (pos2 != string::npos)
	{
		s18.replace(pos2, 1, "%d%");
		pos2 = s18.find(' ', pos2 + 3);
	}
	cout << s18 << endl;

	string s19("hello solity");
	string s20("######");
	s19.swap(s20);  //更高效
	//swap(s19,s20);
	cout << s19 << endl;
	cout << s20 << endl;

	/*find_first_of*/
	//正着找
	//把指定的所有字符全部替换为自定字符
	string str("please,replace the vowels in this sentence by asterisks.");
	size_t found = str.find_first_of("abcd");
	while (found != string::npos)
	{
		str[found] = '*';
		found = str.find_first_of("abcd", found + 1);
	}
	cout << str << endl;

	/*find_last_of*/
	//倒着找

	pzh::func_str1();
	pzh::func_str2();
	pzh::func_str3();

	return 0;
}
