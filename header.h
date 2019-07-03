#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

struct SymbolTable {
	char name[8];
	unsigned int add;
	unsigned int size;
};

struct IntermediateLan {
	int ino;
	int opcode;
	int parameters[4];
};

struct BlockTable {
	char name[12];
	unsigned int add;
};

struct Stack {
	int ins_nos[100];
	int top;
};

struct Label {
	int inso;
	char name[100];
};

char ** parse(char cd[],int *na) {
	int i, j, k = 0, sp = 0, t = 0;
	*na = 0;
	char ch;
	char **s = (char**)malloc(sizeof(char*) * 10);
	for (i = 0; cd[i] != '\0'; i++) {
		if (cd[i] == ' ' || cd[i]==',') {
			s[t++] = (char*)malloc(sizeof(char) * 20);
		}
	}
	s[t] = (char*)malloc(sizeof(char) * 20);
	k = 0; t = -1;
	for (i = 0; cd[i]; i++) {
		if (cd[i] == ' ' || cd[i]==',') {
			s[k][++t] = '\0';
			k += 1;
			t = -1;
			if (cd[i] == ',' && cd[i+1] == ' ') i += 1;
			if (cd[i] == ' ' && cd[i+1] == ',') i += 1;
			continue;
		}
		s[k][++t] = cd[i];
	}
	s[k][++t] = '\0';

	*na = k + 1;

	return s;
}

unsigned to_num(char *num) {
	unsigned int dec = 0;
	int len = strlen(num), i;
	for (i = 0; i<len; i++){
		dec = dec * 10 + (num[i] - '0');
	}
	return dec;
}

char * get_variable_name(char *s) {
	int flag, i, t = -1;
	char *vn = (char*)malloc(sizeof(char) * 10);
	for (i = 0; s[i] != '\0'; i++) {
		if (s[i] == '[') {
			vn[++t] = '\0';
			return vn;
		}
		vn[++t] = s[i];
	}
	vn[++t] = '\0';
	return vn;
}

char * get_block_name(char *s) {
	int flag, i, t = -1;
	char *vn = (char*)malloc(sizeof(char) * 10);
	for (i = 0; i<strlen(s)-1; i++) {
		vn[++t] = s[i];
	}
	vn[++t] = '\0';
	return vn;
}

int check(struct BlockTable bt[], int n, char s[]) {
	for (int i = 0; i <= n; i++) {
		if (strcmp(bt[i].name, s) == 0) {
			return i;
		}
	}
	return -1;
}


int get_sym_pos(char s[],struct SymbolTable *st,int n) {
	for (int i = 0; i <= n; i++) {
		if (strcmp(s, st[i].name) == 0) {
			return i;
		}
	}
	return -1;
}

int get_size(char *s) {
	int num = 0, i, flag = 0;
	for (i = 0; s[i] != '\0'; i++) {
		if (s[i] == '[') { flag = 1; continue; }
		if (s[i] == ']') return num;
		if(flag==1) num = (num * 10) + (s[i] - '0');
	}
	return 1;
}

int compute(int x, int y, char ch) {
	if (ch == '+') return x + y;
	if (ch == '-') return x - y;
	if (ch == '*') return x * y;
}

int get_conditional_operator_code(char s[]) {
	if (strcmp(s, "EQ") == 0) return 8;
	if (strcmp(s, "LT") == 0) return 9;
	if (strcmp(s, "GT") == 0) return 10;
	if (strcmp(s, "LTEQ") == 0) return 11;
	if (strcmp(s, "GTEQ") == 0) return 12;
	return -1;
}

int get_register_code(char s[]) {
	if (strcmp(s, "AX") == 0) return 0;
	if (strcmp(s, "BX") == 0) return 1;
	if (strcmp(s, "CX") == 0) return 2;
	if (strcmp(s, "DX") == 0) return 3;
	if (strcmp(s, "EX") == 0) return 4;
	if (strcmp(s, "FX") == 0) return 5;
	if (strcmp(s, "GX") == 0) return 6;
	if (strcmp(s, "HX") == 0) return 7;
	return -1;
}

