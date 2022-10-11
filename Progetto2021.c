#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Constants for the program

#define true 1
#define UNREACHABLE 0
#define COMMBUFFER 20

enum status{F, T};

typedef struct t{
  unsigned long long int index;
  unsigned long long int value;
  struct t *left;
  struct t *right;
} tier_t;

typedef struct{
  unsigned long long int value;
  enum status status;
} table_t;

//Custom parser

unsigned long int Parser(char **line) {
  unsigned long int value = 0;
  while(**line){
    value = value*10 + (unsigned long int)((**line) - '0');
    (*line)++;
    if(**line - ',' == 0 || **line - '\n' == 0 || **line - ' ' == 0) {
      (*line)++;
      break;
    }
  }
  return value;
}

//Dijkstra functions

void Line(char *line, table_t a[], unsigned long long int *m, unsigned long int *m_i) {
  unsigned long int i=0;
  unsigned long int min_i = 0;
  unsigned long long int temp, min = UNREACHABLE;
  while(*line){
    temp = (unsigned long long int) Parser(&line);
    if(i != 0 && i != *m_i && a[i].status == F && temp != 0){
      temp += a[*m_i].value;
      if(a[i].value == UNREACHABLE || temp < a[i].value){
        a[i].value = temp;
      }
    }
    if(i != 0 && i != *m_i && a[i].status != T && min == UNREACHABLE && a[i].value != UNREACHABLE) {
      min = a[i].value;
      min_i = i;
    }
    if(i != 0 && i != *m_i && a[i].value != UNREACHABLE && a[i].status == F && a[i].value < min) {
      min = a[i].value;
      min_i = i;
    }
    i++;
  }
  *m = min;
  *m_i = min_i;
}

void FirstLine(char *line, table_t a[], unsigned long long int *m, unsigned long int *m_i) {
  unsigned long int i = 0;
  unsigned long int min_i = 0;
  unsigned long long int min = UNREACHABLE;
  while(*line) {
    a[i].value = Parser(&line);
    if(i != 0 && min == UNREACHABLE && a[i].value != UNREACHABLE) {
      min = a[i].value;
      min_i = i;
    }
    if(i != 0 && a[i].value != UNREACHABLE && a[i].value < min) {
      min = a[i].value;
      min_i = i;
    }
    i++;
  }
  *m = min;
  *m_i = min_i;
}

unsigned long long int Dijkstra(char ***matrix, table_t a[], unsigned long int max_nodes) {
  unsigned long int i, next, count = 0;
  unsigned long long int min, sum = 0;
  char *line;
  for(i = 0; i < max_nodes; i++){
    a[i].value = UNREACHABLE;
    a[i].status = F;
  }
  line = **matrix;
  FirstLine(line, a, &min, &next);
  if(min != UNREACHABLE){
    a[0].value = 0;
    a[0].status = T;
    line = *(*matrix + next);
    while((count < max_nodes - 1) && (next > 0)){
      a[next].status = T;
      Line(line, a, &min, &next);
      if(min == UNREACHABLE){
        break;
      }
      else {
        line = *(*matrix + next);
        count++;
      }
    }
    for(i = 1; i < max_nodes; i++){
      sum += a[i].value;
    }
    return sum;
  }
  else {
    return 0;
  }
}

//Program's fucntions

//The first read of the program
void FirstRead(unsigned long int *max_nodes, unsigned long int *tier_length){
  char *buffer;
  buffer = (char *) malloc(COMMBUFFER);
  if(fgets(buffer, COMMBUFFER, stdin)){
    *max_nodes = Parser(&buffer);
    *tier_length = Parser(&buffer);
  }
}

void InitializeMatrix(char ***matrix, unsigned long int max_nodes, unsigned long int b_size){
  unsigned long int i = 0;
  for(i = 0; i < max_nodes; i++){
    (*matrix)[i] = (char *) malloc(b_size);
  }
}

void AddGraph(char ***matrix, unsigned long int max_nodes, unsigned long int b_size) {
  unsigned long int i;
  for(i = 0; i < max_nodes; i++) {
    if(fgets((*matrix)[i], b_size, stdin)){
    }
  }
}

//Tier functions

unsigned long int InitArray(tier_t **root, unsigned long int size){
  unsigned long int l, r, i = 0;
  *root = (tier_t *) calloc(sizeof(tier_t), size);
  while(true) {
    l = 2*i+1;
    r = 2*i+2;
    if(size > l) {
      (*((*root)+i)).left = ((*root)+l);
    }
    if(size > r) {
      (*((*root)+i)).right = ((*root)+r);
    }
    else {
      break;
    }
    i++;
  }
  return 0;
}

