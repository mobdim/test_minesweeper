//
//  main.c
//  minesweeper
//
//  Created by Dmitriy Zadoroghnyy on 21/03/2018.
//  Copyright © 2018 Dmitriy Zadoroghnyy. All rights reserved.
//

#include <stdio.h>

#define linesSize 100
#define columnsSize 100

int field[linesSize][columnsSize];
int max_lines = linesSize;
int max_columns = columnsSize;

void clear_field() {
  int i;
  for (i = 0; i < max_lines; i++) {
    int j;
    for (j = 0; j < max_columns; j++) {
      field[i][j] = 0;
    }
  }
}

void set_bounds(int l, int c) {
  max_lines = l;
  max_columns = c;
}

int is_inside_bounds(int i, int j) {
  return i >= 0 && i < max_lines && j >=0 && j < max_columns;
}

void increment(int i, int j) {
  if (is_inside_bounds(i, j) && field[i][j] != -1) {
    ++field[i][j];
  }
}

void set_mine(int i, int j) {
  field[i][j] = -1;
  
  int m;
  for (m = -1; m <= 1; m++) {
    int n;
    for (n = -1; n <= 1; n++) {
      increment(i + m, j + n);
    }
  }
}

void print_field() {
  int i;
  for (i = 0; i < max_lines; i++) {
    int j;
    for (j = 0; j < max_columns; j++) {
      int x = field[i][j];
      
      if (x == -1) {
        printf("X ");
      } else {
        printf("%d ", x);
      }
    }
    printf("\n");
  }
}

char* remove_spaces(char* input)
{
  int i, j;
  char *output = input;
  for (i = 0, j = 0; i < strlen(input); i++, j++) {
    if (input[i] != ' ') {
      output[j] = input[i];
    } else {
      j--;
    }
  }
  output[j] = 0;
  return output;
}

#define bufSize 1024
int main(int argc, const char * argv[]) {
  
  FILE* fp;
  char buf[bufSize];
  char *alllines[linesSize];
  int lines = 0, columns = 0;
  memset(alllines, 0, bufSize);
  
  printf("Calulate mines!\n");
  
  if (argc != 2) {
    puts("no file!");
    return 1;
  }
  
  const char *filename = argv[1];
  printf("reading %s\n", filename);
  
  if ((fp = fopen(filename, "r")) == NULL) { /* Open source file. */
    perror("fopen source-file");
    return 1;
  }
  
  while ( fgets(buf, sizeof(buf), fp) != NULL) {
    buf[strlen(buf) - 1] = '\0';
    char *nospacesbuf = remove_spaces(buf);
    if (!columns) {
      columns = strlen(nospacesbuf);
    } else if (columns != strlen(nospacesbuf)) {
      puts("error columns length!");
      fclose(fp);
      return 2;
    }
    alllines[lines] = strdup(nospacesbuf);
    lines++;
  }
  
  fclose(fp);
  
  
  set_bounds(lines, columns);
  clear_field();
  
  for (int i = 0; i < lines; i++) {
    char line[columns + 1];
    strncpy(line, alllines[i], sizeof(line));
    int j;
    for (j = 0; j < columns; j++) {
      char currentChar = line[j];
      if (currentChar == 'X') {
        set_mine(i, j);
      }
    }
  }
  
  print_field();
  return 0;
}
