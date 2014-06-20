#include <stdio.h>   /* required for file operations */
#include <conio.h>  /* for clrscr */
#include <dos.h>  /* for delay */
#include <stdint.h>
#include <sys/stat.h>

int file_exist (char *filename);

FILE *elf;            /* declare the file pointer */
FILE *binary;

int main(int argc, char *argv[]) {
	char * binary_file_name;
	char * elf_file_name;
	if ( argc != 3 ) {
		printf( "usage: elf-filename, binary-filename");
	} else {
		elf_file_name = argv[1];
		binary_file_name = argv[2];

		if(file_exist(elf_file_name) != 1) return -1;

		if(file_exist(binary_file_name) == 1) {
			if(remove(binary_file_name) < -1) {
				printf("Unable to delete the file\n");
				perror("removing file");
			}
		}

		elf = fopen (argv[1], "rt");
		binary = fopen(binary_file_name,"a+");
	}

	// check filesize
	fseek(elf, 0L, SEEK_END);
	int sz = ftell(elf);
	rewind( elf );

   // write array header
   fprintf(binary,"%s","uint32_t elf [SIZE] = {\n");

   char data [sz];
   int buff_size = sizeof(char);
   fread(&data, sz, buff_size, elf);

   int i = 0;
   uint32_t values [sz/4];
   uint32_t val = 0;
   for(; i < sz; i++) {
	   int current = data[i];
	   //fprintf(binary,"0x%x, \n", current);
	   if((i % 4) == 0) {
		   if (i != 0) values[(i-1)/4] = val;
		   val = 0;
		   val += current << 24;
	   } else if ((i % 4 == 1)) {
		   val += current << 16;
	   } else if ((i % 4) == 2) {
		   val += current << 8;
	   } else if ((i % 4) == 3) {
		   val += current << 0;
	   }
   }

   i = 0;
   for (;i < sz/4; i++) {
	   fprintf(binary,"0x%x, \n", values[i]);
   }

   // print array end
   fprintf(binary,"%s","}");

   fprintf(binary, "\r\n SIZE: %i\n", sz/4);

   // close file-streams
   fclose(binary);
   fclose(elf);
} /*of main*/

int file_exist (char *filename)
{
	struct stat buffer;
	int ret;
	ret = stat(filename,&buffer);
	if (ret == 0) {
		return 1;
	} else return -1;
}
