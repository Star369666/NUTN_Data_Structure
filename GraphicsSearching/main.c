//#define _CRT_SECURE_NO_WARNINGS		/* 消除Visual studio scanf報錯 */
#include <stdio.h>
#include <stdlib.h>
#define maximum 100

typedef struct nodeList {
	int vertex;
	struct nodeList* next;
}Node;

Node* adjacentList[maximum + 1];		/* adjacent list，分配給每一個被存入的vertex */
int visited[maximum + 1], numberOfVertex = 0;
										/* 紀錄是否被拜訪過，紀錄被輸入的節點數 */
Node* findLastNode(Node*);
void createAdjacentMatrix(int[][30], int*, int);
void buildAdjacentList(void);
void printAdjacentList(void);
void DepthFirstSearch(int);
void BreadthFirstSearch(int);

int main(int argc, char *argv[]) {
	buildAdjacentList();
	printAdjacentList();

	printf("DFS結果: ");
	DepthFirstSearch(0);
	printf("end\n");

	printf("BFS結果: ");
	BreadthFirstSearch(0);
	printf("\n");

	return 0;
}

/* 
						1
					/		\
				2				4
			/		\		/		\
		7				3				8
	/		\						/		\
5				10				6				9
範例輸入: 
1 2
1 4
2 3
2 7
3 4
4 8
5 7
6 8
7 10
8 9
-1 -1
*/

Node* findLastNode(Node* current) {				/* 找某adjacency list最後一個node的位置，以方便加入下一個有連接的節點 */
	while (current->next) {
		current = current->next;
	}
	return current;
}
												/* 將使用者輸入的拓樸資訊轉換成adjacency matrix */
void createAdjacentMatrix(int adjacentMatrix[][30], int* vertexConnectList, int count) {
	int i, j;

	for (i = 0; i < numberOfVertex; i++) {		/* matrix初始化 */
		for (j = 0; j < numberOfVertex; j++) {
			adjacentMatrix[i][j] = 0;
		}
	}

	for (i = 0; i < count; i++) {				/* 繪製matrix */
		adjacentMatrix[vertexConnectList[2 * i] - 1][vertexConnectList[2 * i + 1] - 1] = 1;
		adjacentMatrix[vertexConnectList[2 * i + 1] - 1][vertexConnectList[2 * i] - 1] = 1;
	}
}

void buildAdjacentList(void) {					/* 將adjacency matrix轉換成adjacency list儲存 */
	int j;
	int i = 0, count = 0;
	int adjacentMatrix[30][30], vertexConnectList[2*maximum];
	Node* lastOne;

	printf("請輸入共有幾個節點: ");
	scanf("%d", &numberOfVertex);
	while (numberOfVertex <= 0) {				/* 不允許節點數為0或負數 */
		printf("無效的節點數，請重新輸入節點數: ");
		scanf("%d", &numberOfVertex);
	}

	printf("請輸入拓樸資訊(例如1與3連接，則輸入1 3，以空格區分，輸入-1 -1即可退出):\n注意:編號必須從1開始，且節點數值必須是1<=x<=n，且連續不可中斷某一數字，n為節點數\n");
	i = -2;
	do {
		i += 2;
		scanf("%d %d", &vertexConnectList[i], &vertexConnectList[i+1]);
		count++;
	} while (vertexConnectList[i] != -1 && vertexConnectList[i+1] != -1);

	for (i = 0; i < numberOfVertex; i++) {		/* 初始化所有節點的adjacency list */
		visited[i] = 0;
		adjacentList[i] = (Node*)calloc(1, sizeof(Node));
		adjacentList[i]->next = NULL;
		adjacentList[i]->vertex = i + 1;
	}													
												/* 取得對應的adjacency matrix */
	createAdjacentMatrix(adjacentMatrix, vertexConnectList, count);		

	for (i = 0; i < numberOfVertex; i++) {		/* 轉換adjacency matrix為adjacency list */
		for (j = 0; j < numberOfVertex; j++) {
			if (adjacentMatrix[i][j] == 1) {
				Node* node = (Node*)calloc(1, sizeof(Node));
				node->vertex = j + 1;
				lastOne = findLastNode(adjacentList[i]);
				lastOne->next = node;
			}
		}
	}
}

void printAdjacentList(void) {					/* 用指標讀取並輸出每個adjacency list的元素 */
	Node* current;
	int i;

	printf("\n");
	printf("Adjacency list結果列表:\n");
	for (i = 0; i < numberOfVertex; i++) {
		current = adjacentList[i];
		printf("%2d", current->vertex);
		current = current->next;
		while (current) {
			printf(" -> ");
			printf("%d", current->vertex);
			current = current->next;
		}
		printf("\n");
	}
}

void DepthFirstSearch(int currentVertex) {
	Node* pointer;
	int findNextVertex;
												/* 讀取currentVertex的值，標示為已拜訪 */
	printf("%d -> ", adjacentList[currentVertex]->vertex);
	visited[currentVertex] = 1;
	pointer = adjacentList[currentVertex]->next;

	do {										/* 利用adjacency list找到下一個應該當成currentVertex的值 */
		findNextVertex = pointer->vertex - 1;		
		if (!visited[findNextVertex]) {			/* Case1: 該值已經被拜訪，則跳到下一個adjacency list */
			DepthFirstSearch(findNextVertex);
		}
		else {									/* Case2: 該adjacency list還有值沒有被拜訪，繼續adjacency list的下一個vertex */
			pointer = pointer->next;
		}
	} while (pointer);							/* 拜訪完adjacency list的值，跳出function */
}	

void BreadthFirstSearch(int currentVertex) {	
	int i;
	int front = 0, rear = 1;						/* queue的指標 */
	int* queue = (int*)calloc(maximum + 1, sizeof(int));

	if (!adjacentList[currentVertex]) {				/* 防止讀取到空adjacency list */
		return;
	}

	for (i = 0; i < numberOfVertex; i++) {			/* 初始化visited紀錄陣列 */
		visited[i] = 0;
	}

	Node* pointer = adjacentList[currentVertex];	/* 拜訪第一個值 */
	visited[currentVertex] = 1;
	printf("%d -> ", pointer->vertex);

	do {
		if (!pointer) {								/* Case1: 該node沒有與之相聯的node，代表做到最底，需要到下一條adjacency list */
			pointer = adjacentList[queue[front] - 1];
			front++;
		}
		else {										/* Case2: 該node有相連的node存在 */
			pointer = pointer->next;
		}

		if (pointer) {								/* 到下一個node，查看是否拜訪過，如果沒有則收獲周圍node，並重複拜訪動作 */
			if (visited[pointer->vertex - 1] == 0) {
				rear++;
				queue[rear] = pointer->vertex;		/* 儲存下一個可能的adjacecy list值 */
				printf("%d -> ", pointer->vertex);
				visited[pointer->vertex - 1] = 1;
			}
		}
	} while (rear < numberOfVertex);			
	printf("end\n");

	free(queue);
}
