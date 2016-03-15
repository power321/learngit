#include <iostream>
#include <algorithm>
using namespace std;

int n;
int heap[505];

inline void cxSwap(int i, int j)
{
	int temp = heap[i];
	heap[i] = heap[j];
	heap[j] = temp;
}

void shiftDown(int node)
{
	while(node<=n/2)
	{
		int temp = node;
		if(node*2<=n && heap[node*2] < heap[temp])
			temp = node * 2;
		if(node*2+1<=n && heap[node*2+1] < heap[temp])
			temp = node * 2 + 1;
		if(temp != node)
		{
			cxSwap(temp, node);
			node = temp;
		}
		else
			break;
	}
}

void create()
{
	for(int i=n/2; i>=1; --i)
		shiftDown(i);
}

void heapSort()
{
	while(n)
	{
		cout << heap[1] << endl;
		heap[1] = heap[n];
		n--;
		shiftDown(1);
	}
	cout << endl;
}

int main()
{
	while(1)
	{
		cin >> n;
		if(n == 0)
			break;

		for(int i=1; i<=n; ++i)
			cin >> heap[i];

		create();

		//for(int i=1; i<=n; ++i)
		//	cout << heap[i] << endl;

		heapSort();
	}
	return 0;
}
