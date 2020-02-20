/*
// Created by shadikanda on 9/8/19.
*/



#include <stdlib.h>
#include "vector_generic.h"

struct Vector{
    void **m_buffer;
    size_t m_size;
    size_t m_capacity;
};

Vector* vectorCreate(size_t size) {
    Vector *vector = NULL;
    if(size<=0) {
        return NULL;
    }

    vector = malloc(sizeof(Vector));
    if(!vector){
        return NULL;
    }

    vector->m_buffer = malloc(sizeof(void*)* size);
    if(!vector->m_buffer){
        return NULL;
    }

    vector->m_capacity = size ;
    vector->m_size = 0;

    return  vector;
}

void vectorDestroy(Vector **vector){
    if(!(*vector)->m_buffer){
        free((*vector)->m_buffer);
        printf("The buffer has been freed!\n");
    }
    free(*vector);
    *vector = NULL;
    printf("The vector has been destroyed successfully!\n");
}

/* Adds an item at the end. Grows if needed (by * 2) */
ErrorCode vectorPush(Vector *vector, void *value){ /*problem in realloc*/

    if(!vector) {
        return  E_NULL_PTR;
    }

    /* Validate overflow. If full reallocate. */
    if (vector->m_size >= vector->m_capacity ) { /* better to put assert than if in this case, to protect and to make it
    * fast cause this function would be used a lot */
        if(!realloc_func(vector))
            return E_ALLOCATION_ERROR;
    }

    vector->m_buffer[vector->m_size++] = &value;
    return E_OK;
}

/* Adds an item at a certain position and shifts. Grows if needed (by * 2) */
ErrorCode vectorInsert(Vector *vector, void *value, size_t index){
    size_t i = 0;
    if(!vector) {
        return  E_NULL_PTR;
    }

    if(index > vector->m_size || index < 0 ) {
        return E_BAD_INDEX;
    }

    if (vector->m_size >= vector->m_capacity-1 ) {
        if(!realloc_func(vector))
            return E_ALLOCATION_ERROR;
    }

    /* better make a function for right shifting.
    * Also we can make a generic shift function for left & right with ENUMs */
    i = vector->m_size++;
    while (i > index){
        vector->m_buffer[i] = vector->m_buffer[i-1];
        i--;
    }
    vector->m_buffer[index + 1]=value;

    return E_OK;
}

ErrorCode vectorPop(Vector *vector, void **res) {
    if(!vector) {
        return  E_NULL_PTR;
    }

    if ( 0 == vector->m_size ) {
        return E_UNDERFLOW;
    }

/*    *res=vector->m_buffer[--vector->m_size];*/
    vectorRemove(vector, vector->m_size - 1, *res);
    return E_OK;
}


ErrorCode vectorRemove(Vector *vector, size_t index, void **res){
    size_t i = index;
    void *temp = vector->m_buffer;
    if(!temp) {
        return  E_NULL_PTR;
    }

    if ( index <0 || index > vector->m_size) {
        return E_BAD_INDEX;
    }

    if ( 0 == vector->m_size) {
        return E_UNDERFLOW;
    }

    *res = vector->m_buffer[index];
    while(i < vector->m_size){
        vector->m_buffer[i] = vector->m_buffer[i+1];
        i++;
    }

    vector->m_buffer = realloc(vector->m_buffer, sizeof(void*)*(--vector->m_size));
    if(!vector->m_buffer) {  /* checks if the reallocation is successful*/
        return E_ALLOCATION_ERROR;
    }

    vector->m_capacity = vector->m_size;
    free(temp);
    return E_OK;

}

ErrorCode vectorGetElement(const Vector *vector, size_t index, void **res){
    if (!vector){
        return E_NULL_PTR ;
    }
    else if ( index < 0 || index > vector->m_size) {
        return E_BAD_INDEX;
    }

    *res=vector->m_buffer[index];
    return E_OK;

}

ErrorCode vectorSetElement(Vector *vector, size_t index, void *value){
    if (!vector){
        return E_NULL_PTR ;
    }
    else if ( index <0 || index > vector->m_size) {
        return E_BAD_INDEX;
    }

    vector->m_buffer[index] = value;
    return E_OK;
}

size_t vectorGetSize(const Vector *vector){
    return vector->m_size;
}

size_t vectorGetCapacity(const Vector *vector){
    return vector->m_capacity;
}

size_t vectorCount(const Vector *vector, void *value){
    size_t i = 0, c = 0;
    void *res;

    if(!vector) {
        return  E_NULL_PTR;
    }

    while (i < vector->m_size){
        vectorGetElement(vector, i++, &res);
        if(*(int *)value == *(int *)res) {
            c++;
        }
    }
    return c;
}

int realloc_func(Vector *vector){

    /* Realloc copy's the old memory to the new one. */
    /* Realloc frees old memory alone. */

    void **res = realloc(vector->m_buffer,(2*(vector->m_capacity)) * sizeof(void*));

    if(!res) {  /* checks if the reallocation is successful*/
        return 0;
    }
    vector->m_capacity *= 2;
    vector->m_buffer = res;
    return 1;
}

void printAll(Vector *vector) {
    size_t size = vectorGetSize(vector), i = 0;
    void *res;
    printf("size = %lu\n", size);
    for(; i < size; ++i){
        vectorGetElement(vector, i, &res);
        printf("%lu: %d <> ", i, *(int *)res);
    }
}


