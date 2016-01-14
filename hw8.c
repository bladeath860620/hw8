#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
//0.0.0.0(address)/length
/*unsigned long long int begin,end, result= 0;
begin=rdtsc();
// executing code you want to measure
end=rdtsc();
result = end-begin;*/
inline unsigned long long int rdtsc()
{
	unsigned long long int x;
	asm volatile ("rdtsc" : "=A" (x));
	return x;
} 

struct IP
{
	unsigned int address;
	unsigned int length;
	struct IP *next;
};
typedef struct IP ip;

unsigned int bin(unsigned int adr[])
{
	unsigned int bin_adr = 0;
	bin_adr |= (adr[0] << 24);
	bin_adr |= (adr[1] << 16);
	bin_adr |= (adr[2] << 8);
	bin_adr |= adr[3];	
	return bin_adr;
}

ip *newnode(ip *head, unsigned int adr, unsigned int len)//put ip under ling lists while doing sort at the same time()
{
	ip *temp = head;
	ip *node = malloc(sizeof(ip));
	node -> address = adr;
	node -> length = len;
	node -> next = NULL;
	if(head == NULL)
	{
		return node;
	}
	if(head -> length <= len)
	{
		node -> next = head;
		return node;
	}//
	while(temp -> next != NULL)
	{
		if((temp -> next) -> length <= len)
		{
			node -> next = temp -> next;
			temp -> next = node;
			return head;
		}
		else
		{
			temp = temp -> next;
		}
	}
	temp -> next = node;//
	return head;
}

unsigned int bin_to_int(unsigned int adr[], unsigned int length, unsigned int int_adr)
{
	if(length >= 8 && length <= 15)
	{
		return bin(adr) >> (32-8);
	}
	if(length >= 16 && length <= 32)
	{
		return bin(adr) >> (32-12);
	}
}

unsigned int bin_to_int1(unsigned int adr[])//rewrote two kinds of bin_to_int because no length is given while searching
{
		return bin(adr) >> (32-8);
}
unsigned int bin_to_int23(unsigned int adr[])
{
		return bin(adr) >> (32-12);
}

/*unsigned int compare_length(unsigned int len_mem[])
{
	unsigned int longest, compare;
	int i;
	longest = len_mem[0];
	compare = len_mem[1];
	if(compare > longest)
	{
		longest = len_mem[1];
		return longest;
	}
	else
	{
		return longest;
	}
}*/

ip *searching(ip *head, unsigned int goal/*, unsigned int len_mem[]*/)
{
	unsigned int origin_adr; 
	unsigned int target;
	int i = 0;
	int count = 0;
	ip *save;
	ip *temp = head;
	while(temp != NULL)
	{
		origin_adr = (temp -> address) >> (32 - (temp -> length));
		target = goal >> (32 - (temp -> length));
		if(origin_adr == target)
		{
			/*save = temp;
			len_mem[i] = temp -> length;
			if(len_mem[1] > len_mem[0])
			{
				len_mem[0] = len_mem[1];
			}
			i = 1;
			count++;
			printf("%d\n", count);*/
			return temp;
		}
		else
		{
			temp = temp -> next;
		}
	}
	/*if(len_mem[0] == 0)
	{
		return NULL;
	}
	else
	{
		save -> length = len_mem[0];
		return save;
	}*/
	return NULL;
}

void memory_to_ip(ip *memory, FILE *result)//turn memory to binary mode
{
	unsigned int adr[4];
	unsigned int length = memory -> length;
	unsigned int binary = memory -> address;
	adr[0] = binary >> 24;
	binary &= 0x00FFFFFF;
	adr[1] = binary >> 16;
	binary &= 0x0000FFFF;
	adr[2] = binary >> 8;
	binary &= 0x000000FF;
	adr[3] = binary;
	fprintf(result, "%u.%u.%u.%u/%u\n", adr[0], adr[1], adr[2], adr[3], length);
	return;
}

void link_list(ip *head, FILE *test)
{
	ip *temp = head;
	while(temp != NULL)
	{
		memory_to_ip(temp, test);
		temp = temp -> next;
	}
	return;
}

ip* del(ip *head, unsigned int adr, unsigned int len)
{
	ip *temp = head;
	ip *dele;
	if(head == NULL)
	{
		return head;
	}
	if((head -> length) == len && (head -> address) == adr)
	{
		dele = head;
		free(dele);
		head = head -> next;
		return head;
	}
	while(temp -> next != NULL)
	{
		if(((temp -> next) -> length) == len && ((temp -> next) -> address) == adr)
		{
			dele = temp -> next;
			free(dele);
			temp -> next = (temp -> next) -> next;
			return head;
		}
		else
		{
			temp = temp -> next;
		}
	}
	return head;
}

