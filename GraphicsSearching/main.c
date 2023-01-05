//#define _CRT_SECURE_NO_WARNINGS		/* ����Visual studio scanf���� */
#include <stdio.h>
#include <stdlib.h>
#define maximum 100

typedef struct nodeList {
	int vertex;
	struct nodeList* next;
}Node;

Node* adjacentList[maximum + 1];		/* adjacent list�A���t���C�@�ӳQ�s�J��vertex */
int visited[maximum + 1], numberOfVertex = 0;
										/* �����O�_�Q���X�L�A�����Q��J���`�I�� */
Node* findLastNode(Node*);
void createAdjacentMatrix(int[][30], int*, int);
void buildAdjacentList(void);
void printAdjacentList(void);
void DepthFirstSearch(int);
void BreadthFirstSearch(int);

int main(int argc, char *argv[]) {
	buildAdjacentList();
	printAdjacentList();

	printf("DFS���G: ");
	DepthFirstSearch(0);
	printf("end\n");

	printf("BFS���G: ");
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
�d�ҿ�J: 
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

Node* findLastNode(Node* current) {				/* ��Yadjacency list�̫�@��node����m�A�H��K�[�J�U�@�Ӧ��s�����`�I */
	while (current->next) {
		current = current->next;
	}
	return current;
}
												/* �N�ϥΪ̿�J���ݾ��T�ഫ��adjacency matrix */
void createAdjacentMatrix(int adjacentMatrix[][30], int* vertexConnectList, int count) {
	int i, j;

	for (i = 0; i < numberOfVertex; i++) {		/* matrix��l�� */
		for (j = 0; j < numberOfVertex; j++) {
			adjacentMatrix[i][j] = 0;
		}
	}

	for (i = 0; i < count; i++) {				/* ø�smatrix */
		adjacentMatrix[vertexConnectList[2 * i] - 1][vertexConnectList[2 * i + 1] - 1] = 1;
		adjacentMatrix[vertexConnectList[2 * i + 1] - 1][vertexConnectList[2 * i] - 1] = 1;
	}
}

void buildAdjacentList(void) {					/* �Nadjacency matrix�ഫ��adjacency list�x�s */
	int j;
	int i = 0, count = 0;
	int adjacentMatrix[30][30], vertexConnectList[2*maximum];
	Node* lastOne;

	printf("�п�J�@���X�Ӹ`�I: ");
	scanf("%d", &numberOfVertex);
	while (numberOfVertex <= 0) {				/* �����\�`�I�Ƭ�0�έt�� */
		printf("�L�Ī��`�I�ơA�Э��s��J�`�I��: ");
		scanf("%d", &numberOfVertex);
	}

	printf("�п�J�ݾ��T(�Ҧp1�P3�s���A�h��J1 3�A�H�Ů�Ϥ��A��J-1 -1�Y�i�h�X):\n�`�N:�s�������q1�}�l�A�B�`�I�ƭȥ����O1<=x<=n�A�B�s�򤣥i���_�Y�@�Ʀr�An���`�I��\n");
	i = -2;
	do {
		i += 2;
		scanf("%d %d", &vertexConnectList[i], &vertexConnectList[i+1]);
		count++;
	} while (vertexConnectList[i] != -1 && vertexConnectList[i+1] != -1);

	for (i = 0; i < numberOfVertex; i++) {		/* ��l�ƩҦ��`�I��adjacency list */
		visited[i] = 0;
		adjacentList[i] = (Node*)calloc(1, sizeof(Node));
		adjacentList[i]->next = NULL;
		adjacentList[i]->vertex = i + 1;
	}													
												/* ���o������adjacency matrix */
	createAdjacentMatrix(adjacentMatrix, vertexConnectList, count);		

	for (i = 0; i < numberOfVertex; i++) {		/* �ഫadjacency matrix��adjacency list */
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

void printAdjacentList(void) {					/* �Ϋ���Ū���ÿ�X�C��adjacency list������ */
	Node* current;
	int i;

	printf("\n");
	printf("Adjacency list���G�C��:\n");
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
												/* Ū��currentVertex���ȡA�Хܬ��w���X */
	printf("%d -> ", adjacentList[currentVertex]->vertex);
	visited[currentVertex] = 1;
	pointer = adjacentList[currentVertex]->next;

	do {										/* �Q��adjacency list���U�@�����ӷ�currentVertex���� */
		findNextVertex = pointer->vertex - 1;		
		if (!visited[findNextVertex]) {			/* Case1: �ӭȤw�g�Q���X�A�h����U�@��adjacency list */
			DepthFirstSearch(findNextVertex);
		}
		else {									/* Case2: ��adjacency list�٦��ȨS���Q���X�A�~��adjacency list���U�@��vertex */
			pointer = pointer->next;
		}
	} while (pointer);							/* ���X��adjacency list���ȡA���Xfunction */
}	

void BreadthFirstSearch(int currentVertex) {	
	int i;
	int front = 0, rear = 1;						/* queue������ */
	int* queue = (int*)calloc(maximum + 1, sizeof(int));

	if (!adjacentList[currentVertex]) {				/* ����Ū�����adjacency list */
		return;
	}

	for (i = 0; i < numberOfVertex; i++) {			/* ��l��visited�����}�C */
		visited[i] = 0;
	}

	Node* pointer = adjacentList[currentVertex];	/* ���X�Ĥ@�ӭ� */
	visited[currentVertex] = 1;
	printf("%d -> ", pointer->vertex);

	do {
		if (!pointer) {								/* Case1: ��node�S���P�����p��node�A�N����̩��A�ݭn��U�@��adjacency list */
			pointer = adjacentList[queue[front] - 1];
			front++;
		}
		else {										/* Case2: ��node���۳s��node�s�b */
			pointer = pointer->next;
		}

		if (pointer) {								/* ��U�@��node�A�d�ݬO�_���X�L�A�p�G�S���h����P��node�A�í��ƫ��X�ʧ@ */
			if (visited[pointer->vertex - 1] == 0) {
				rear++;
				queue[rear] = pointer->vertex;		/* �x�s�U�@�ӥi�઺adjacecy list�� */
				printf("%d -> ", pointer->vertex);
				visited[pointer->vertex - 1] = 1;
			}
		}
	} while (rear < numberOfVertex);			
	printf("end\n");

	free(queue);
}
