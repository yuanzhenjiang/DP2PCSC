#include "friend.h"
void init_friend_name_addr()
{
	FILE *friend_address_file;
	friend_address_file = fopen(FRIEND_ADDRESS_FILE,"r");
	InitQueue(&name_address, sizeof(struct friend_name_addr *), sizeof(struct friend_name_addr));
	char line_data[LINE_LENGTH] = {0};
	while(fgets(line_data,LINE_LENGTH,friend_address_file) != NULL){
		char *friend_name;
		char *friend_address;
		friend_name = strtok(line_data, "@");
		friend_address = strtok(NULL, "\n");
		if(friend_name == NULL || friend_address == NULL)
			continue;
		enqueue_friend_name_addr(&name_address, friend_name, friend_address);
		memset(line_data, 0, sizeof(line_data));		
	}
	fclose(friend_address_file);
}

void enqueue_friend_name_addr(LinkQueue *queue, char *friend_name,char *friend_address)
{
	struct friend_name_addr *fna;
	fna = (struct friend_name_addr *)malloc(sizeof(struct friend_name_addr));
	memset(fna, 0, sizeof(struct friend_name_addr));
	fna->friend_name = (char *)malloc((strlen(friend_name) + 1) * sizeof(char));
	fna->friend_address = (char *)malloc((strlen(friend_address) + 1) * sizeof(char));
	memset(fna->friend_name, 0, strlen(friend_name) * sizeof(char));
	memset(fna->friend_address, 0, strlen(friend_address) * sizeof(char));
	strcpy(fna->friend_name, friend_name);
	strcpy(fna->friend_address, friend_address);
	EnQueue(queue, (void *)fna);
	free(fna);	
}

void dequeue_friend_name_addr(LinkQueue *queue)
{
	struct friend_name_addr *fna;
	fna = (struct friend_name_addr *)malloc(sizeof(struct friend_name_addr));	
	memset(fna, 0, sizeof(struct friend_name_addr));	
	DeQueue(queue, (void *)fna);
	free(fna->friend_name);
	free(fna->friend_address);
	free(fna);
}

void destory_friend_name_addr(LinkQueue *queue)
{
	while(QueueLength(queue) != 0){
		dequeue_friend_name_addr(queue);
	}
	DestoryQueue(queue);
}

int get_friend_address(LinkQueue *name_address_queue, char *friend_name, char *friend_ip)
{
	QNode *p = name_address_queue->front;
	while((p = p->next)){
		printf("[get_friend_address]%s %s\n",((struct friend_name_addr *)p->pointer)->friend_name,((struct friend_name_addr *)p->pointer)->friend_address);
		if (!strcmp(friend_name, ((struct friend_name_addr *)p->pointer)->friend_name)) {
			strcpy(friend_ip, ((struct friend_name_addr *)p->pointer)->friend_address);
			return OK;
		}
	}
	return ERROR;		
}

int get_friend_name(LinkQueue *name_address_queue, char *friend_ip, char *friend_name)
{
	QNode *p = name_address_queue->front;
	while((p = p->next)){
		printf("[get_friend_name]%s %s\n",((struct friend_name_addr *)p->pointer)->friend_name,((struct friend_name_addr *)p->pointer)->friend_address);
		if (!strcmp(friend_ip, ((struct friend_name_addr *)p->pointer)->friend_address)) {
			strcpy(friend_name, ((struct friend_name_addr *)p->pointer)->friend_name);
			return OK;
		}
	}
	return ERROR;		
}

int get_friend_name_length(LinkQueue *name_address_queue, char *friend_ip)
{
	QNode *p = name_address_queue->front;
	while((p = p->next)){
		if (!strcmp(friend_ip, ((struct friend_name_addr *)p->pointer)->friend_address)) {
			return strlen(((struct friend_name_addr *)p->pointer)->friend_name);
		}
	}
	return 0;
}
/*void print_name_addr(LinkQueue *queue)*/
/*{*/
/*	QNode *p = queue->front;*/
/*	*/
/*	//printf("[PTR]%p\n",p);*/
/*	*/
/*	while((p = p->next)){*/
/*		*/
/*		printf("[look!]%s %s\n",((struct friend_name_addr *)p->pointer)->friend_name,((struct friend_name_addr *)p->pointer)->friend_address);*/
/*	}*/
/*}*/

