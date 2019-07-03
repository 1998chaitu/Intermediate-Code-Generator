#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<conio.h>
#include"header.h"


int main() {
	char **cd;
	char s[30],*temp;
	Stack stk; stk.top = -1;
	int x;
	int val;
	int no_of_ins=-1;
	int no_of_symbols=-1;
	int no_of_blocks = -1;
	int start_add = 0;
	int memory[100];
	int ms = -1;
	struct SymbolTable st[50];
	struct IntermediateLan IL[50];
	struct BlockTable bt[50];
	struct Label lab[30]; int no_of_labels = -1;
	int curr_add = 0, ins = 0;
	FILE *flp = fopen("source.txt", "r");

	while (fscanf(flp, " %[^\n]s", s) == 1) {
		cd = parse(s, &x);

		if ((strcmp(cd[0], "DATA") == 0)) {
			no_of_symbols += 1;
			int size = get_size(cd[1]);
			char *temp = get_variable_name(cd[1]);
			strcpy(st[no_of_symbols].name, temp);
			st[no_of_symbols].add = start_add;
			start_add += size;
			st[no_of_symbols].size = size;
			continue;
		}

		if (strcmp(cd[0], "START:") == 0) {
			ins = 1;
			continue;
		}

		if (strcmp(cd[0], "READ") == 0) {
			no_of_ins += 1;
			READ(IL,no_of_ins,cd);
			continue;
		}

		if (strcmp(cd[0], "CONST")==0) {
			strcpy(st[++no_of_symbols].name, cd[1]);
			st[no_of_symbols].size = 0;
			st[no_of_symbols].add = ++ms;
			memory[ms] = to_num(cd[3]);
		}

		if ((strcmp(cd[0], "MOV") == 0)) {
			no_of_ins += 1;
			MOVE(IL, no_of_ins, cd, st, no_of_symbols);
			continue;
		}

		if (strcmp(cd[0], "JMP") == 0) {
			no_of_ins += 1;
			IL[no_of_ins].ino = no_of_ins + 1;
			IL[no_of_ins].opcode = 6;
			int k = check(bt, no_of_blocks, cd[1]);
			if (k == -1) {
				//printf("diwdwriyrgf");
				strcpy(lab[++no_of_labels].name, cd[1]);
				lab[no_of_labels].inso = no_of_ins + 1;
				continue;
			}
			IL[no_of_ins].parameters[0] = bt[k].add;
			continue;
		}

		if ((strcmp(cd[0], "ADD") == 0)) {
			no_of_ins += 1;
			ADD(IL, no_of_ins, cd);
			continue;
		}

		if ((strcmp(cd[0], "SUB") == 0)) {
			no_of_ins += 1;
			SUB(IL, no_of_ins, cd);
			continue;
		}

		if ((strcmp(cd[0], "MUL") == 0)) {
			no_of_ins += 1;
			MUL(IL,no_of_ins,cd);
			continue;
		}

		if (x == 1 && cd[0][strlen(cd[0]) - 1] == ':') {
			bt[++no_of_blocks].add = no_of_ins + 1;
			strcpy(bt[no_of_blocks].name,get_block_name(cd[0]));
			for (int i = 0; i <= no_of_labels; i++) {
				//printf("---- %d %s\n", lab[i].inso, lab[i].name);
				if (strcmp(lab[i].name, bt[no_of_blocks].name) == 0) {
					IL[lab[i].inso-1].parameters[0] =no_of_ins+1;
					break;
				}
			}
			continue;
		}

		if (strcmp(cd[0], "PRINT") == 0) {
			no_of_ins += 1;
			PRINT(IL,no_of_ins,cd,st,no_of_symbols);
			continue;
		}

		if ((strcmp(cd[0], "IF") == 0) && (strcmp(cd[4], "THEN") == 0)) {
			no_of_ins += 1;
			IL[no_of_ins].ino = no_of_ins + 1;
			IL[no_of_ins].opcode = 7;
			IL[no_of_ins].parameters[0] = get_register_code(cd[1]);
			IL[no_of_ins].parameters[1] = get_register_code(cd[3]);
			IL[no_of_ins].parameters[2] = get_conditional_operator_code(cd[2]);
			stk.top += 1;
			stk.ins_nos[stk.top] = no_of_ins;
			continue;
		}

		if ((strcmp(cd[0], "ELSE") == 0)) {
			no_of_ins += 1;
			IL[no_of_ins].ino = no_of_ins + 1;
			IL[no_of_ins].opcode = 6;
			IL[stk.ins_nos[stk.top]].parameters[3] = no_of_ins + 1;
			stk.top -= 1;
			stk.top += 1;
			stk.ins_nos[stk.top] = no_of_ins;
			continue;
		}

		if (strcmp(cd[0], "ENDIF") == 0) {
			if (IL[stk.ins_nos[stk.top]].opcode == 7) {
				IL[stk.ins_nos[stk.top]].parameters[3] = no_of_ins + 1;
				//printf("-- %d\n", no_of_ins);
				stk.top -= 1;
				//printf("--- %d\n", stk.top);
				//printf("- %d\n", stk.ins_nos[stk.top]);
				continue;
			}
			IL[stk.ins_nos[stk.top]].parameters[0] = no_of_ins + 1;
			stk.top -= 1;
			continue;
		}

	}

//	for (int i = 0; i <= no_of_ins; i++) {
//		printf("%d %d %d %d\n", IL[i].parameters[0], IL[i].parameters[1], IL[i].parameters[2], IL[i].parameters[3]);
//	}


	fclose(flp);
	FILE *fp = fopen("Intermediate_Code.txt", "w");
	
	if (fp == NULL) {
		printf("cannot open file\n");
	}

	fprintf(fp,"Intermediate Code:\n\n");

	for (int i = 0; i <= no_of_ins; i++) {
		if (IL[i].opcode == 1 || IL[i].opcode == 2) {
			fprintf(fp, "%d %d %d %d\n", IL[i].ino, IL[i].opcode, IL[i].parameters[0], IL[i].parameters[1]);
		}
		else if (IL[i].opcode == 3 || IL[i].opcode == 4 || IL[i].opcode==5) {
			fprintf(fp, "%d %d %d %d %d\n", IL[i].ino, IL[i].opcode, IL[i].parameters[0], IL[i].parameters[1],IL[i].parameters[2]);
		}
		else if (IL[i].opcode == 6) {
			fprintf(fp, "%d %d %d\n", IL[i].ino, IL[i].opcode, IL[i].parameters[0]);
		}
		else if (IL[i].opcode == 7) {
			fprintf(fp, "%d %d %d %d %d %d\n", IL[i].ino, IL[i].opcode, IL[i].parameters[0], IL[i].parameters[1], IL[i].parameters[2],IL[i].parameters[3]);
		}
		else if (IL[i].opcode == 13) {
			fprintf(fp, "%d %d %d\n", IL[i].ino, IL[i].opcode, IL[i].parameters[0]);
		}
		else if (IL[i].opcode == 14) {
			fprintf(fp, "%d %d %d\n", IL[i].ino, IL[i].opcode, IL[i].parameters[0]);
		}
	}
	
	fprintf(fp, "\n\nSymbol Table:\n\n");

	for (int i = 0; i <= no_of_symbols; i++) {
		fprintf(fp, "%s %d %d\n", st[i].name, st[i].add, st[i].size);
	}

	fprintf(fp, "\n\nBlock Table:\n\n");

	for (int i = 0; i <= no_of_blocks; i++) {
		fprintf(fp, "%s %u\n", bt[i].name, bt[i].add);
	}

	fprintf(fp, "\n\nMemory:\n\n");

	for (int i = 0; i <=ms; i++) {
		fprintf(fp, "%d\n", memory[i]);
	}

	fclose(fp);
	_getch();
}