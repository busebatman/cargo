//Buse Batman - 150117011
//I made this project to simulate a simple cargo system for a firm.
#include <stdio.h>
#include <stdlib.h>
// For memset 
#include <string.h> 

//Defining constants
#define forbiddenItemsCount 6
#define itemMaxLength 50
#define maxCustomerCount 500
#define maxTransCount 730

//Creating forbiddenItems array */
char forbiddenItems[forbiddenItemsCount][itemMaxLength];
int customerNumber;
//Creating customers array
int customers[maxCustomerCount][2]; // 0 is customerNumber 1 is transactionCount
int totalCustomerCount = 0;
//Creating transactions array
double transactions[maxCustomerCount][maxTransCount];

//This function searches the customers array to find the given customerNumber and returns index of the customer if finds
int searchCustomer(int customerNumber){
	int i;
	for(i = 0; i < maxCustomerCount; i++){
		if (customers[i][0] == customerNumber) {
			return i;
		}
	}
	//Otherwise, the function returns -1
	return -1;
}

//This function updates the stringToConstruct array by extracting the substring between start and end indices
void reconstruct(char stringToConstruct[], int start, int end) {
	char reconstructedString[500];
	int len = strlen(stringToConstruct);
	int i;
	int j=0;
	for (i = 0; i < len; i++){
		if (i < start || i > end) {
			reconstructedString[j] = stringToConstruct[i];
			j++;
		}
	}
	reconstructedString[j] = NULL;
	strcpy(stringToConstruct, reconstructedString);
}

//This function determines the start and end index of str2 in str1, if it exists in str1
int searchWord(char str1[], char str2[]){
	int str1Len = strlen(str1);
	int str2Len = strlen(str2);
	
	int i;
	int countStartInd = 0;
	int wordStartInd = 0;
	//Searches from start to (str1Len - str2Len)
	for (i = 0; i <= str1Len - str2Len; i++) {
		int j;
		int isEqual = 1;
		for (j = 0; j < str2Len; j++) {
			if (str1[i + j] != str2[j]) {
				isEqual = 0;
				break;
			}
		}
		if (isEqual == 1) {
			wordStartInd = i;
			break;
		}
	}
	if (wordStartInd == 0) {
		return 0;
	}
	for (i = wordStartInd - 1; i >= 0; i--) {
		if (str1[i] != ' ') { // We found the countEndInd
			int j;
			for (j = i-1; j >= 0; j--) {
				if (str1[j] == ' ') {
					break;
				}
			}
			countStartInd = j+1;
			break;
		}
	}
	reconstruct(str1, countStartInd, wordStartInd + str2Len - 1);
	return 1;
}

//This function checks whether any of the forbiddenItems is contained in the content of the cargo or not
void removeForbiddens(char content[], int itemIndex){
	if (itemIndex < 0) {
		return;
	}
	if(searchWord(content, forbiddenItems[itemIndex]) == 1) {
		removeForbiddens(content, itemIndex);
	}
	else {
		removeForbiddens(content, itemIndex - 1);
	}
}

//This function separates each item from the string
void seperateItem(char wholeString[], int *itemCount, char item[]){
	int i=0;
	int countStartInd = 0;
	int countEndInd = 0;
	int wordStartInd = 0;
	int wordEndInd = 0;
	while (1) {
		if (wholeString[i] != ' ') { // start of count
			countStartInd = i;
			i++;
			while(1) {
				if (wholeString[i] == ' ') { // end of count + 1
					countEndInd = i - 1;
					i++;
					while(1) {
						if (wholeString[i] != ' ') { // start of word
							wordStartInd = i;
							i++;
							while(1) {
								if (wholeString[i] == ' ' || wholeString[i] == NULL) { // end of word + 1
									wordEndInd = i - 1;
									break;
								}
								i++;
							}
							break;
						}
						i++;
					}
					break;
				}
				i++;
			}
			break;
		}
		i++;
	}
	char itemCountStr[itemMaxLength];
	int j = 0;
	for (i = countStartInd; i <= countEndInd; i++) {
		itemCountStr[j] = wholeString[i];
		j++;
	}
	itemCountStr[j] = NULL;
	j = 0;
	for (i = countStartInd; i <= wordEndInd; i++) {
		item[j] = wholeString[i];
		j++;
	}
	item[j] = NULL;
	//Returns itemCount from character to integer
	*itemCount = atoi(itemCountStr);
	reconstruct(wholeString, countStartInd, wordEndInd);
}

