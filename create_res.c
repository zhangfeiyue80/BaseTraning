#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define BEGIN 0
#define MAXLEN 5+1 //每题最大可选项数,generate_per select 参数满足:select <= MAXLEN


// 堆结构相关定义,操作
typedef struct heap_node NODE;
struct heap_node{
	char s[MAXLEN];
	NODE *lchild;
	NODE *rsib;
};

NODE *heap_create() {
	NODE *pnode;
	pnode = (NODE *)malloc(sizeof(NODE));
	memset(pnode->s, 0, MAXLEN);
	pnode->rsib = NULL;
	pnode->lchild = NULL;
	return pnode;
}

int heap_delete(NODE *header) {
	NODE *p;
	p = header;
	if(p->rsib != NULL) {
		heap_delete(p->rsib);
	}
	else{
		if(p->lchild != NULL) {
			heap_delete(p->lchild);
		}
	}
	if(p->s[0] != 0) 
		free(p);
	else
		p->lchild = NULL;
	return 0;
}

NODE *heap_chiladd(NODE *pnode, char *s) {
	NODE *p;
	p = (NODE *)malloc(sizeof(NODE));
	strcpy(p->s, s);
	p->lchild = NULL;
	p->rsib = NULL;
	pnode->lchild = p;
	return p;
}

NODE *heap_sibadd(NODE *pnode, char *s) {
	NODE *p;
	p = (NODE *)malloc(sizeof(NODE));
	strcpy(p->s, s);
	p->lchild = NULL;
	p->rsib = NULL;
	pnode->rsib = p;
	return p;
}

// 链表相关定义,操作
typedef struct linker LINK;
struct linker{
	char s[MAXLEN];
	LINK *next;
};

LINK * link_create() {
	LINK *plink;
	plink = (LINK *)malloc(sizeof(LINK));
	memset(plink->s, 0, MAXLEN);
	plink->next = NULL;
	return plink;
}

LINK * link_add(LINK *plink, char *s) {
	LINK *p;
	p = (LINK *)malloc(sizeof(LINK));
	strcpy(p->s,s);
	p->next = NULL;
	plink->next = p;
	return p;
}

LINK * link_delete(LINK *plink) {
	LINK *p;
	free(plink->next);
	plink->next = NULL;
	return plink;
}

int link_free(LINK *header) {
	LINK *p;
	p = header;
	if(p->next != NULL) {
		link_delete(p->next);
	}
	if(p->s[0] != 0) 
		free(p);
	else
		p->next = NULL;
	return 0;
}

typedef struct link_linker LINK_LINK;
struct link_linker{
	LINK_LINK *sheader;
	LINK_LINK *next;
};

/* n个可选项中选select个，产生所有可能结果存到堆Node中 */
/*
int generate_tree(int sum, int select, NODE *Node) {
	static int order = 0;
	char s[MAXLEN];
	int i,j;
	memset(s, 0, MAXLEN);
	for(i = order; i < sum-select+1; i++) {
		s[0] = i+65;
		if(i == order) {
			Node = heap_chiladd(Node, s);
		}
		else {
			Node = heap_sibadd(Node, s);
		}
		select--;
		if(select > 0) {	
			order = i+1;
			generate_tree(sum, select, Node);
			order--;
		}
		select++;
		
	}
	return 0;
}
*/

int generate_tree_sub(int sum, int select, int order,  NODE *Node) {
	char s[MAXLEN];
	int i,j;
	memset(s, 0, MAXLEN);
	for(i = order; i < sum-select+1; i++) {
		s[0] = i+65;
		if(i == order) {
			Node = heap_chiladd(Node, s);
		}
		else {
			Node = heap_sibadd(Node, s);
		}
		select--;
		if(select > 0) {	
			order = i+1;
			generate_tree_sub(sum, select, order, Node);
			order--;
		}
		select++;
		
	}
	return 0;
}

int generate_tree(int sum, int select, NODE *Node) {
	generate_tree_sub(sum, select, 0,  Node);
	return 0;
}

LINK * traver_tree(NODE *node, LINK *plink) {
	static char per_result[MAXLEN];
	static int i = 0;
	NODE *p = node;
	if(strlen(p->s) != 0) strcpy(per_result+i++, p->s);
	if (p->lchild != NULL) {
		plink = traver_tree(p->lchild, plink);
	}
	else {
		plink = link_add(plink, per_result);
	}

	if(p->rsib != NULL) {
		i--;
		memset(per_result+i, 0, MAXLEN-i);
		plink = traver_tree(p->rsib, plink);
	}
	else{
		if(i > 0) {
			i -= 1;
			memset(per_result+i, 0, MAXLEN-i);
		}
	}

	return plink;
}



