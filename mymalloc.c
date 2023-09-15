#include<stdio.h>
#include<stddef.h>

#define mem 25000

char memory[mem];

struct allocated{
    size_t size;
    struct allocated  *nothing;
};


struct free{
    size_t size;
    struct free *next;
};

struct garbage{
    int check;
    int size;
};

struct free *start;
struct free *end ;


int count =0;

void split(struct allocated *fit, size_t S,size_t size,struct free *temp){

      struct free *current,*prev;
        current=start;
        while(((current->next)!=end)){
        if(current == (void*)fit){
            break;
        }

  prev=current;
  current=current->next;
        }

    if((S-(size+sizeof(struct allocated)))> (sizeof(struct free)) && ((void*)current == start)){
      
        struct free *new=(void*)((void*)fit+size+sizeof(struct allocated));
        new->size=S-(size+sizeof(struct allocated));
        new->next=temp;
       start =new;
       start->next = current->next;
        fit->size=size;
         printf("ALLOCATED\n");
        printf("SPLITED\n\n\n");
        
    }

   else if((S-(size+sizeof(struct allocated)))> (sizeof(struct free))){
      
        struct free *new=(void*)((void*)fit+size+sizeof(struct allocated));
        new->size=S-(size+sizeof(struct allocated));
        new->next=temp;
        prev->next =new;
        fit->size=size;
         printf("ALLOCATED\n");
        printf("SPLITED\n\n\n");
        
    }

    else if((S>(size+sizeof(struct allocated))) && (S-(size+sizeof(struct allocated)))< (sizeof(struct free))&& ((void*)current ==start)){
       start = current->next;
       start->next=current->next->next;
     
       fit->size=size;
        printf("ALLOCATED\n");
       printf("GARBAGE MEMORY CREATED!!!\n\n\n");

        if((int)(S-(size+sizeof(struct allocated)))>=8){
          
       struct garbage *gar = (void*)fit+size+sizeof(struct allocated);
      gar->size = (S-(size+sizeof(struct allocated)));
      gar->check =1;
    
        }
        
    }
     else if((S>(size+sizeof(struct allocated))) && (S-(size+sizeof(struct allocated)))< (sizeof(struct free)) && ((void*)current !=start)){
       prev->next = temp->next;
     
       fit->size=size;
        printf("ALLOCATED\n");
       printf("GARBAGE MEMORY CREATED!!!\n\n\n");

        if((int)(S-(size+sizeof(struct allocated)))>=8){
       struct garbage *gar = (void*)fit+size+sizeof(struct allocated);
      gar->size = (S-(size+sizeof(struct allocated)));
      gar->check =1;
        }
        
    }

    else if((S==(size+sizeof(struct allocated))) && (S-(size+sizeof(struct allocated)))< (sizeof(struct free))&&((void*)current !=start)){
       prev->next = temp->next;
    
       fit->size=size;
        printf("ALLOCATED\n\n\n");
       
        
    }

     else if((S==(size+sizeof(struct allocated))) && (S-(size+sizeof(struct allocated)))< (sizeof(struct free))&& ((void*)current ==start)){
       start = current->next;
       start->next= current->next->next;
    
       fit->size=size;
        printf("ALLOCATED\n\n\n");
           
       
        
    }
}


