#include <iostream>
#include <algorithm>
using namespace std;

void quickSort(int *array, int left, int right)
{
	if(left >= right)
		return ;
	int temp = array[left];
	int i = left, j = right;
	while(i != j)
	{
		while(array[j]>=temp && j>i)
			j--;
		while(array[i]<=temp && j>i)
			i++;
		if(i != j)
		{
			array[i] ^= array[j];
			array[j] ^= array[i];
			array[i] ^= array[j];
		}
	}
	array[left] = array[i];
	array[i] = temp;
	quickSort(array, left, i-1);
	quickSort(array, i+1, right);
}
int main()
{
	int n,array[2000+5];
	while(1)
	{
		cin >> n;
		if(n == 0)
			break;
		for(int i=0; i<n; ++i)
			cin >> array[i];
		quickSort(array, 0, n-1);
		for(int i=0; i<n; ++i)
		cout << array[i] << endl;
		cout << endl;
	}
	return 0;
}
