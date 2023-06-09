#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

// 创建了一个结构体，用以存储人的属性
struct Person {
	char *name;
	int age;
	int height;
	int weight;
};

// 这个是新建一个人的资料
struct Person* Person_create(char *name, int age, int height, int weight)
{
	struct Person *who = malloc(sizeof(struct Person));
	assert(who != NULL);
	
	who->name = strdup(name);
	who->age = age;
	who->height = height;
	who->weight = weight;

	return who;
}

// 删除一个人的资料
void Person_destroy(struct Person* who)
{
	assert(who != NULL);
	
	// 释放掉分别申请的两块内存
	free(who->name);
	free(who);
}

// 打印一个人的资料
void Person_print(struct Person* who)
{
	printf("Name : %s\n",who->name);
	printf("\tAge: %d\n",who->age);
	printf("\theight: %d\n",who->height);
	printf("\tweight: %d\n",who->weight);
}

int main(int argc, char* argv[])
{
	// make two people structures
	struct Person* joe = Person_create(
			"Joe Alex", 32, 64, 140);

	struct Person* frank = Person_create(
			"Frank Blank", 20, 72, 180);

	// print them out and where they are in memory
	printf("Joe is at memory location %p:\n",joe);
	Person_print(joe);

	printf("Frank is at memory location %p:\n",frank);
	Person_print(frank);

	// make everyone age 20 years and print them again
	joe->age += 20;
	joe->height -=2;
	joe->weight += 40;
	Person_print(joe);

	frank->age += 20;
	frank->weight += 20;
	Person_print(frank);

	// destory them both so we clean up
	Person_destroy(joe);
	Person_destroy(frank);
	
	// try to transfer NULL to Person_destroy
	// Person_destroy(NULL);

	// try to transfer NULL to Person_print
	// Person_print(NULL);


	return 0;
}