void *MyMalloc(size_t Size){
    void* result;
    int flag =0;


if((count == 0) && (Size <(mem-2*sizeof(struct free)))){
    
    end = ((void*)memory + mem) - sizeof(struct free);
    end->next = end;
   

    struct allocated *Block = (void*)memory;
    Block->size = Size;

    start = (void*)memory + sizeof(struct allocated) + Size;
    start->next = end;
    start->size =mem -(Size+2*sizeof(struct free));
   

    printf("ALLOCATED\nSPLITED\n\n\n");

    count++;
    flag =1;

   

    result = ((void*)Block + sizeof(struct allocated));
    
}
else if((count >0) && (start != end) && (Size <(mem-2*sizeof(struct free)))){

    struct free *current = start ;
    struct free *prev;

    while((current->size < sizeof(struct allocated)+Size) && (((int)(current->size)) > 0)&&(current->next != end))
     {
         
         if(current->next !=end){
            prev = current;
           current = current->next; 
        }
       
     }

        if(current->size >Size){


        if((current == start) && (current->size == Size+sizeof(struct allocated)) && ((int)current->size > 0)){

            struct free *temp = current;
            struct free *temp1= current->next;
            size_t S = temp->size;

            struct allocated *Block = (struct allocated*)(void*)current;
            split(Block,S,Size,temp);

            count++;
            flag =1;

             result = ((void*)Block + sizeof(struct allocated));
            
            
        
        }

        else if((current == start) && (current->size > Size+sizeof(struct allocated)) && ((int)current->size > 0)){

            struct free *temp = current;
            struct free *temp1= current->next;
            size_t S = temp->size;

            struct allocated *Block = (struct allocated*)(void*)current;
            split(Block,S,Size,temp);

            count++;
            flag =1;

             result = ((void*)Block + sizeof(struct allocated));
           

        }

        else  if((current > start) && (current->size == Size+sizeof(struct allocated)) && ((int)current->size > 0)){

            struct free *temp = current;
            struct free *temp1= current->next;
            size_t S = temp->size;

            struct allocated *Block = (struct allocated*)(void*)current;
            split(Block,S,Size,temp);

            count++;
            flag =1;

             result = ((void*)Block + sizeof(struct allocated));
            

        }

        else  if((current > start) && (current->size > Size+sizeof(struct allocated)) && ((int)current->size > 0)){
               struct free *temp = current;
            struct free *temp1= current->next;
            size_t S = temp->size;

            struct allocated *Block = (struct allocated*)(void*)current;
            split(Block,S,Size,temp);

            count ++;
            flag =1;

             result = ((void*)Block + sizeof(struct allocated));
           
            
        }
        }
     

}

if(flag == 0){
    result = NULL;
    printf("NOT ENOUGH MEMORY LEFT!!!\n\n\n");

}
        return result;

}


void merge(){
   
     struct free *current,*prev;
    current=start;
    
    while((current->next)!=end){
        if(((void*)current+(current->size)) == (void*)(current->next)){
        current->size=(current->next->size)+sizeof(struct free);
        current->next=current->next->next;
        printf("MERGED\n\n");
        }

  prev=current;
  current=current->next;

 }

}




void MyFree(void*ptr){
      struct allocated *temp1;
         size_t S ;

         
    if(((void*)memory <= (void*)ptr) && ((void*)temp1 < (void*)end)){
        temp1 = (void*)ptr -sizeof(struct allocated);
        S = temp1->size;

       if((int)(((struct garbage*)((void*)temp1+S+sizeof(struct allocated)))->check) == 1){
            S =S + (size_t)(int)(((struct garbage*)((void*)temp1+S+sizeof(struct allocated)))->size);
            printf("GARBAGE MEMORY ADDED\n");
            
        }
        count--;
        if(((void*)temp1 < (void*)start) && (start != end)){

        
            struct free *temp =start;
            start = (void*)temp1;
            start->next = temp;
            start->size = S+sizeof(struct allocated);

            printf("FREED\n\n\n");
           
        }

        else  if(((void*)temp1 < (void*)start) && (start = end)){
         
        start = (void*)temp1;
        start->size = S+sizeof(struct allocated);
        start->next = end;
         printf("FREED\n\n\n");
       
        
       
        }

        else if(((void*)start < (void*)temp1 ) ){

            struct free *current = start;
            struct free *prev;
            while(((void*)current->next < (void*)temp1) && (current->next != end)){
                prev = current;
                current = current->next;
            }
            struct free *new = (void*)temp1;
            new->size=S+sizeof(struct allocated);
            struct free *temp2=current->next;
            current->next =new;
            new->next = temp2;
             printf("FREED\n\n\n");
        }

       merge();
    }
    else{
        printf("INVALID POINTER!!!!!!\n\n\n");
    }
}