void MaxHeapify(tier_t **root, unsigned long int size, unsigned long int i) {
  unsigned long int l, r, max = i;
  unsigned long long int tvalue, tindex;
  l = 2*i + 1;
  r = 2*i + 2;
  if(l < size && (*((*root)+l)).value > (*((*root)+i)).value) {
    max = l;
  }
  if(r < size && (*((*root)+r)).value > (*((*root)+max)).value){
    max = r;
  }
  if(max != i) {
    tvalue = (*((*root)+i)).value;
    tindex = (*((*root)+i)).index;
    (*((*root)+i)).value = (*((*root)+max)).value;
    (*((*root)+i)).index = (*((*root)+max)).index;
    (*((*root)+max)).value = tvalue;
    (*((*root)+max)).index = tindex;
    MaxHeapify(root, size, max);
  }
}

void BuildMaxHeap(tier_t **root, unsigned long int size) {
  unsigned long int i = size/2 -1;
  while(true) {
    MaxHeapify(root, size, i);
    if(i == 0){
      break;
    }
    i--;
  }
}

void Heapsort(tier_t **root, unsigned long int size) {
  unsigned long long int tvalue, tindex;
  unsigned long int i, hsize = size;
  BuildMaxHeap(root, hsize);
  for(i = size - 1; i > 0; i--) {
    tvalue = (**root).value;
    tindex = (**root).index;
    (**root).value = (*((*root)+i)).value;
    (**root).index = (*((*root)+i)).index;
    (*((*root)+i)).value = tvalue;
    (*((*root)+i)).index = tindex;
    hsize--;
    MaxHeapify(root, hsize, 0);
  }
}

void CheckValue(tier_t **root, unsigned long int size,
  unsigned long int *current,
  unsigned long long int index, unsigned long long int result, tier_t **max) {
    if(*current < size){
      (*((*root) + *current)).index = index;
      (*((*root) + *current)).value =  result;
      *current = (*current) + 1;
      if(*current == size) {
        Heapsort(root, size);
        *max = (*root) + size - 1;
      }
    }
    else {
      if((**max).value <= result) {
        return;
      }
      else {
        unsigned long int min;
        min = ((*max) - (*root) + 1)%size;
        if((*((*root)+min)).value > result) {
          (**max).index = index;
          (**max).value = result;
          *max = (*root) + ((size + min - 2)%size);
        }
        else {
          (**max).index = index;
          (**max).value = result;
          Heapsort(root, size);
          *max = (*root) + size - 1;
        }
      }
    }
}

void IntToString(unsigned long long int value) {
  unsigned long long int div = 1, rest, temp = value;
  int order = 0;
  while(temp > 9) {
    div = div * 10;
    temp = value/div;
    order++;
  }
  temp = value;
  while(order > 0) {
    rest = temp%div;
    temp = temp/div;
    putchar_unlocked(temp + '0');
    temp = rest;
    div = div/10;
    order--;
  }
  putchar_unlocked(temp + '0');
}

void ShowTier(tier_t **root, unsigned long long int current) {
  unsigned long long int i;
  if(current == 0) {
    putchar_unlocked('\n');
  }
  else{
    for(i = 0; i < current; i++) {
      IntToString((*((*root) + i)).index);
      if(i == current - 1){
        putchar_unlocked('\n');
      }
      else{
        putchar_unlocked(' ');
      }
    }
  }
}

//Global variables

unsigned long int max_nodes, tier_length;

int main(int argc, char * argv[]){
  unsigned long long int b_size, result, i = 0;
  unsigned long int current;

  FirstRead(&max_nodes, &tier_length);
  b_size = max_nodes*11;
  tier_t *max, *tier;
  char **matrix, *command = NULL;
  table_t a[max_nodes];

  command = (char *)malloc(COMMBUFFER);
  matrix = (char **)malloc(max_nodes*sizeof(char *));

  InitializeMatrix(&matrix, max_nodes, b_size);
  current = InitArray(&tier, tier_length);

  while(true) {
    if(!fgets(command, COMMBUFFER, stdin)) { //Il !comando permette di trovare il EOF
      break;
    }
    if(strcmp(command, "AggiungiGrafo\n") == 0) {
      AddGraph(&matrix, max_nodes, b_size);
      result = Dijkstra(&matrix, a, max_nodes);
      CheckValue(&tier, tier_length, &current, i, result, &max);
      i++;
      }
    else if(strcmp(command, "TopK\n") == 0) {
      ShowTier(&tier, current);
    }
  }
  return 0;
}
