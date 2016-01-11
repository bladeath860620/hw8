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
	int address;
	int length;
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

ip *newnode(ip *head, unsigned int adr, unsigned int len)
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
	while(temp -> next != NULL)
	{
		if((temp -> next) -> length < len)
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

int main()
{
	int i;
	char temp[20];
	unsigned int adr[4], length;
	unsigned int int_adr = 0;
	ip *tab1[(int)pow(2,8)];
	ip *tab2[(int)pow(2,12)];
	ip *tab3[(int)pow(2,12)];
	FILE *k400, *result, *search, *insert, *delete;
	k400 = fopen("IPv4_400k.txt", "r");
	result = fopen("result.txt", "w");
	search = fopen("IPv4_search.txt", "r");
	insert = fopen("IPv4_insert.txt", "r");
	delete = fopen("IPv4_delete.txt", "r");

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
	while(fgets(temp, 20, k400) != NULL)
	{
		sscanf(temp, "%u.%u.%u.%u/%u", &adr[0], &adr[1], &adr[2], &adr[3], &length);
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
	//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^create^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
	while(fgets(temp, 20, search) != NULL)
	{
		sscanf(temp, "%u.%u.%u.%u", &adr[0], &adr[1], &adr[2], &adr[3]);
	}
	fclose(k400);
	fclose(result);
	fclose(search);
	fclose(insert);
	fclose(delete);
}