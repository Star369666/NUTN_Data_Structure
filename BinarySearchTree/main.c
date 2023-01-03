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

		if (!flag) {		//�ˬd�O�_���Ĥ@���ϥ�(�O�_�إߦnBST) 
			int inputKeys[100];
			readKeysFromFile(inputKeys, &lengthOfKeys);
			root = insertNode(root, inputKeys[0]);
			for (i = 1; i < lengthOfKeys; i++) {
				insertNode(root, inputKeys[i]);
			}
			printf("\n");
			inoderTraversal(root);
			flag++;			//�����w�ϥΦܤ֤@�� 
		}

		printf("�\����:\n[1]�s�Wkey��\n[2]�R��key��\n[3]�M��key��\n[4]�M���n�p��key��\n[5]��Xkey�ȥѤp��j���Ƨǵ��G\n[6]�����{��\n�п�J���: ");
		scanf("%d", &choose);
		printf("\n");

		switch (choose) {
			printf("�w���[%d], ", choose);
			int key;
			case 1:
				printf("�п�J���W�[����: ");
				scanf("%d", &key);
				insertNode(root, key);
				inoderTraversal(root);
				break;
			case 2:
				printf("�п�J���R������: ");
				scanf("%d", &key);
				deleteNode(root, key);
				inoderTraversal(root);
				break;
			case 3:
				printf("�п�J���M�䪺��: ");
				scanf("%d", &key);
				printf("�M����|:");
				searchNode(root, key);
				inoderTraversal(root);
				break;
			case 4:
				printf("�п�J���M��ĴX�p����: ");
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
				printf("�L�Ī���ܡA�Э��s���\n");
				break;
		}
		printf("\n");
	}

	return 0;
}

void readKeysFromFile(int* inputKeys, int* lengthOfKeys) {
	char fileName[20];
	int i = 0;
										//����ɦW�A�ö}���ɮ� 
	printf("�Ы��w�@�����ɦW��\"txt\"���ɮ�Ū��key�ȧǦC(��ƥB���o����): ");
	scanf("%s", &fileName);
	FILE* inputFile;
	inputFile = fopen(fileName, "r");

	while (!inputFile) {				//�p�G�S�����ɮסA�N�A������ɦW 			
		printf("�L�Ī��ɮצW�١A�Э��s��J�ɮצW: ");
		scanf("%s", &fileName);
		inputFile = fopen(fileName, "r");
	}
										//Ū�J�ɮפ����Ҧ�key�� 
	while (fscanf(inputFile, "%d ", &inputKeys[i]) != EOF) {
		i++;
	}

	*lengthOfKeys = i;					//�sŪ�Jkey�Ȫ��ƶq 				

	fclose(inputFile);
}

treePointer buildRoot(int data) {
	treePointer currentRoot;
	currentRoot = (treePointer)malloc(sizeof(struct node));	//�ӽФ@��node���O���� 
	currentRoot->data = data;					//�]�wroot�]�t���� 
	currentRoot->leftChild = NULL;				//��ڨS�����l�� 
	currentRoot->rightChild = NULL;				//��ڨS���k�l�� 
	return currentRoot;
}

treePointer insertNode(treePointer root, int data) {	//����node���w�p��m 
	if (isExistNode(root, data)) {				//�ninsert���Ȥw�g�s�b 
		printf("%d���ơA�L�k�s�W\n", data);
		return root;
	}

	treePointer current = root, previous = NULL;

	while (current) {
		previous = current;
		if (data > current->data) {				//�p�G�ƭȤ��e��root�j�A���ӧ�k�l��
			current = current->rightChild;
		}
		else if (data < current->data) {		//�p�G�ƭȤ��e��root�p�A ���ӧ䥪�l�� 
			current = current->leftChild;
		}
	}
	//���w�p��m����� 
	if (!previous) {							//�p�G�@�}�l�N�O�Ťl��A�إߪ�l��� 
		previous = buildRoot(data);
	}
	else {
		if (data > previous->data) {			//�p�G�ӭȤj��l��ڭȡA�إߥk�l�� 
			previous->rightChild = buildRoot(data);
		}
		else {									//�p�G�ӭȤp��l��ڴӡA�إߥ��l�� 
			previous->leftChild = buildRoot(data);
		}
	}

	printf("�w�s�W%d\n", data);
	return previous;							//�إߧ���						
}

treePointer deleteNode(treePointer root, int data) {
	if (!root) {								//�ˬd�O�_���Ū�BST 
		printf("�Ū��G���j�M��A%d�L�k�R��\n", data);
		return root;
	}

	treePointer currentRoot = root, previous = NULL;		//currentRoot = �n�Q�R����node�Aprevious = �n�Q�R����node��parent 

	while (currentRoot && currentRoot->data != data) {
		previous = currentRoot;
		if (data > currentRoot->data) {
			currentRoot = currentRoot->rightChild;
		}
		else {
			currentRoot = currentRoot->leftChild;
		}
	}
											//�M�䧹���A�ثe��b�w�p�R����node����m 
	if (!currentRoot) {						//�M��L��p�G�䤣�� = �ӭȤ��s�b��BST�� 
		printf("�����%d\n", data);
		return root;
	}

	if (!currentRoot->leftChild || !currentRoot->rightChild) {	//�Q�R����node�u���@�Ӥl�� 
		treePointer replace;

		if (!currentRoot->leftChild) {		//��node�u���k�l�� 
			replace = currentRoot->rightChild;
		}
		else {								//��node�u�����l�� 
			replace = currentRoot->leftChild;
		}

		if (!previous) {						//�R����node��m�Oroot�A������root���l����s��BST 
			return replace;
		}

		if (currentRoot == previous->leftChild) {	//�ݳQ�R����node��b��parent����/�k�l��A�ç�s���l�𱵤W 
			previous->leftChild = replace;
		}
		else {
			previous->rightChild = replace;
		}
		free(currentRoot);						//�R����node 
	}
	else {										//�Q�R����node����Ӥl�� 
		treePointer checker = NULL, successor = currentRoot->rightChild;	//�M��successor����m�A���i�J�k�l�𪺽d�� 
															
		while (successor->leftChild) {			//�b�k�l�𤤧��̤p��node 
			checker = successor;
			successor = successor->leftChild;
		}
												//checker = �Q�R��node�Droot���p�U�Asuccessor��parent 
		if (checker) {
			checker->leftChild = successor->rightChild;		//�Droot�Q�R���Asuccessor�ɤW����A ���U�Ӫ��l�𱵨�checker���l�� 
		}
		else {									//�Oroot�Q�R���Asuccessor�ɤW����A���U�Ӫ��l�𱵨�Q�R��node���k�l�� 
			currentRoot->rightChild = successor->rightChild;
		}

		currentRoot->data = successor->data;	//��Q�R����node�ȧ令successor���� 
		free(successor);
	}
	return root;
}