int get_opcode(char s[]) {
	if (strcmp("MOV", s) == 0) return 1;
	if (strcmp("MOV", s) == 0) return 2;
	if (strcmp("ADD", s) == 0) return 3;
	if (strcmp("SUB", s) == 0) return 4;
	if (strcmp("MUL", s) == 0) return 5;
	if (strcmp("JMP", s) == 0) return 6;
	if (strcmp("IF", s) == 0) return 7;
	if (strcmp("EQ", s) == 0) return 8;
	if (strcmp("LT", s) == 0) return 9;
	if (strcmp("GT", s) == 0) return 10;
	if (strcmp("LTEQ", s) == 0) return 11;
	if (strcmp("GTEQ", s) == 0) return 12;
	if (strcmp("PRINT", s) == 0) return 13;
	if (strcmp("READ", s) == 0) return 14;
	return -1;
}

void ADD(struct IntermediateLan IL[],int no_of_ins,char **cd) {
	IL[no_of_ins].ino = no_of_ins + 1;
	IL[no_of_ins].opcode = 3;
	IL[no_of_ins].parameters[0] = get_register_code(cd[1]);
	IL[no_of_ins].parameters[1] = get_register_code(cd[2]);
	IL[no_of_ins].parameters[2] = get_register_code(cd[3]);
}

void SUB(struct IntermediateLan IL[], int no_of_ins, char **cd) {
	IL[no_of_ins].ino = no_of_ins + 1;
	IL[no_of_ins].opcode = 4;
	IL[no_of_ins].parameters[0] = get_register_code(cd[1]);
	IL[no_of_ins].parameters[1] = get_register_code(cd[2]);
	IL[no_of_ins].parameters[2] = get_register_code(cd[3]);
}

void MUL(struct IntermediateLan IL[], int no_of_ins, char **cd) {
	IL[no_of_ins].ino = no_of_ins + 1;
	IL[no_of_ins].opcode = 5;
	IL[no_of_ins].parameters[0] = get_register_code(cd[1]);
	IL[no_of_ins].parameters[1] = get_register_code(cd[2]);
	IL[no_of_ins].parameters[2] = get_register_code(cd[3]);
}

void PRINT(struct IntermediateLan IL[], int no_of_ins, char **cd,struct SymbolTable st[],int no_of_symbols) {
	char *temp = get_variable_name(cd[1]);
	IL[no_of_ins].ino = no_of_ins + 1;
	IL[no_of_ins].opcode = 13;
	IL[no_of_ins].parameters[0] = get_sym_pos(temp, st, no_of_symbols);
}

void MOVE(struct IntermediateLan IL[], int no_of_ins, char **cd, struct SymbolTable st[], int no_of_symbols) {
	int f = get_register_code(cd[1]);
	char *temp;
	if (f == -1) {
		temp = get_variable_name(cd[1]);
		IL[no_of_ins].ino = no_of_ins + 1;
		IL[no_of_ins].opcode = 1;
		IL[no_of_ins].parameters[0] = get_sym_pos(temp, st, no_of_symbols);
		IL[no_of_ins].parameters[1] = get_register_code(cd[2]);
	}
	else {
		temp = get_variable_name(cd[2]);
		IL[no_of_ins].ino = no_of_ins + 1;
		IL[no_of_ins].opcode = 2;
		IL[no_of_ins].parameters[0] = get_register_code(cd[1]);
		IL[no_of_ins].parameters[1] = get_sym_pos(temp, st, no_of_symbols);
	}
}

void READ(struct IntermediateLan IL[], int no_of_ins, char **cd) {
	IL[no_of_ins].ino = no_of_ins + 1;
	IL[no_of_ins].opcode = get_opcode(cd[0]);
	IL[no_of_ins].parameters[0] = get_register_code(cd[1]);
}
