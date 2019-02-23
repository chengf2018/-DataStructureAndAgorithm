/*
 *   斐波那契数列的几种实现方式
 */

#include <iostream>
#include <stack>
#include <vector>
#include <time.h>
#include <sys/time.h>
#include <queue>
using namespace std;

//用于测试，获取当前时间
unsigned long long getCurrentTime()
{    
   struct timeval tv;    
   gettimeofday(&tv, 0);
   return tv.tv_sec * 1000 + tv.tv_usec / 10000;    
}

//递归实现 速度非常慢
unsigned long long func(int n) {
	if (n <= 2)
		return 1;
	else {
		return func(n-1) + func(n-2);
	}
}

//循环实现
unsigned long long func1(int n) {
	unsigned long long result = 0;
	unsigned long long n1 = 1, n2 = 1;

	if (n <= 2) return 1;
	
	for (int i = 2; i < n; i++) {
		result = n1 + n2;
		n1 = n2;
		n2 = result;
	}
	return result;
}

//数组实现
unsigned long long func2(int n) {
	vector<unsigned long long> vec;
	vec.push_back(1);
	vec.push_back(1);

	for (int i = 2; i < n; i++) {
		vec.push_back(vec[i - 1] + vec[i - 2]);
	}

	return vec[vec.size()-1];
}

//栈实现
unsigned long long func3(int n) {
	if (n <= 2) return 1;
	
	stack<unsigned long long> s;
	s.push(1);
	s.push(1);

	unsigned long long n1 , n2, result = 0;
	for (int i = 2; i < n; i++) {
		n1 = s.top();
		s.pop();
		n2 = s.top();
		s.pop();

		result = n1 + n2;

		s.push(result);//注意入栈顺序
		s.push(n2);
	}

	return result;
}

//队列实现
unsigned long long func4(int n) {
	if (n <= 2) return 1;

	queue<unsigned long long> q;
	q.push(1);
	q.push(1);

	unsigned long long n1 , n2, result = 0;
	for (int i = 2; i < n; i++) {
		n1 = q.front();
		q.pop();
		n2 = q.front();
		q.pop();

		result = n1 + n2;
		q.push(n2);
		q.push(result);
	}
	return result;
}

int main(int argc, char **argv) {
	unsigned long long result[5] = {0};
	int n = 1;
	
	cout << "请输入斐波那契数列的n(必须大于等于1):";
	cin >> n;

	unsigned long long oldtime = getCurrentTime();

	//result[0] = func(n);
	result[1] = func1(n);
	result[2] = func2(n);
	result[3] = func3(n);
	result[4] = func4(n);

	unsigned long long nowtime = getCurrentTime();
	
	for (int i = 0; i < sizeof(result)/sizeof(result[0]); i++) {
		cout << result[i] << endl;
	}

	cout << "using time:" << nowtime - oldtime << endl;
	

	cin >> n;//pause
	return 0;
}