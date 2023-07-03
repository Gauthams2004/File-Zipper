#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>

char filename[50];

char* data;
int len;
char (*codes)[40];
char* characters;
int* freq;
int diff = 0;

char* final;
char* encodedTree;

int extraTree = 0;
int extraData = 0;

char tempCode[100];
int idx = 0;

struct Node{
    char data;
    unsigned freq;
    struct Node* left;
    struct Node* right;
};

struct MinHeap{
    unsigned size;
    unsigned cap;
    struct Node** arr;
};

typedef struct Node*Node;
typedef struct MinHeap*MinHeap;

Node root;

Node createNode(Node left,Node right,int freq, char data)
{
    Node N = (Node)malloc(sizeof(struct Node));
    N->data = data;
    N->freq = freq;
    N->left = left;
    N->right = right;
    return N;
}


void swapNodes(Node* a, Node* b){
    Node temp = *a;
    *a = *b;
    *b = temp;
}

void Heapify(MinHeap H, int i){
    int p = i;
    int left = 2*i + 1;
    int right = 2*i + 2;
    
    if(left < H->size && H->arr[left]->freq < H->arr[p]->freq){
        p = left;
    }
    
    if(right < H->size && H->arr[right]->freq < H->arr[p]->freq){
        p = right;
    }
    
    if(p!=i){
        swapNodes(&H->arr[p],&H->arr[i]);
        Heapify(H,p);
    }
}

Node extractMin(MinHeap H){
    Node temp = H->arr[0];
    H->arr[0] = H->arr[H->size - 1];
    H->size--;
    Heapify(H,0);
    return temp;
}

void insertNode(MinHeap H, Node N){
    H->size++;
    int i = H->size - 1;
    
    while(i && N->freq < H->arr[(i - 1)/2]->freq){
        H->arr[i] =  H->arr[(i - 1)/2];
        i = (i - 1)/2;
    }
    
    H->arr[i] = N;
}

MinHeap createHeap(){
    MinHeap H = (MinHeap)malloc(sizeof(struct MinHeap));
    H->size = 0;
    H->arr = (Node*)malloc(diff * sizeof(Node));
    for(int i=0;i<diff;i++){
        Node N = createNode(NULL,NULL,freq[i],characters[i]);
        insertNode(H,N);
    }
    return H;
}

bool isLeaf(Node N){
    if(!N->left && !N->right){
        return true;
    }
    return false;
}

void createCode(Node root){
    if(root->left){
        tempCode[idx++] = '0';
        createCode(root->left);
        idx--;
    }
    if(root->right){
        tempCode[idx++] = '1';
        createCode(root->right);
        idx--;
    }
    if(isLeaf){
        int i = 0;
        for(int j=0;j<diff;j++){
            if(characters[j] == root->data){
                i = j;
                break;
            }
        }
        
        for(int j=0;j<idx;j++){
            codes[i][j] = tempCode[j];
        }
        
        codes[i][idx] = '\0';
    }
}

bool size_one(MinHeap H){
    if(H->size == 1) return true;
    return false;
}

void buildHuffmanTree(){
    Node left, right, top;
    MinHeap H = createHeap();
    
    while(!size_one(H)){
        left = extractMin(H);
        right = extractMin(H);
        
        top = createNode(NULL,NULL,left->freq + right->freq,'#');
        top->left = left;
        top->right = right;
        insertNode(H,top);
    }
    
    root = extractMin(H);
    idx = 0;
    createCode(root);
    
    for(int i=0;i<diff;i++){
        for(int j=0;codes[i][j];j++){
            if(codes[i][j] == '0' || codes[i][j] == '1'){
                continue;
            }
            else{
                codes[i][j] = '\0';
                break;
            }
        }
    }
    
}

int helper;

char tree(int curr){
    Node temp = root;
    while(temp->left && temp->right){
        if(final[curr] == '1'){
            temp = temp->right;
        }
        else{
            temp = temp->left;
        }
        
        (curr)++;
    }
    helper = curr;
    printf("JUst CHECHKING\n");
    
    return temp->data;
}

Node DecodeTree(){
    if(idx >= strlen(encodedTree)){
        return NULL;
    }
    
    if(encodedTree[idx] == '1'){
        idx++;
        int val = 0;
        for(int i=1;i<=8 && idx < strlen(encodedTree); i++, idx++){
            if(encodedTree[idx] == '1'){
                val += (1 << (8 - i));
            }
        }
        return createNode(NULL,NULL,0,(char)val);
    }
    else{
        idx++;
        Node left = DecodeTree();
        Node right = DecodeTree();
        return createNode(left,right,0,'#');
    }
}

