#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int nthSmallCounter = 0;

typedef struct node* treePointer;

typedef struct node {
	int data;
	treePointer leftChild, rightChild;
};

void readKeysFromFile(int*, int*);

treePointer buildRoot(int);

treePointer insertNode(treePointer, int);

treePointer deleteNode(treePointer, int);

treePointer searchNode(treePointer, int);

treePointer isExistNode(treePointer, int);

treePointer findNthSmallNode(treePointer, int);

void printQuickSort(treePointer);

void quickSort(int*, int, int);

void inorder(treePointer);

void getAllNodes(treePointer, int*, int*);

void inoderTraversal(treePointer);

int main(int argc, char* argv[]) {
	treePointer root = NULL;
	int choose, lengthOfKeys, i;
	int flag = 0;

	while (1) {

		if (!flag) {		//檢查是否為第一次使用(是否建立好BST) 
			int inputKeys[100];
			readKeysFromFile(inputKeys, &lengthOfKeys);
			root = insertNode(root, inputKeys[0]);
			for (i = 1; i < lengthOfKeys; i++) {
				insertNode(root, inputKeys[i]);
			}
			printf("\n");
			inoderTraversal(root);
			flag++;			//紀錄已使用至少一次 
		}

		printf("功能選單:\n[1]新增key值\n[2]刪除key值\n[3]尋找key值\n[4]尋找第n小的key值\n[5]輸出key值由小到大的排序結果\n[6]結束程式\n請輸入選擇: ");
		scanf("%d", &choose);
		printf("\n");

		switch (choose) {
			printf("已選擇[%d], ", choose);
			int key;
			case 1:
				printf("請輸入欲增加的值: ");
				scanf("%d", &key);
				insertNode(root, key);
				inoderTraversal(root);
				break;
			case 2:
				printf("請輸入欲刪除的值: ");
				scanf("%d", &key);
				deleteNode(root, key);
				inoderTraversal(root);
				break;
			case 3:
				printf("請輸入欲尋找的值: ");
				scanf("%d", &key);
				printf("尋找路徑:");
				searchNode(root, key);
				inoderTraversal(root);
				break;
			case 4:
				printf("請輸入欲尋找第幾小的值: ");
				scanf("%d", &key);
				findNthSmallNode(root, key);
				nthSmallCounter = 0;
				inoderTraversal(root);
				break;
			case 5:
				printQuickSort(root);
				break;
			case 6:
				exit(0);
			default:
				printf("無效的選擇，請重新選擇\n");
				break;
		}
		printf("\n");
	}

	return 0;
}

void readKeysFromFile(int* inputKeys, int* lengthOfKeys) {
	char fileName[20];
	int i = 0;
										//獲取檔名，並開啟檔案 
	printf("請指定一延伸檔名為\"txt\"的檔案讀取key值序列(整數且不得重複): ");
	scanf("%s", &fileName);
	FILE* inputFile;
	inputFile = fopen(fileName, "r");

	while (!inputFile) {				//如果沒有該檔案，就再次獲取檔名 			
		printf("無效的檔案名稱，請重新輸入檔案名: ");
		scanf("%s", &fileName);
		inputFile = fopen(fileName, "r");
	}
										//讀入檔案中的所有key值 
	while (fscanf(inputFile, "%d ", &inputKeys[i]) != EOF) {
		i++;
	}

	*lengthOfKeys = i;					//存讀入key值的數量 				

	fclose(inputFile);
}

treePointer buildRoot(int data) {
	treePointer currentRoot;
	currentRoot = (treePointer)malloc(sizeof(struct node));	//申請一個node的記憶體 
	currentRoot->data = data;					//設定root包含的值 
	currentRoot->leftChild = NULL;				//樹根沒有左子樹 
	currentRoot->rightChild = NULL;				//樹根沒有右子樹 
	return currentRoot;
}