treePointer searchNode(treePointer currentRoot, int key) {
	while (currentRoot) {					//�p�G�����Ťl��A�}�l�j�M 
		if (key == currentRoot->data) {		//���j�פ����: �p�G���ӭȡAreturn��e���
			printf(" %d, �w���%d\n", currentRoot->data, currentRoot->data);
			return currentRoot;
		}
		else if (key < currentRoot->data) {	//�p�G�ӭȤp���e�l��A�����l��� 
			printf(" %d ->", currentRoot->data);
			currentRoot = currentRoot->leftChild;
		}
		else {								//�p�G�ӭȤj���e�l��A���k�l��� 
			printf(" %d ->", currentRoot->data);
			currentRoot = currentRoot->rightChild;
		}
	}
	printf(" end, �����%d\n", key);
	return NULL;							//�p�G���Ťl��A�άO�䤣��A�hreturn NULL 
}

treePointer isExistNode(treePointer currentRoot, int key) {
	while (currentRoot) {					//�p�G�����Ťl��A�}�l�j�M 
		if (key == currentRoot->data) {		//���j�פ����: �p�G���ӭȡAreturn��e���
			return currentRoot;
		}
		else if (key < currentRoot->data) {	//�p�G�ӭȤp���e�l��A�����l��� 
			currentRoot = currentRoot->leftChild;
		}
		else {								//�p�G�ӭȤj���e�l��A���k�l��� 
			currentRoot = currentRoot->rightChild;
		}
	}
	return NULL;							//�p�G���Ťl��A�άO�䤣��A�hreturn NULL 
}

treePointer findNthSmallNode(treePointer root, int nthNumber) {
	if (!root) {			//�ˬd�O�_����BST 
		return root;
	}
	//�M�䥪�l��O�_�s�b��n�p�� 
	treePointer left = findNthSmallNode(root->leftChild, nthNumber);

	if (left) {			//�b���l�����n�p�ȡA�^�ǥ� 
		return left;
	}

	nthSmallCounter++;	//���l�����nthSmallCounter++�p��
	if (nthSmallCounter == nthNumber) {	//����n�ӳ̤p��
		printf("�w����%d�p����%d\n", nthNumber, root->data);
		return root;
	}
	//���k�l���(�]�����ӬO���j�Mnodes���̤p��)
	return findNthSmallNode(root->rightChild, nthNumber);
}

void printQuickSort(treePointer root) {
	int allNodes[200];
	int lengthOfKeys = 0, i;

	getAllNodes(root, allNodes, &lengthOfKeys);
	quickSort(allNodes, 0, lengthOfKeys - 1);

	printf("Key�Ȫ��Ƨǵ��G(�Ѥp�ܤj):");
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
	if (left >= right) {					//�p�G����j��k��A�N���Xfunction
		return;
	}

	int i = left, j = right, key = allNodes[left], temp;	//key = ����I 

	while (i < j) {						//���������I�j�A�k�������I�p 
		while (allNodes[j] > key && i < j) {
			j--;
		}
		while (allNodes[i] < key && i < j) {
			i++;
		}
		if (i < j) {						//���\���N���� 
			temp = allNodes[i];
			allNodes[i] = allNodes[j];
			allNodes[j] = temp;
		}
	}
	allNodes[left] = allNodes[i];		//��k���쪺�p�ȴ���̥� 
	allNodes[i] = key;					//�����I�����¤p�Ȧ�m 

	quickSort(allNodes, left, i - 1);		//�B�z����l�ǦC 
	quickSort(allNodes, i + 1, right);	//�B�z�k��l�ǦC 
}

void getAllNodes(treePointer root, int* allNodes, int* lengthOfKeys) {
	if (root) {							//��inorder trarveral�����U�Ӹ`�I�A�ú�n�`�I�� 
		getAllNodes(root->leftChild, allNodes, lengthOfKeys);
		int length = *lengthOfKeys;
		allNodes[length++] = root->data;
		*lengthOfKeys = length;
		getAllNodes(root->rightChild, allNodes, lengthOfKeys);
	}
	return;
}

void inorder(treePointer root) {
	if (root) {		//�ˬd�O�_���Ťl��A�p�G���O�A��X�M�X���G���쬰�Ťl��  
		inorder(root->leftChild);
		printf(" %d ->", root->data);
		inorder(root->rightChild);
	}
	return;			//�p�G�O�Ťl��A���X�禡
}

void inoderTraversal(treePointer root) {	//��inorder��X����(�u�B��D��BST) 
	if (root) {
		printf("Inoder traversal���G:");
		inorder(root);
		printf(" end\n");
	}
}