int init_connector()
{
	return InitQueue(&connectors, sizeof(struct friend *), sizeof(struct friend));
}

/*int add_connector(LinkQueue *friend_queue, struct friend *friendinfo)*/
/*{*/
/*	return EnQueue(friend_queue, friendinfo);*/
/*}*/

int enqueue_connector(LinkQueue *friend_queue, char *friend_name, pthread_t friend_thread_id, socket_fd friend_socket_fd)
{
	struct friend *connector;
	connector = (struct friend *)malloc(sizeof(struct friend));
	memset(connector, 0, sizeof(struct friend));
	connector->friend_name = (char *)malloc((strlen(friend_name) + 1) * sizeof(char));
	memset(connector->friend_name, 0, (strlen(friend_name) + 1) * sizeof(char));
	strcpy(connector->friend_name, friend_name);
	connector->friend_thread_id = friend_thread_id;
	connector->friend_socket_fd = friend_socket_fd;
	int result = EnQueue(friend_queue, (void *)connector);
	free(connector);
	return result;	
}

int dequeue_connector(LinkQueue *friend_queue)
{
	struct friend *connector;
	connector = (struct friend *)malloc(sizeof(struct friend));		
	memset(connector, 0, sizeof(struct friend));
	int result = DeQueue(friend_queue, (void *)connector);
	free(connector->friend_name);
	free(connector);
	return result;
}

/*int get_connector_socket_fd_by_name(LinkQueue *friend_queue, char *friend_name){*/
/*	QNode *p = friend_queue->front;*/
/*	while((p = p->next)){*/
/*		if (!strcmp(friend_name, ((struct friend *)p->pointer)->friend_name)) {*/
/*			return ((struct friend *)p->pointer)->friend_socket_fd;*/
/*		}*/
/*	}*/
/*	return 0;*/
/*}*/

int find_connector_by_name(LinkQueue *friend_queue, char *friend_name, struct friend *friend_find)
{
	QNode *p = friend_queue->front;
	while((p = p->next)){
		if (!strcmp(friend_name, ((struct friend *)p->pointer)->friend_name)) {
			memcpy(friend_find, p->pointer, sizeof(struct friend));
			return OK;
		}
	}
	return ERROR;
}

int find_connector_by_threadid(LinkQueue *friend_queue, pthread_t friend_thread_id, struct friend *friend_val)
{
	QNode *p = friend_queue->front;
	while((p = p->next)){
		if (!memcmp(&friend_thread_id, &((struct friend *)p->pointer)->friend_thread_id, sizeof(pthread_t))) {
			memcpy(friend_val, p->pointer, sizeof(struct friend));
			return OK;
		}
	}
	return ERROR;
}


int remove_connector(LinkQueue *friend_queue, char *friend_name)
{// TODO this function doesn't work well
	print_connector(friend_queue);
	QNode *p = friend_queue->front;
	QNode *before = p;
	while((p = p->next)){
		printf("[\telement]%s\n",((struct friend *)p->pointer)->friend_name);
		printf("[\telement cmp]%d\n",strcmp(friend_name, ((struct friend *)p->pointer)->friend_name));
		if (!strcmp(friend_name, ((struct friend *)p->pointer)->friend_name)) {
			before->next = p->next;
			printf("[element for remove]%s\n",((struct friend *)p->pointer)->friend_name);
			free(((struct friend *)p->pointer)->friend_name);
			free(p->pointer);
			free(p);
			return OK;
		}
		before = p;
	}
	print_connector(friend_queue);
	return ERROR;
}

void destory_connector(LinkQueue *friend_queue)
{
	while(QueueLength(friend_queue) != 0){
		dequeue_connector(friend_queue);
	}
	DestoryQueue(friend_queue);
}

void print_connector(LinkQueue *queue)
{
	QNode *p = queue->front;
	
	//printf("[PTR]%p\n",p);
	//printf("[print connectors]\n");
	while((p = p->next)){
		
		;//printf("[elements]%s     sockfd = %d\n",((struct friend *)p->pointer)->friend_name,((struct friend *)p->pointer)->friend_socket_fd);
	}
	//printf("[print connectors end]\n");
}
