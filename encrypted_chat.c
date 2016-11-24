#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int isFileEmpty();
int compareString();
char* addUser();
void turnChatEncrypted();
char* getString();
void writeString();
void decrypt();
void encrypt();
void reverse();

int main ( int argc, char * argv[]){

	turnChatEncrypted( argv[1], argv[2], argv[3], argv[4]);
}

void turnChatEncrypted ( char* file_in, char* file_out, char* user, char* k){
	int max_size = 300;
	int key = atoi( k);
	// void segmented error if file not exist
	FILE* file_read = fopen(file_in, "rb");
	if ( file_read == NULL){
		file_read = fopen(file_in, "w");
	}

	// check for initial condition
	if ( isFileEmpty(file_read) == 1){
		printf("NOthing received yet.\n");
		printf("Send:         ");
		char* message_in= malloc(max_size + 1);
		fgets(message_in, max_size, stdin);
		writeString(file_out, message_in, user, key);
	}
	fclose(file_read);

	// while loop with turn 
	int turn = 1;
	char* queue_message = malloc( max_size + 1);
	while (1){
		char* current_message = getString( file_in, key);
		if ( compareString( current_message, queue_message) == 0){
			printf("Received:   %s", current_message);
			strcpy( queue_message, current_message);
			turn = 0;
		}
		if ( turn == 0){
			printf("Send:         ");
			char* message_in = malloc(max_size + 1);
			fgets(message_in, max_size, stdin);
			writeString(file_out, message_in, user, key);
			turn = 1;
		}

	}
}

void writeString ( char* file_name, char* message, char* user, int key){
	FILE* file_write = fopen(file_name, "wb");
	char* message_processed = addUser(message, user);
	// encrypt input message
	encrypt( message_processed, key);
	fputs(message_processed, file_write);
	fclose(file_write);
}

char* getString ( char* file_name, int key){
	FILE* file_read = fopen( file_name, "r");
	fseek(file_read, 0, SEEK_END);
	int fsize = ftell( file_read);
	fseek( file_read, 0, SEEK_SET);
	char* message = malloc( fsize + 1);
	fread(message, fsize, 1, file_read);
	message[fsize] = '\0';
	fclose(file_read);
	decrypt(message, key);
	return message;
}

int isFileEmpty( FILE* file){
	int current_pos = ftell(file);
	fseek(file, 0, SEEK_END);
	if (ftell(file) == 0){
		fseek(file, current_pos, SEEK_SET);
		return 1;
	}
	fseek(file, current_pos, SEEK_SET);
	return 0;
}
int compareString( char* string1, char*string2){
	while ( *string1 != '\0'){
		if ( *string1 != *string2){
			return 0;
		}
		string1++;
		string2++;
	}
	return 1;
}
char* addUser( char*string1, char*user){
	int n = strlen(string1);
	int N = strlen(user);
	char* newString=malloc(n +  N + 4);
	strcat(newString, "[");
	strcat(newString, user);
	strcat(newString, "] ");
	strcat(newString, string1);
	return newString;
}

void decrypt ( char*str, int key){
	int c, i;
	for ( i = 0; i < strlen(str); i++){
		if ( (c = (str[i] - key)) < 0 ){
			c += 256;
		}
		str[i] = c;
	}
	str[i] = '\0';
	reverse(str);
}

void reverse( char * str){
	if (str == 0 || *str == 0){
		return;
	}
	char *start = str + 0;
	char *end = start + strlen(str) -1;
	char temp;
	while (end > start){
		temp = *start;
		*start++ = *end;
		*end-- = temp;
	}
}

void encrypt( char* str, int key){
	reverse(str);
	int i;
	for (i = 0; i < strlen(str); i++){
		str[i] = (str[i] + key)% 256;
	}
}