int createTree_helper(){
    
    printf("Enter the filename (with extension) where the data to be decoded is stored: ");
    scanf("%s",filename);
    FILE *FIN = fopen(filename,"r");
    if(FIN == NULL) return 0;
    
    char dataInText[1000];
    encodedTree = (char*)malloc(15000*sizeof(char));
    strcpy(encodedTree,"");
    
    char c;
    idx = 0;
    int sz = 0;
    for(int i=0,power = 1000; i<4;i++, power /= 10){
        sz += (fgetc(FIN) - '0')*(power);
    }
    
    int i=8;
    
    while(idx < sz){
        fseek(FIN,i,SEEK_SET);
        c = fgetc(FIN);
        if(c == -1){
            c = 26;
        }
        dataInText[idx] = c;
        idx++;
        i++;
        if(c == '\n') i++;
    }
    
    dataInText[idx] = '\0';
    
    for(int i=0;i<sz;i++){
        char bin[8];
        int d = dataInText[i];
        for(int j=0;j<7;j++){
            bin[6-j] = (d % 2) + '0';
            d /= 2;
        }
        bin[7] = '\0';
        strcat(encodedTree,bin);
    }
    
    fseek(FIN,-2,SEEK_END);
    extraTree = fgetc(FIN) - '0';
    encodedTree[strlen(encodedTree) - extraTree] = '\0';
    
    fclose(FIN);
    idx = 0;
    
    root = DecodeTree();
    printf("Just Checking.\n");
    
    return 1;
    
}

int decodeData(){
    
    FILE *FIN = fopen(filename,"r");
    if(FIN == NULL) return 0;
    FILE* FOP = fopen("decompressed.txt","w");
    final = (char*)malloc(15000*sizeof(char));
    strcpy(final,"");
    char c;
    
    int sz1=0, sz2=0;
    for(int i=0,power = 1000; i<4;i++, power /= 10){
        sz1 += (fgetc(FIN) - '0')*(power);
    }
    
    for(int i=0,power = 1000; i<4;i++, power /= 10){
        sz2 += (fgetc(FIN) - '0')*(power);
    }
    
    int numberOfNewLines = 0;
    
    int i=8, j=0;
    
    idx = 0;
    
    while(idx < sz1){
        fseek(FIN,i,SEEK_SET);
        c = fgetc(FIN);
        idx++; i++;
        if(c == '\n'){
            i++;
            numberOfNewLines++;
        }
    }
    
    i = 8 + sz1 + numberOfNewLines;
    
    while(j < sz2){
        fseek(FIN,i,SEEK_SET);
        c= fgetc(FIN);
        if(c==-1) c= 26;
        char bin[8];
        int d = c;
        
        for(int j=0;j<7;j++){
            bin[6-j] = (d % 2) + '0';
            d /= 2;
        }
        bin[7] = '\0';
        strcat(final,bin);
        i++;j++;
        if(c=='\n')i++;
        
    }
    
    fseek(FIN,-1,SEEK_END);
    int extraData = fgetc(FIN) - '0';
    final[strlen(final) - extraData] = '\0';
    printf("Just Checking.\n");
    for(int i=0;i<strlen(final);){
        c = tree(i);
        fprintf(FOP,"%c",c);
        i = helper;
    }
    printf("Just Checking.\n");
    fclose(FOP);
    fclose(FIN);
    return 1;
    
}

void EncodedTree(Node N){
    if(isLeaf(N)){
        strcat(encodedTree,"1");
        int val = N->data;
        char bin[9] = "000000000";
        int curr = 7;
        while(val > 0){
            bin[curr] = (char)((val%2) + '0');
            val /= 2;
            curr--;
        }
        strcat(encodedTree,bin);
    }
    else{
        strcat(encodedTree,"0");
        EncodedTree(N->left);
        EncodedTree(N->right);
    }
}

