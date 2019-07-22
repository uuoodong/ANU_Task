#include <stdio.h>
#include <iostream>
#include <ctime>
#include <fstream>
#include <string>
#include <cstring>

int number = 10000;
int heap[10000];
using namespace std;
int main(void) {
	// 힙을 구성 
	int selectFlag = 0;
	for(int i=number; i>0; i--){
		heap[selectFlag] = i;
		selectFlag++;
	}
	for(int i = 1; i < number; i++) {
		int c = i;
		do {
			int root = (c - 1) / 2;
			if(heap[root] < heap[c]) {
				int temp = heap[root];
				heap[root] = heap[c];
				heap[c] = temp;
			}
			c = root;
		} while (c != 0);
	}
	// 크기를 줄여가며 반복적으로 힙을 구성
	for (int i = number - 1; i >= 0; i--) { 
		int temp = heap[0];
		heap[0] = heap[i];
		heap[i] = temp;
		int root = 0;
		int c = 1;
		do {
			c = 2 * root + 1;
			// 자식 중에 더 큰 값을 찾기 
			if(c < i - 1 && heap[c] < heap[c + 1]) {
				c++;
			}
			// 루트보다 자식이 크다면 교환 
			if(c < i && heap[root] < heap[c]) {
				temp = heap[root];
				heap[root] = heap[c];
				heap[c] = temp;
			}
			root = c;
		} while (c < i);
	} 
	// 결과 출력 
	for(int i = 0; i < number; i++) {
		printf("%d ", heap[i]);
	}
	
}
//https://m.blog.naver.com/PostView.nhn?blogId=ndb796&logNo=221228342808&proxyReferer=https%3A%2F%2Fwww.google.com%2F
