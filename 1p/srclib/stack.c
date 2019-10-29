#include <stddef.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include "stack.h"

struct stack {
    size_t top;
    size_t size;
    size_t memb_sz;
    void *elems;
};

stack *stack_new(){
    return malloc(sizeof(stack));
}

void stack_delete(stack *s){
    if(s){
        free(s);
    }
}

stack *stack_init(stack *s, size_t size, size_t memb_sz){
    if(s){
        if(size){
            s->top = 0;
            s->size = size;
            s->elems = malloc(size*memb_sz);
            s->memb_sz = memb_sz;
            if(!s->elems) s->size = 0;
        }else{
            *s = (stack){0};
        }
    }
    return s;
}

void stack_destroy(stack *s){
    if(s){
        free(s->elems);
        stack_init(s, 0, 0);
    }
}

void stack_peek(stack *s, void *dest){
    if(!s || !s->elems){
        errno = EINVAL;
        perror("stack_peek");
    }else if(!s->top){
        errno = EPERM;
        perror("stack is empty");
    }else{
        void *src = s->elems+(s->top-1)*s->memb_sz;
        memcpy(dest, src, s->memb_sz);
    }
}

void stack_pop(stack *s, void *dest){
    if(!s || !s->elems){
        errno = EINVAL;
        perror("stack_peek");
    }else if(!s->top){
        errno = EPERM;
        perror("stack is empty");
    }else{
        stack_peek(s, dest);
        s->top--;
    }
}

void stack_push(stack *s, void *elem){
    if(!s || !s->elems){
        errno = EINVAL;
        perror("stack_push");
        return;
    }
    if(s->top == s->size){
        void *old = s->elems;
        s->elems = realloc(s->elems, 2*s->size);
        if(!s->elems){
            errno = EPERM;
            perror("stack is full");
            s->elems = old;
            return;
        }
        s->size *=2;
    }
    void *dest = s->elems + s->top * s->memb_sz;
    memcpy(dest, elem, s->memb_sz);
    s->top++;
}

void stack_print(FILE *fout, stack *s, char *(*to_str)(void *, size_t)){
    for(int i = s->top-1; i>= 0; i--){
        char *str = to_str(s->elems + (i*s->memb_sz), s->memb_sz);
        fprintf(fout, "( %s ) ", str);
        free(str);
    }
    fprintf(fout, "\n");
}