//This function asks prices for each item and calculates the price	
void askPrice(char item[], int itemCount, double *money) {
	//If itemCount is a negative number, function gives an error message
	if (itemCount <= 0) {
		printf("Price for item %s will not be calculated because of having invalid number of items\n", item);
		return;
	}
	printf("enter the approximate value for one of %s\n", item);
	double price;
	scanf("%lf", &price);
	*money += itemCount * price;
	return;
}

//This function checks whether the content is empty or contains just spaces
int checkFinish(char stringToCheck[]){
	int len = strlen(stringToCheck);
	int i;
	for (i = 0; i<len; i++) {
		if (stringToCheck[i] != ' ' && stringToCheck != NULL) {
			return 0;
		}
	}
	return 1;
}

void getContent(char content[], double *money){
	//This recursive function first checks whether the content is empty or contains just spaces, again
	if (checkFinish(content) == 1) {
		return;
	}
	//Then it removes forbidden items if there exists any in the content of the cargo
	removeForbiddens(content, forbiddenItemsCount - 1);
	int itemCount = 0;
	char item[itemMaxLength];
	//Checks again after removing forbiddenItems
	if (checkFinish(content) == 1) {
		return;
	}
	//Separates each item from the string
	seperateItem(content, &itemCount, item);
	//Calls itself
	getContent(content, money);
	//Asks and calculates the price
	askPrice(item, itemCount, money);
}

//When a negative number is entered for customerNumber,function prints all customerNumbers,their totalTransactionCount price for each transaction
void printCustomers(){
	int i;
	for(i = 0; i < totalCustomerCount; i++){
		int user = customers[i][0];
		int cargoCount = customers[i][1];
		printf("User %d has sent %d cargos for price of each as follows\n", user, cargoCount);
		int j;
		for (j = 0; j < cargoCount; j++) {
			double price = transactions[i][j];
			printf("price for cargo %d -> %lf ", j + 1, price);
		}
		printf("\n\n");
	}
}

int main(void){
	//Setting all elements in arrays to 0
	memset(customers, 0, sizeof customers);
	memset(transactions, 0, sizeof transactions);
	printf("Hello Admin Please Enter the Forbidden Items: \n");
	int i;
	//Takes forbiddenItems from user
	for (i = 0; i < forbiddenItemsCount ; i++) {
		scanf("%s", &forbiddenItems[i]);
    }
    //Starting the system
	printf("Do you want to start the system? 1 --> Yes, 0 --> No:\n");
	int a;
	scanf("%d", &a);
	
	if (a==0) {
		exit(0);
	}
	
	printf("Welcome to Cargo Tracking System\n");
	
	while (1) {
		printf("Please Enter the Customer Number:\n");
		scanf("%d", &customerNumber);
		if (customerNumber < 0) {
			printCustomers();
			exit(0);
		}
		//Checks the customerNumber
		int customerInd = searchCustomer(customerNumber);
		if (customerInd == -1){
			//If couldn't find , adds it if there is enough capacity
			if (totalCustomerCount < maxCustomerCount) {
				customers[totalCustomerCount][0] = customerNumber;
				customerInd = totalCustomerCount;
				totalCustomerCount += 1;
			}
			else {
				printf("Maximum number of customers reached. Exiting\n");
				exit(0);
			}
		}	
		//Cleaning console
		fseek(stdin, 0, SEEK_END); 
		printf("Please Enter the Content of the Cargo :");
		char str[500];
		char c;
		//Reads the content of the cargo
		scanf("%c", &c);
		i = 0;
		while ( c != '\n' ) {
			str[i] = c;
			i++;
			scanf("%c", &c);
		}
		str[i] = NULL;
		double money = 0;
		getContent(str, &money);
		printf("price %lf\n", money);
		if (money > 0) {
			int numberOfTransaction = customers[customerInd][1]++;
			transactions[customerInd][numberOfTransaction] = money;
		}
	}
}
