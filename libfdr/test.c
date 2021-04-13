#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "jrb.h"
#include "fields.h"

int cmpstr(void* v1, void* v2)
{

    char *a1 = *(char**)v1;
    char *a2 = *(char**)v2;
    return strcmp(a1, a2);
}

int cmpnum(void* s1, void* s2)
{
    int *a = (int*)s1;
    int *b = (int*)s2;
    if ((*a) > (*b))
        return 1;
    else if ((*a) < (*b))
        return -1;
    else
        return 0;
}

void swap(void* v1, void* v2, int size)
{
    char buffer[size];
    memcpy(buffer, v1, size);
    memcpy(v1, v2, size);
    memcpy(v2, buffer, size);
}

void _qsort(void* v, int size,void* v2, int size2, int left, int right,int (*comp)(void*, void*))
{
    void *vt, *v3;
    void *vt2, *v32;
    int i, last, mid = (left + right) / 2;
    if (left >= right)
        return;

    // casting void* to char* so that operations
    // can be done.
    void* vl = (char*)(v + (left * size));
    void* vr = (char*)(v + (mid * size));
    void* vl2 = (char*)(v2 + (left * size2));
    void* vr2 = (char*)(v2 + (mid * size2));

    swap(vl, vr, size);
    swap(vl2, vr2, size2);

    last = left;
    for (i = left + 1; i <= right; i++) {

        // vl and vt will have the starting address
        // of the elements which will be passed to
        // comp function.
        vt = (char*)(v + (i * size));
        vt2 = (char*)(v2 + (i * size2));

        if ((*comp)(vl, vt) < 0) {
            ++last;
            v3 = (char*)(v + (last * size));
            v32 = (char*)(v2 + (last * size2));
            swap(vt, v3, size);
            swap(vt2, v32, size2);
        }
    }
    v3 = (char*)(v + (last * size));
    v32 = (char*)(v2 + (last * size2));

    swap(vl, v3, size);
    swap(vl2, v32, size2);

    _qsort(v, size, v2 , size2, left, last - 1, comp);
    _qsort(v, size, v2 , size2, last + 1, right, comp);

}


void toLowerCase(char str[100])
{
  int i;
     for(i=0;i<=strlen(str);i++){
        if(str[i]>=65&&str[i]<=90)
           str[i]=str[i]+32;
     }
}

int main(int argc, char **argv){
  if(argc!=2) {
    printf ( "Usage: printwords filename\n");
    exit(1);
  }

  JRB b;
  JRB bn;

  int count=0;
  int numberOfWords=0;
  IS is;  int i;
  char word[100];

  is = new_inputstruct(argv[1]);
  b = make_jrb();
  if (is == NULL) {
    perror(argv[1]);
    exit(1);
  }

  while(get_line(is) >= 0) {

    for (i = 0; i < strlen(is->text1); i++) {
      if((is->text1[i]<=57 && is->text1[i]>=48)
       ||(is->text1[i]<=90 && is->text1[i]>=65)
       ||(is->text1[i]<=122 && is->text1[i]>=97)
          ){
            word[count] = is->text1[i];
            count ++;
        }
      else if(strlen(word)>0){
          toLowerCase(word);
          JRB temp=jrb_find_str(b, word);
          if(temp==NULL) {
            (void) jrb_insert_str(b, strdup(word), new_jval_i(1));
            numberOfWords++;
          }
          else {
            temp->val.i++;
          }


          memset(word , '\0', sizeof word);
          count = 0;

      }
    }
  }
  int * listNumber = (int *)malloc(numberOfWords*sizeof(int));

  char **liststring;
  liststring = malloc(numberOfWords * sizeof(char*));
  for (i = 0; i < numberOfWords; i++)
      liststring[i] = malloc((100) * sizeof(char));

  i=0;
  jrb_traverse(bn, b) {
    listNumber[i] = bn->val.i;
    strcpy(liststring[i], bn->key.s);
    i++;
  }
  // for (i = 0; i < numberOfWords; i++){
  //   printf("%d  %s\n", listNumber[i], liststring[i]);
  // }
  //---------------------------------

    (void) jrb_free_tree(b);

    //_qsort(a, sizeof(char*), 0, 8, (int (*)(void*, void*))(cmpstr));
    _qsort(listNumber, sizeof(int),liststring, sizeof(char*), 0, numberOfWords-1, (int (*)(void*, void*))(cmpnum));
    //quicksort(listNumber,0,numberOfWords-1, int_compare,sizeof(int));


    for (i = 0; i < numberOfWords; i++){
      printf("%d  %s\n", listNumber[i], liststring[i]);
    }
  //---------------------------------
  jettison_inputstruct(is);
  return 0;
}
