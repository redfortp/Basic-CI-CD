#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct arguments {
  int b, n, s, E, T, v;
} arguments;

arguments argument_parser(int argc, char** argv) {
  arguments arg = {0};  // инициализация структуры
  struct option long_options[] = {// эта структура в бибиотеке getopt
                                  {"number", no_argument, NULL, 'n'},
                                  {"number-nonblank", no_argument, NULL, 'b'},
                                  {"squeeze-blank", no_argument, NULL, 's'},
                                  {0, 0, 0, 0}};

  int opt = 0;  // результат парсинга нашего флага
  while (opt != -1) {
    opt = getopt_long(argc, argv, "+bneETtsv", long_options, 0);
    switch (opt) {
      case 'b':  // нумерация непустых строк
        arg.b = 1;
        break;
      case 'n':  // нумерация всех строк +
        arg.n = 1;
        break;
      case 's':  // автоматическ удалять пустые повторяющиеся строки
        arg.s = 1;
        break;
      case 'E':  // в конце каждой строки $ +
        arg.E = 1;
        break;
      case 'T':  // табуляция отмечается, как ^|
        arg.T = 1;
        break;
      case 't':  // табуляция отмечается, как ^|
        arg.T = 1;
        arg.v = 1;
        break;
      case 'e':  // в конце каждой строки $ +
        arg.E = 1;
        arg.v = 1;
        break;
      case 'v':
        arg.v = 1;
        break;
      case '?':
        printf("usage: cat [-belnstuv] [file ...]");
        perror("ERROR");
        exit(1);
        break;

      default:
        break;
    }
  }
  return arg;
}

char v_output(char ch) {
  if (ch == '\n' || ch == '\t') return ch;
  if (ch < 0) {
    printf("M-");
    ch = ch & 0x7F;  // многобайтовые символы
  }
  if (ch <= 31) {
    putchar('^');
    ch += 64;
  } else if (ch == 127) {
    putchar('^');
    ch = '?';
  }
  return ch;
}

void outline(arguments* arg, char* line, int n) {
  for (int i = 0; i < n; i++) {
    if (arg->T && line[i] == '\t') {
      printf("^");
      line[i] = 'I';
    } else {
      if (arg->E && line[i] == '\n') {
        if (arg->b && line[0] == '\n') {
          printf("      \t$");
        } else
          printf("$");
      }
    }

    if (arg->v) line[i] = v_output(line[i]);
    putchar(line[i]);
  }
}

void output(arguments* arg, char** argv, int number) {
  // printf ("%d", optind);
  FILE* f = fopen(argv[number], "r");
  if (f != NULL) {
    char* line = NULL;
    size_t memline = 0;  // будет хранить размер памяти, которая эта функция
                         // выделит под нашу строку
    int read = 0;  // переменная, которая будет хранить количество символов,
                   // которое мы считали

    int line_count = 1;
    int empty_count = 0;
    read = getline(&line, &memline, f);
    while (read !=
           -1) {  // -1 то, что вернет getline, когда дойдет до конца файла
      if (line[0] == '\n')
        empty_count++;
      else
        empty_count = 0;
      if (arg->s && empty_count > 1) {
      } else {
        if (arg->n || arg->b) {
          if (arg->b && line[0] != '\n') {
            printf("%6d\t\t", line_count);
            line_count++;
          } else if (arg->n && !(arg->b)) {
            printf("%6d\t", line_count);
            line_count++;
          }
        }
        outline(arg, line, read);
      }
      read = getline(&line, &memline, f);
    }
    free(line);
    fclose(f);
  } else {
    fprintf(stderr, "cat: %s No such file or directory", argv[number]);
  }
}

int main(int argc, char** argv) {
  arguments arg = argument_parser(argc, argv);
  int number = optind;
  while (argc > number) {
    output(&arg, argv, number);
    number++;
  }

  return 0;
}