treePointer insertNode(treePointer root, int data) {	//找到該node的預計位置 
	if (isExistNode(root, data)) {				//要insert的值已經存在 
		printf("%d重複，無法新增\n", data);
		return root;
	}

	treePointer current = root, previous = NULL;

	while (current) {
		previous = current;
		if (data > current->data) {				//如果數值比當前的root大，應該找右子樹
			current = current->rightChild;
		}
		else if (data < current->data) {		//如果數值比當前的root小， 應該找左子樹 
			current = current->leftChild;
		}
	}
	//找到預計位置的樹根 
	if (!previous) {							//如果一開始就是空子樹，建立初始樹根 
		previous = buildRoot(data);
	}
	else {
		if (data > previous->data) {			//如果該值大於子樹根值，建立右子樹 
			previous->rightChild = buildRoot(data);
		}
		else {									//如果該值小於子樹根植，建立左子樹 
			previous->leftChild = buildRoot(data);
		}
	}

	printf("已新增%d\n", data);
	return previous;							//建立完畢						
}

treePointer deleteNode(treePointer root, int data) {
	if (!root) {								//檢查是否為空的BST 
		printf("空的二元搜尋樹，%d無法刪除\n", data);
		return root;
	}

	treePointer currentRoot = root, previous = NULL;		//currentRoot = 要被刪掉的node，previous = 要被刪掉的node的parent 

	while (currentRoot && currentRoot->data != data) {
		previous = currentRoot;
		if (data > currentRoot->data) {
			currentRoot = currentRoot->rightChild;
		}
		else {
			currentRoot = currentRoot->leftChild;
		}
	}
											//尋找完畢，目前位在預計刪除的node的位置 
	if (!currentRoot) {						//尋找過後如果找不到 = 該值不存在於BST中 
		printf("未找到%d\n", data);
		return root;
	}

	if (!currentRoot->leftChild || !currentRoot->rightChild) {	//被刪除的node只有一個子樹 
		treePointer replace;

		if (!currentRoot->leftChild) {		//該node只有右子樹 
			replace = currentRoot->rightChild;
		}
		else {								//該node只有左子樹 
			replace = currentRoot->leftChild;
		}

		if (!previous) {						//刪除的node位置是root，直接拿root的子樹當成新的BST 
			return replace;
		}

		if (currentRoot == previous->leftChild) {	//看被刪除的node位在其parent的左/右子樹，並把新的子樹接上 
			previous->leftChild = replace;
		}
		else {
			previous->rightChild = replace;
		}
		free(currentRoot);						//刪除該node 
	}
	else {										//被刪除的node有兩個子樹 
		treePointer checker = NULL, successor = currentRoot->rightChild;	//尋找successor的位置，先進入右子樹的範圍 
															
		while (successor->leftChild) {			//在右子樹中找到最小的node 
			checker = successor;
			successor = successor->leftChild;
		}
												//checker = 被刪除node非root情況下，successor的parent 
		if (checker) {
			checker->leftChild = successor->rightChild;		//非root被刪除，successor補上之後， 接下來的子樹接到checker左子樹 
		}
		else {									//是root被刪除，successor補上之後，接下來的子樹接到被刪除node的右子樹 
			currentRoot->rightChild = successor->rightChild;
		}

		currentRoot->data = successor->data;	//把被刪除的node值改成successor的值 
		free(successor);
	}
	return root;
}

treePointer searchNode(treePointer currentRoot, int key) {
	while (currentRoot) {					//如果不為空子樹，開始搜尋 
		if (key == currentRoot->data) {		//遞迴終止條件: 如果找到該值，return當前樹根
			printf(" %d, 已找到%d\n", currentRoot->data, currentRoot->data);
			return currentRoot;
		}
		else if (key < currentRoot->data) {	//如果該值小於當前子樹，往左子樹找 
			printf(" %d ->", currentRoot->data);
			currentRoot = currentRoot->leftChild;
		}
		else {								//如果該值大於當前子樹，往右子樹找 
			printf(" %d ->", currentRoot->data);
			currentRoot = currentRoot->rightChild;
		}
	}
	printf(" end, 未找到%d\n", key);
	return NULL;							//如果為空子樹，或是找不到，則return NULL 
}