LINK *generate_per_sub(int sum, int select) {
	NODE *root;
	LINK *header,*p;
	header = link_create();
	p = header;
	root = heap_create();

	generate_tree(sum, select, root);
	p = traver_tree(root, p);
	heap_delete(root);

	return header;
}

LINK *generate_per(int sum) {
	int i;
	NODE *root;
	LINK *header,*p;
	header = link_create();
	p = header;
	root = heap_create();

	for(i=1; i<sum+1; i++){
		generate_tree(sum, i, root);
		p = traver_tree(root, p);
		heap_delete(root);
	}

	return header;
}


int traver_linker(LINK *header) {
	LINK *p;
	for(p = header->next; p != NULL; p = p->next)
		printf("%s\n", p->s);
	return 0;
}

/*
int traver_linker_list(LINK *header) {
	traver_linker_list_sub(header, 0);
	return 0;
}
*/

/*
int traver_linker_list_sub(LINK *header, int i) {
	LINK *p;
	p = header[i];
	i++;
	for(p = p->next; p != NULL; p = p->next) {
		printf("%d.%s ", i+1, p->s);
		if(header[i] != NULL)
		traver_linker_list_sub(header, i);
	}
	return 0;
}
*/

/*
int main(int argc, char *argv[]) {
	int sum;
	LINK *header;
	if(argc>3)
		printf("参数错误:\n	usage: %s arg1 [arg2]", argv[0]);
	else if(argc == 2) {
		if(strcmp(argv[1], "-h") == 0) {
			printf("usage: %s arg1 [arg2]\n", argv[0]);
			printf("description: 穷举出选择题所有结果.\n");
			printf("	arg1: 选择题可选项数.\n");
			printf("	arg2: 选择题选出项数.\n");
		}
		else {
			sum = atoi(argv[1]);
			header = generate_per(sum);
		}
	}
	else {
		sum = atoi(argv[1]);
		header = generate_per_sub(sum, atoi(argv[2]));
	}
	//header = generate_per(4, 1);
	traver_linker(header);
	return 0;
}
*/

LINK **generate_array(int argc, char *argv[]){
	int sum,select,num,i;
	char *s;
	LINK *sheader,**header;
	num = argc-1;
	header = (LINK **)malloc( sizeof(LINK *)*(num+1) );
	memset(header, 0, sizeof(LINK *)*(num+1));
	for(i=num; i>0; i--) { 
		if(strstr(argv[argc-i], ",") == NULL) {
			sum = atoi(argv[argc-i]);
			sheader = generate_per(sum);
		}
		else {
			s = strstr(argv[argc-i], ",");
			s[0] = 0;
			sum = atoi(argv[argc-i]);
			select = atoi(s+1);
			sheader = generate_per_sub(sum, select);
		}
		header[num-i] = sheader;
	}
	header[num-i] = NULL;
	return header;
}

int link_generate_tree_sub(LINK **header, NODE *node, int i) {
	char s[MAXLEN];
	LINK *p;
	p = header[i];
	//int i,j;
	memset(s, 0, MAXLEN);
	for(p = p->next; p != NULL; p = p->next) {
		if( header[i]->next == p ) {
			strcpy(s, p->s);
			node = heap_chiladd(node, s);
		}
		else {
			strcpy(s, p->s);
			node = heap_sibadd(node, s);
		}
		i++;
		if(header[i] != NULL) {	
			link_generate_tree_sub(header, node, i);
		}
		i--;
	}
	return 0;
}

int link_generate_tree(LINK **header, NODE *node) {
	link_generate_tree_sub(header, node, 0);
	return 0;
}

int traver_tree_linker(LINK *header) {
	LINK *p;
	int i=1;
	for(p = header->next; p != NULL; p = p->next) {
		printf("%d.%s\t", i, p->s);
		i++;
	}
	printf("\n");
	return 0;
}

int traver_link_tree_sub(NODE *node, LINK *header, LINK *plink) {
	/*
	static char per_result[MAXLEN];
	static int i = 0;
	*/
	NODE *p = node;
	LINK *plinker = plink;
	if(strlen(p->s) != 0) plinker = link_add(plink, p->s);
	if (p->lchild != NULL) {
		traver_link_tree_sub(p->lchild, header, plinker);
	}
	else {
		traver_tree_linker(header);
	}

	if(p->rsib != NULL) {
		link_delete(plink);
		traver_link_tree_sub(p->rsib, header,  plink);
	}

	return 0;
}

int traver_link_tree(NODE *node) {
	LINK *header;
	header = link_create();
	traver_link_tree_sub(node, header, header);
	return 0;
}

int main(int argc, char *argv[]) {
	LINK **header;
	NODE *root;
	header = generate_array(argc, argv);
	//traver_linker_list(header);
	root = heap_create();
	link_generate_tree(header, root);
	traver_link_tree(root);
	return 0;
}