int main()
{
	int i;
	int locate_tab = 0;
	char string[30];
	unsigned int adr[4], length;
	unsigned int int_adr = 0;
	//unsigned int len_mem[20] = {0};
	ip *tab1[(int)pow(2,8)];
	ip *tab2[(int)pow(2,12)];
	ip *tab3[(int)pow(2,12)];
	FILE *k400, *result, *search, *insert, *delete, *test ,*search_csv, *insert_csv, *delete_csv;
	k400 = fopen("IPv4_400k.txt", "r");
	result = fopen("result.txt", "w");
	search = fopen("IPv4_search.txt", "r");
	insert = fopen("IPv4_insert.txt", "r");
	delete = fopen("IPv4_delete.txt", "r");
	test = fopen("test.txt", "w");
	search_csv = fopen("search.csv", "w");
	insert_csv = fopen("insert.csv", "w");
	delete_csv = fopen("delete.csv", "w");
	unsigned long long int begin,end;
	for(i = 0; i < pow(2,8); i++)
	{
		tab1[i] = NULL;
	}
	for(i = 0; i < pow(2,12); i++)
	{
		tab2[i] = NULL;
		tab3[i] = NULL;
	}
	//put all element in tab arrays to prevent *head from segmentation fault
	while(fgets(string, 30, k400) != NULL)
	{
		sscanf(string, "%u.%u.%u.%u/%u", &adr[0], &adr[1], &adr[2], &adr[3], &length);
		if(length >= 8 && length <= 15)
		{
			int_adr = bin_to_int(adr, length, int_adr);
			tab1[int_adr] = newnode(tab1[int_adr], bin(adr), length);
		}
		if(length >= 16 && length <= 24)
		{
			int_adr = bin_to_int(adr, length, int_adr);
			tab2[int_adr] = newnode(tab2[int_adr], bin(adr), length);
		}
		if(length >=25 && length <= 32)
		{
			int_adr = bin_to_int(adr, length, int_adr);
			tab3[int_adr] = newnode(tab3[int_adr], bin(adr), length);
		}
	} 
	/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^create^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
	for(i = 0; i < 1 << 8; i++)
	{
		link_list(tab1[i], test);
		fprintf(test, "%s", "=========================\n");
	}
	for(i = 0; i < 1 << 12; i++)
	{
		link_list(tab2[i], test);
		fprintf(test, "%s", "=========================\n");
	}
	for(i = 0; i < 1 << 12; i++)
	{
		link_list(tab3[i], test);
		fprintf(test, "%s", "=========================\n");
	}
	/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^test^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
	ip *memory;
	while(fgets(string, 30, search) != NULL)
	{
		begin=rdtsc();
		sscanf(string, "%u.%u.%u.%u", &adr[0], &adr[1], &adr[2], &adr[3]);
		int_adr = bin_to_int23(adr);
		memory = searching(tab3[int_adr], bin(adr)/*, len_mem*/);
		if(memory != NULL)
		{
			memory_to_ip(memory, result);
			locate_tab = 3;
		}

		int_adr = bin_to_int23(adr);
		memory = searching(tab2[int_adr], bin(adr)/*, len_mem*/);
		if(memory != NULL && locate_tab == 0)
		{
			memory_to_ip(memory, result);
			locate_tab = 2;
		}

		int_adr = bin_to_int1(adr);
		memory = searching(tab1[int_adr], bin(adr)/*, len_mem*/);
		if(memory != NULL && locate_tab == 0)
		{
			memory_to_ip(memory, result);
			locate_tab = 1;
		}
		if(locate_tab == 0)
		{
			fprintf(result, "%s", "0.0.0.0/0\n");
		}
		locate_tab = 0;
		end=rdtsc();
		fprintf(search_csv, "%llu\n", end-begin);
	}	
	/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^search^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
	while(fgets(string, 30, insert) != NULL)
	{
		begin=rdtsc();
		sscanf(string, "%u.%u.%u.%u/%u", &adr[0], &adr[1], &adr[2], &adr[3], &length);
		if(length >= 8 && length <= 15)
		{
			int_adr = bin_to_int(adr, length, int_adr);
			tab1[int_adr] = newnode(tab1[int_adr], bin(adr), length);
		}
		if(length >= 16 && length <= 24)
		{
			int_adr = bin_to_int(adr, length, int_adr);
			tab2[int_adr] = newnode(tab2[int_adr], bin(adr), length);
		}
		if(length >=25 && length <= 32)
		{
			int_adr = bin_to_int(adr, length, int_adr);
			tab3[int_adr] = newnode(tab3[int_adr], bin(adr), length);
		}
		end=rdtsc();
		fprintf(insert_csv, "%llu\n", end-begin);
	}
	/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^insert^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
	while(fgets(string, 30, delete) != NULL)
	{
		begin=rdtsc();
		sscanf(string, "%u.%u.%u.%u/%u", &adr[0], &adr[1], &adr[2], &adr[3], &length);
		if(length >= 8 && length <= 15)
		{
			int_adr = bin_to_int(adr, length, int_adr);
			tab1[int_adr] = del(tab1[int_adr], bin(adr), length);
		}
		if(length >= 16 && length <= 24)
		{
			int_adr = bin_to_int(adr, length, int_adr);
			tab2[int_adr] = del(tab2[int_adr], bin(adr), length);
		}
		if(length >=25 && length <= 32)
		{
			int_adr = bin_to_int(adr, length, int_adr);
			tab3[int_adr] = del(tab3[int_adr], bin(adr), length);
		}
		end=rdtsc();
		fprintf(delete_csv, "%llu\n", end-begin);
	} 
	/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^delete^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
	fclose(k400);
	fclose(result);
	fclose(search);
	fclose(insert);
	fclose(delete);
	fclose(test);
	fclose(search_csv);
	fclose(insert_csv);
	fclose(delete_csv);
}