treePointer isExistNode(treePointer currentRoot, int key) {
	while (currentRoot) {					//如果不為空子樹，開始搜尋 
		if (key == currentRoot->data) {		//遞迴終止條件: 如果找到該值，return當前樹根
			return currentRoot;
		}
		else if (key < currentRoot->data) {	//如果該值小於當前子樹，往左子樹找 
			currentRoot = currentRoot->leftChild;
		}
		else {								//如果該值大於當前子樹，往右子樹找 
			currentRoot = currentRoot->rightChild;
		}
	}
	return NULL;							//如果為空子樹，或是找不到，則return NULL 
}

treePointer findNthSmallNode(treePointer root, int nthNumber) {
	if (!root) {			//檢查是否為空BST 
		return root;
	}
	//尋找左子樹是否存在第n小值 
	treePointer left = findNthSmallNode(root->leftChild, nthNumber);

	if (left) {			//在左子樹找到第n小值，回傳它 
		return left;
	}

	nthSmallCounter++;	//左子樹找到第nthSmallCounter++小值
	if (nthSmallCounter == nthNumber) {	//找到第n個最小值
		printf("已找到第%d小的值%d\n", nthNumber, root->data);
		return root;
	}
	//往右子樹找(因為那個是未搜尋nodes中最小值)
	return findNthSmallNode(root->rightChild, nthNumber);
}

void printQuickSort(treePointer root) {
	int allNodes[200];
	int lengthOfKeys = 0, i;

	getAllNodes(root, allNodes, &lengthOfKeys);
	quickSort(allNodes, 0, lengthOfKeys - 1);

	printf("Key值的排序結果(由小至大):");
	for (i = 0; i < lengthOfKeys; i++) {
		if (i == lengthOfKeys - 1) {
			printf(" %d", allNodes[i]);
		}
		else {
			printf(" %d,", allNodes[i]);
		}
	}
	printf("\n");
}

void quickSort(int* allNodes, int left, int right) {
	if (left >= right) {					//如果左邊大於右邊，就跳出function
		return;
	}

	int i = left, j = right, key = allNodes[left], temp;	//key = 基準點 

	while (i < j) {						//左邊找比基準點大，右邊找比基準點小 
		while (allNodes[j] > key && i < j) {
			j--;
		}
		while (allNodes[i] < key && i < j) {
			i++;
		}
		if (i < j) {						//成功找到就互換 
			temp = allNodes[i];
			allNodes[i] = allNodes[j];
			allNodes[j] = temp;
		}
	}
	allNodes[left] = allNodes[i];		//把右邊找到的小值換到最左 
	allNodes[i] = key;					//把基準點換到舊小值位置 

	quickSort(allNodes, left, i - 1);		//處理左邊子序列 
	quickSort(allNodes, i + 1, right);	//處理右邊子序列 
}

void getAllNodes(treePointer root, int* allNodes, int* lengthOfKeys) {
	if (root) {							//用inorder trarveral紀錄各個節點，並算好節點數 
		getAllNodes(root->leftChild, allNodes, lengthOfKeys);
		int length = *lengthOfKeys;
		allNodes[length++] = root->data;
		*lengthOfKeys = length;
		getAllNodes(root->rightChild, allNodes, lengthOfKeys);
	}
	return;
}

void inorder(treePointer root) {
	if (root) {		//檢查是否為空子樹，如果不是，輸出尋訪結果直到為空子樹  
		inorder(root->leftChild);
		printf(" %d ->", root->data);
		inorder(root->rightChild);
	}
	return;			//如果是空子樹，跳出函式
}

void inoderTraversal(treePointer root) {	//使inorder輸出完善(只運行非空BST) 
	if (root) {
		printf("Inoder traversal結果:");
		inorder(root);
		printf(" end\n");
	}
}