void createTree(){
    encodedTree = (char*)malloc(15000*sizeof(char));
    strcpy(encodedTree,"");
    EncodedTree(root);
    char dataInText[15000];
    int sz = 0;
    for(int i=0;i<strlen(encodedTree);){
        char bin[8];
        int curr = 0;
        /////////////////////////////////
        if(strlen(encodedTree) - i < 7){
             extraTree = 7 - (strlen(encodedTree) - i);
        }
        for(int cnt = 6;cnt>=0;cnt--,i++){
            curr += (encodedTree[i] == 1)*(1LL << (cnt));
        }
        dataInText[sz] = (char)curr;
        sz++;
        dataInText[sz] = '\0';
        FILE* FOP = fopen("compressed.txt","w");
        fprintf(FOP,"%04d%04d",sz,idx);
        for(int i = 0;i<sz;i++){
            fprintf(FOP,"%c",dataInText[i]);
        }
        
        for(int i=0;i<idx;i++){
            fprintf(FOP,"%c",final[i]);
        }
        
        fprintf(FOP,"%d%d",extraTree,extraData);
        fclose(FOP);
    }
}

void encodedData(){
    FILE* FIN = fopen(filename,"r");
    char c, dataInBin[15000];
    final = (char*)malloc(sizeof(char)*15000);
    strcpy(final,"");
    while((c=fgetc(FIN)) != EOF){
        for(int i=0;i,diff;i++){
            if(characters[i] == c){
                strcat(dataInBin,codes[i]);
                break;
            }
        }
    }
    idx = 0;
    for(int i=0;i<strlen(dataInBin);){
        int curr = 0;
        if(strlen(dataInBin) - i < 7){
            extraData = 7 - (strlen(dataInBin) - i);
        }
        for(int cnt = 6;cnt>=0 && i<strlen(dataInBin);cnt--,i++){
            curr += ((1<<cnt))*(dataInBin[i] == '1');
        }
        final[idx] = (char)curr;
        idx++;
    }
    final[idx] = '\0';
    fclose(FIN);
}

int scanData(){
    printf("Enter Filename: ");
    scanf("%s",filename);
    
    FILE* fp;
    fp = fopen(filename,"r");
    
    if(fp == NULL){
        printf("There is no file with the given name.\n");
        return 0;
    }
    else{
        printf("File opened successfully :)\n");
    }
    
    fseek(fp,0,SEEK_END);
    len = ftell(fp);
    
    data = (char*)malloc(sizeof(char) * (len + 5));
    char c;
    int curIdx = 0;
    fseek(fp,0,SEEK_SET);
    while((c=fgetc(fp)) != EOF){
        data[curIdx++] = c;
    }
    
    data[curIdx] = '\0';
    return 1;
}

void createFreqArray(){
    int cnt[128] = {};
    for(int i=0;i<len && data[i];i++){
        cnt[data[i]]++;
    }
    for(int i=0;i<128;i++){
        if(cnt[i] >=1){
            ++diff;
        }
    }
    
    characters = (char*)malloc(sizeof(char)*diff);
    freq = (int*)malloc(diff*sizeof(int));
    codes = malloc(sizeof(char[diff][40]));
    for(int i=0,j=0;i<128;i++){
        if(cnt[i]>=1){
            characters[j] = (char)(i);
            freq[j++] = cnt[i];
        }
    }
    
    for(int i=0;i<diff - 1;i++){
        for(int j=0;j<diff - i - 1;j++){
            if(freq[j] > freq[j + 1]){
                int t1 = freq[j];
                freq[j] = freq[j + 1];
                freq[j + 1] = t1;
                char t2 = characters[j];
                characters[j] = characters[j + 1];
                characters[j + 1] = t2;
            }
        }
    }
}


int main(){
    char choice;
    printf("Would you like to compress or decompress your file?\n");
    printf("Enter 'C' to compress file, or 'D' to decompress: ");
    while(1){
        scanf("%c%*c",&choice);
        if(choice == 'C' || choice == 'c' || choice == 'D' || choice == 'd'){
            break;
        }
        else{
            printf("Enter proper choice: ");
        }
    }
    
    if(choice == 'C' || choice == 'c'){
        int isFileOpened = scanData();
        if(isFileOpened){
            createFreqArray();
            buildHuffmanTree();
            encodedData();
            createTree();
            printf("Data compressed successfully :)\n");
        }
    }
    else{
        int isOk = createTree_helper();
        if(isOk){
            isOk = decodeData();
        }
        if(isOk){
            printf("Data decompressed successfully :)\n");
        }
        else{
            printf("Unexpected error, couldn't decompress file.\n");
        }
    }
    
    printf("\n\n------------THANK YOU------------\n\n");
    return 0;
}






















