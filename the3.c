#include "the3.h"
#include <stdio.h>
#include <stdlib.h>


/* Helper */

int compare_apartment_names(char* name1, char* name2)
{
    int i=0;
    int result = 1;
    while(name1[i]!='\0' && name2[i]!='\0')
    {
        if(name1[i]!=name2[i])
        {
            result = 0;
            break;
        }
        i++;
    }
    return result;
}


/* Functions */

Apartment* add_apartment(Apartment* head, int index, char* apartment_name, int max_bandwidth)
{

    int i;
    Apartment *curr, *prev;
    Apartment* new;

    new = (Apartment*) malloc(sizeof(Apartment));

    new->max_bandwidth = max_bandwidth;
    new->name = apartment_name;
    new->flat_list = NULL;

    if(head == NULL)  
    {
	new->next = new;
        return new;
    }
   
    if(index == 0)
    {
	new->next = head;
	curr = head;
	while(curr->next != head)
	{
	    curr = curr->next;
	}
	curr->next = new;
	return new;
    }

    prev = NULL;
    curr = head;

    for(i=0; i<index; i++)
    {
       prev = curr;
       curr = curr->next;
    }

    prev->next = new;
    new->next = curr;

    return head;
}


void add_flat(Apartment* head, char* apartment_name, int index, int flat_id, int initial_bandwidth)
{
    int i;
    Apartment *curr;
    Flat *new, *curr_flat, *prev_flat;
    int bandwidth_sum = 0;
    int maximum_possible_bd = 0;

    curr = head;

    new = (Flat*) malloc(sizeof(Flat));
    new->id = flat_id;
    new->is_empty = 0;

    while(!compare_apartment_names(curr->name,apartment_name))
    {
        curr = curr->next;
    }



    curr_flat = curr->flat_list;
    while(curr_flat->next != NULL)
    {
        bandwidth_sum += (curr_flat->initial_bandwidth);
        curr_flat = curr_flat->next; 
    }
    bandwidth_sum += (curr_flat->initial_bandwidth);
    


    maximum_possible_bd =(( curr->max_bandwidth) - bandwidth_sum);

    if(initial_bandwidth < maximum_possible_bd )
    {
	 maximum_possible_bd = initial_bandwidth;
    }

    new->initial_bandwidth = maximum_possible_bd;


    prev_flat = NULL;
    curr_flat = curr->flat_list;

    if(index==0)
    {
	new->next = curr_flat;
	curr_flat->prev = new;
	curr->flat_list = new;
    }
    else
    {
   	for(i=0; i < index; i++)
   	{
            prev_flat = curr_flat;
            curr_flat = curr_flat->next;
	}

   	prev_flat->next = new;
	new->prev = prev_flat;
   	new->next = curr_flat;
	curr_flat->prev = new;
    }
}


Apartment* remove_apartment(Apartment* head, char* apartment_name)
{
    Apartment *curr, *prev, *start;
    
    prev = NULL;
    curr = head;
    start = head;

    if(curr->next == head)
    {
   	free(curr->flat_list);
	free(curr);

        return NULL;
    }
    
    if(compare_apartment_names(curr->name,apartment_name))
    {
	head->next = start;
	curr = head;
	while(curr->next != head)
	{
	    curr = curr->next;
	}
	curr->next=start;
        free(head->flat_list);
        free(head);
	return start;
   }
	

    while(!compare_apartment_names(curr->name,apartment_name))
    {
	prev = curr;
	curr = curr->next;
    }

    prev->next = curr->next;

    free(curr->flat_list);
    free(curr);

    return head;
}

void make_flat_empty(Apartment* head, char* apartment_name, int flat_id)
{
    Apartment *curr;
    Flat *curr_flat;

    curr = head;

    while(!compare_apartment_names(curr->name,apartment_name))
    {
       curr = curr->next;
    }

    curr_flat = curr->flat_list;

    while(curr_flat->id != flat_id)
    {
        curr_flat = curr_flat->next;
    }

    curr_flat->initial_bandwidth = 0;
    curr_flat->is_empty = 1;
}

int find_sum_of_max_bandwidths(Apartment* head)
{
    int max_bd_sum = 0;
    Apartment* start;

    start = head;

    if(head == NULL)
    {
        return 0;
    }

    while(head->next != start)
    {
        max_bd_sum += (head->max_bandwidth);
        head = head->next;
    }
    max_bd_sum += (head->max_bandwidth);

    return max_bd_sum;
}

Apartment* merge_two_apartments(Apartment* head, char* apartment_name_1, char* apartment_name_2)
{
    Apartment *curr, *prev;
    Apartment *first, *second;
    Flat *curr_flat;

    curr = head;

    while(!compare_apartment_names(curr->name,apartment_name_1))
    {
        curr = curr->next;
    }
    first = curr;


    curr = head;
    prev = NULL;

    while(!compare_apartment_names(curr->name,apartment_name_2))
    {
        prev = curr;
        curr = curr->next;
    }
    second = curr;

    if(first->flat_list == NULL)
    {
        if(second->flat_list == NULL)
        {
	    prev->next = second->next;
            return head;
        }
        else
        {
            first->flat_list = second->flat_list;
            second->flat_list = NULL;
	    prev->next = second->next;
            return head;
        }
    }
    else
    {
        if(second->flat_list == NULL)
        {
	    prev->next = second->next;
            return head;
        }
        else
        {
            first->max_bandwidth = (second->max_bandwidth + first->max_bandwidth);

            curr_flat = first->flat_list;

            while(curr_flat->next != NULL)
            {
                curr_flat = curr_flat->next;
            }
	    curr_flat->next = second->flat_list;
    	    second->flat_list->prev = curr_flat;
	    prev->next = curr->next;
            return head;
        }
    }
}

void relocate_flats_to_same_apartment(Apartment* head, char* new_apartment_name, int flat_id_to_shift, int* flat_id_list)
{
    int i=0;
    Apartment *curr, *new_apartment;
    Flat *curr_flat, *new_curr, *new_prev, *temp;
    int curr_id;

    curr = head;
    while(!compare_apartment_names(curr->name,new_apartment_name))
    {
        curr = curr->next;
    }
    new_apartment = curr;

    new_prev = NULL;
    new_curr = new_apartment->flat_list;
    while(new_curr->id != flat_id_to_shift)
    {
        new_prev = new_curr;
        new_curr = new_curr->next;
    }

    curr = head;
    while(flat_id_list[i]!=0)
    {
	curr_id = flat_id_list[i];
        while(curr->next != head)
        {
	    if(compare_apartment_names(curr->name,new_apartment_name))
	    {
		curr = curr->next; 
		continue;
	    }
            curr_flat = curr->flat_list;
            while(curr_flat != NULL && curr_flat->next != NULL)
            {
		temp = curr_flat->next;
                if(curr_id == curr_flat->id)
                {
                    curr_flat->next = new_curr;
		    new_prev->next = curr_flat;
                    new_curr->prev = curr_flat;
		    curr_flat->prev->next = NULL;
                    curr_flat->prev = new_prev;

		    curr->max_bandwidth -= curr_flat->initial_bandwidth;
		    new_apartment->max_bandwidth += curr_flat->initial_bandwidth;
                    
                    new_prev =curr_flat;
                }
                curr_flat = temp;
            }	
            curr = curr->next; 
        }
	i++;
    }
}