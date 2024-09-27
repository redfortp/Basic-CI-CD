#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct arguments {
  int e, i, v, c, l, n, h, s, f, o;
  char pattern[4096];  // строка, которая будет хранить в себе регулярное
                       // выражение
  int len_pattern;
} arguments;

void pattern_add(arguments* arg, char* pattern) {
  // int n = strlen (pattern);

  if (arg->len_pattern != 0) {
    strcat(arg->pattern + arg->len_pattern, "|");
    arg->len_pattern++;
  }

  arg->len_pattern += sprintf(arg->pattern + arg->len_pattern, "(%s)", pattern);
}

void add_reg_from_file(arguments* arg, char* filepath) {
  FILE* f = fopen(filepath, "r");
  if (f != NULL) {
    char* line = NULL;
    size_t memline = 0;  // будет хранить размер памяти, которая эта функция
                         // выделит под нашу строку
    int read = 0;  // переменная, которая будет хранить количество символов,
                   // которое мы считали

    read = getline(&line, &memline, f);
    while (read !=
           -1) {  // -1 то, что вернет getline, когда дойдет до конца файла
      if (line[read - 1] == '\n') line[read - 1] = '\0';
      pattern_add(arg, line);
      read = getline(&line, &memline, f);
    }

    free(line);
    fclose(f);
  } else {
    if (!(arg->s))
      // fprintf(stderr, "grep: %s No such file or directory ", argv[optind]);
      perror(filepath);
    exit(1);
  }
}

arguments argument_parser(int argc, char* argv[]) {
  arguments arg = {0};
  int opt;
  while ((opt = getopt(argc, argv, "e:ivclnhsf:o")) !=
         -1) {  // : означает, что он еммет значение
    switch (opt) {
      case 'e':  // шаблон
        arg.e = 1;
        pattern_add(&arg, optarg);
        break;
      case 'i':  //игнорирует различия регистра
        arg.i = REG_ICASE;
        break;
      case 'v':  //инвертирует смысл поиска соответствий
        arg.v = 1;
        break;
      case 'c':  //выводит только количество совпадающих строк
        arg.c = 1;
        break;
      case 'l':  // выводит только совпадающие файлы
        arg.l = 1;
        break;
      case 'n':  // предваряет каждую строку вывода номером строки из файла
                 // ввода
        arg.n = 1;
        break;
      case 'h':  // выводит совпадающие строки не предваряя их именами файлов
        arg.h = 1;
        break;
      case 's':  // подавляет сообщения об ошибках о несуществующих и нечитаемых
                 // файлах +
        arg.s = 1;
        break;
      case 'f':  // получает регулярные выражения из файла
        arg.f = 1;
        add_reg_from_file(&arg, optarg);
        break;
      case 'o':  // печатает только сопадающие непустые части совпавшей строки
        arg.o = 1;
        break;

      default:
        break;
    }
  }
  if (arg.len_pattern == 0) {
    pattern_add(&arg, argv[optind]);
    optind++;
  }
  if (argc - optind == 1) {
    arg.h = 1;
  }
  return arg;
}

void output_line(char* line, int n) {  //обработка посимвольно
  for (int i = 0; i < n; i++) {
    putchar(line[i]);
  }
  if (line[n - 1] != '\n') putchar('\n');
}

void print_match(regex_t* re, char* line) {
  regmatch_t math;
  int offset = 0;
  while (1) {
    int result = regexec(re, line + offset, 1, &math, 0);
    if (result != 0) {
      break;
    }
    for (int i = math.rm_so; i < math.rm_eo; i++) {
      putchar(line[i]);
    }
    putchar('\n');
    offset += math.rm_eo;
  }
}

void processline() {  // обработка строки
}

//обработка файла
void processFile(arguments arg, char* path, regex_t* reg) {
  FILE* f = fopen(path, "r");
  if (f != NULL) {
    char* line = NULL;
    size_t memline = 0;  // будет хранить размер памяти, которая эта функция
                         // выделит под нашу строку
    int read = 0;  // переменная, которая будет хранить количество символов,
                   // которое мы считали
    int line_count = 1;
    int c = 0;
    read = getline(&line, &memline, f);
    while (read !=
           -1) {  // -1 то, что вернет getline, когда дойдет до конца файла
      int result = regexec(reg, line, 0, NULL, 0);

      if ((result == 0 && !arg.v) || (arg.v && result != 0)) {
        if (!(arg.c) && !(arg.l)) {
          if (!(arg.h)) printf("%s:", path);
          if (arg.n) printf("%d:", line_count);
          if (arg.o) {
            print_match(reg, line);
          } else {
            output_line(line, read);
          }
        }
        c++;
      }
      read = getline(&line, &memline, f);
      line_count++;
    }
    free(line);
    if (arg.c && !(arg.l)) {
      if (!(arg.h)) printf("%s:", path);
      printf("%d\n", c);
    }
    if (arg.l && c > 0) printf("%s\n", path);
    fclose(f);
  } else {
    if (!(arg.s))
      // fprintf(stderr, "grep: %s No such file or directory ", argv[optind]);
      perror(path);
    exit(1);
  }
}
// вывод из всех файлов
void output(arguments arg, int argc, char** argv) {
  regex_t re;
  int error = regcomp(&re, arg.pattern, REG_EXTENDED | arg.i);  // флаг i
  if (error) perror("Error");

  for (int i = optind; i < argc; i++) {
    processFile(arg, argv[i], &re);
  }
  regfree(&re);
}

int main(int argc, char** argv) {
  arguments arg = argument_parser(argc, argv);
  output(arg, argc, argv);
  return 0;
}
