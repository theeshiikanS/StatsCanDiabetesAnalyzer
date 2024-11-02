/*
Program: Diabetes Statistic Reader and Plotter
Author(s): Darren Sin, Ryan Mah, Kayanan Gnanakumar, Theeshiikan Shanmuganathan
Student Number(s): 501167165, 501186906, 501167212, 501157664
Date: April 2nd, 2023
Course: CPS188

Description: This program is the diabetes statistic read and plotter. It will read from a diabetes data file taken from
Statistics Canada and store it into variables of different data types. The program will then use the data in order to calculate
and draw conclusions from the data. These calculations include averages based on a specific area, group, and year. It will be
displayed into the terminal, allowing the user to see it. Additionally, the program will take the data and generate a GNU
plot to display a visualization of said data.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


//Intiailizing and setting global vairables to be used throughout the entire program
int size  = 0;
char *places[] = {"Canada (excluding territories)", "Quebec", "Ontario", "Alberta", "British Columbia", "\0"};

//Function to remove the quotations
void removeQuotations(char str[]) {

	//Finding the size of the inputted stirng
	int size = strlen(str);
	
	//Moving all the parts of the string 1 index to the left 
	for (int i = 0; i < size - 2; i++) {
		str[i] = str[i + 1];
		}

	//Replacing the last quotation mark with \0 to make it NULL
	str[size - 2] = '\0';
	
}

//Function to covert strings into a number to be used in function the program
double stringToNum(char str[]) {
	
  removeQuotations(str);

  return atof(str);         //Convert string to double and return

}

//Function to caluclate the provincial averages
void provincialAvg(char geo[size][32], double valueNum[size]) {

	//Declaring variables
	double location[4];
	double can = 0;
	int counter[5];
	int cc = 0;
  
	//intializing the arrays
  	for (int i = 0; i < 5; i++) {
		counter[i] = 0;
		location[i] = 0;
	}

	int count = 0;      //Counter
    
    //Loop through each province and add the value to the total of the current province
	for (int  j = 1; j < 5; j++) {
		for (int i = 0; i < size; i++) {
			if (strcmp(geo[i], places[j]) == 0) { 
				location[count] += valueNum[i];
				if (valueNum[i] != 0) {
					counter[count]++;       //Increment total amount of "cases"
				}
			}
		}
		count++;        //Increment count for next province
	}
	
    //Loop through all values of the National value and add to total
	for (int i = 0; i < size; i++) {
		if (strcmp(geo[i], places[0]) == 0) {
			can = can + valueNum[i];
			if (valueNum[i] != 0) {
				cc++;
			}
		}
	}

	double avg[5];

    //Calculate the average based on the total value and total number of cases
    //and add to the average array
	for (int i = 0; i < 5; i++) {
		avg[i] = location[i] / counter[i];
	}
	
    //National average
	double cavg = can/cc;

	double temp;
	char *strtemp;
	int j;
	
	//Display the provincial averages
	printf("\nPROVINCIAL AVERAGES: \n");

	for (int i = 0; i < 4; i++) {
		printf("%s: %.2lf%%\n", places[i + 1], avg[i]);
	}

	char *places2[] = {"Quebec", "Ontario", "Alberta", "British Columbia", "\0"};

    //Sort the provincial averages from least to greatest
	for (int i = 1; i < 4; i++) {
		temp = avg[i];
		strtemp = places2[i];
		j = i - 1;

		while (j >= 0 && avg[j] > temp) {
			places2[j + 1] = places2[j];
			avg[j + 1] = avg[j];
			--j;
		}
		places2[j + 1] = strtemp;
		avg[j + 1] = temp;	
	}
	
    //Display the national average
	printf("\nNATIONAL AVERAGE: \n");
	printf("Canada (excluding territories): %.2lf%%\n\n", cavg);
	
    //Display the provinces with the lowest and highest percentage of diabetes
	printf("The province with the lowest percentage of diabetes is: %s (%.2lf%%)\n", places2[0], avg[0]);   
	printf("The province with the highest percentage of diabetes is: %s (%.2lf%%)\n\n", places2[3], avg[3]); 
	
	for (int i = 0; i < 4; i++) {
		if (avg[i] > cavg) {
			printf("%s is above the national average (%.2lf%% > %.2lf%%).\n", places2[i], avg[i], cavg);
		}
		if (avg[i] < cavg) {
			printf("%s is below the national average (%.2lf%% < %.2lf%%).\n", places2[i], avg[i], cavg);
		}
	}
}

//Function to generate a GNU plot with data from the statistics sheet
void gnuPlot(double avg[35]) {
	
    //Open plot file for writing
	FILE* gnu2 = fopen ("plot2.txt", "w");      //Age group average for national
  
    //Write headings and average into the file
    fprintf(gnu2, "Age: Avg:\n");
    fprintf(gnu2, "\"35 to 49 years\" %.2lf\n\"50 to 64 years\" %.2lf\n\"65 years and over\" %.2lf", avg[0], avg[1], avg[2]);
  
    fclose(gnu2);   //Close file
	
}

void gnuPlot2(double avg[35]) {
	
    //Open plot file for writing
	FILE* gnu = fopen ("plot.txt", "w");        //Provicial and yearly average
	
    //Initialize array
	int years[8] = {2015, 2016, 2017, 2018, 2019, 2020, 2021, '\0'};
	
    //Write headings into the plot file
	fprintf(gnu, "YEARS\t");
	fprintf(gnu, "Canada (excluding territories)\tQuebec\tOntario\tAlberta\tBritish Columbia");
	fprintf(gnu, "\n%d", years[0]);

    //Write the average of each year and its corresponding averages in one line onto the file
	for (int i = 0; i < 7; i++) {
		fprintf(gnu, "\t%.2lf\t%.2lf\t%.2lf\t%.2lf\t%.2lf\n", avg[i], avg[i + 7], avg[i + 14], avg[i + 21], avg[i + 28]);
		if (years[i + 1] != '\0') {
			fprintf(gnu, "%d", years[i + 1]);
		} else if (years[i+1] == '\0') {
			break;
		}
	}

	fclose(gnu);    //Close file	
}


//Function to calculate the yearly averages
void yearAvg(int dateNum[size], char geo[size][32], double valueNum[size]) {

	printf("\nYEARLY AVERAGES: \n");
	
    //Intialize arrays
	double total[35];
	double count[35];
	
	char *places[] = {"Canada (excluding territories)", "Quebec", "Ontario", "Alberta", "British Columbia", "\0"};
	
	char *places2[35];
	int q = 0;
	int w = 0;
	
    //Set province for corresponding elements in array (Total of 6 years)
	for (int i = 0; i < 35; i++) {
		places2[i] = places[q];
		w++;
		if (w > 6) {
			w = 0;
			q++;
		}
	}
	
    //Set all values to 0 in total and count array
	for (int i = 0; i < 35; i++) {
		total[i] = count[i] = 0;
	}
	
    //Intialize variables
	double avg[35];
	int t = 0;
	int counter = 0;
	int year  = 2015;
	
    //For each province and each year, add the value of diabetes to the total sum
	while (t < 5) {
		for (int j = 0; j < 7; j++) {
			for (int i = 0; i < size; i++) {
				if (dateNum[i] == (year+j) && strcmp(geo[i], places[t]) == 0) {
					total[counter] = total[counter] + valueNum[i];
					if (total[counter] != (total[counter] - valueNum[i])) {
						count[counter]++;
					}
				}
			}
			counter++;
		}
		t++;
	}
	
	//Calculate the average based on the total value and number of "cases"
	for (int i = 0; i < counter; i++) {
		avg[i] = total[i] / count[i];
	}
	
	//Declaring variables that will be used to sort the array
	year = 2015;
	t = 0;
	int a = 0;
	int b = 1;
	int year1[35];
	
	//initializing the year array so that each year lines up with its corresponding average
	printf("%s:\n", places[0]);
	while (t<counter) {		
			year1[t] = year;
			year++;
			if (year > 2021) {
				year = 2015;
			}
			t++;
		}
	
	//Sending the average value to the GNU plot data file function	
	gnuPlot2(avg);
		
	t=0;		
	
	//Printing out the averages with the correct correspondning averages
	while (t < counter) {		
		printf("%d: %.2lf%%\n", year1[t], avg[t]);
		a++;
		if (a > 6) {
			if (b < 5) {
				printf("\n%s:\n", places[b]);
				b++;
				a=0;
			}
		}
		t++;
	}

	//Declaring variables needed to sort the average array
	double temp;
	int temp2;
	char *strtemp;
	int j;

	//Sorting the average arrya from least to greatest
	for (int i = 1; i < 35; i++) {
		temp = avg[i];
		temp2 = year1[i];
		strtemp = places2[i];
		j = i - 1;

		while (j >= 0 && avg[j] > temp) {
			places2[j + 1] = places2[j];
			avg[j + 1] = avg[j];
			year1[j + 1] = year1[j];
			--j;
		}
		places2[j + 1] = strtemp;
		avg[j + 1] = temp;
		year1[j + 1] = temp2;
	}
	
	//Checking to see if there is any ties for lowest averag values
	for (int i = 0; i < 35; i++) {
		if (avg[0] == avg[i + 1]) {
			printf("\nOne of the provinces with the lowest percent of diabetes is %s in the year %d with a percentage of %.2lf%%.", places2[i], year1[i],avg[i]);
		} else {
			break;
		}
	}
	
	//If there is no tie in the values, the lwoest value will be printed out with the corresponding year and province
	if (avg[0] != avg[1]) {
		printf("\nThe province with the lowest percent of diabetes is %s in the year %d with a percentage of %.2lf%%.", places2[0], year1[0],avg[0]);
	}
	
	//Checking to see if there is any ties for highest average values
	for (int i = 34; i > 0; i--) {
		if (avg[34] == avg[i - 1]) {
			printf("\nOne of the provinces with the highest percent of diabetes is %s in the year %d with a percentage of %.2lf%%.", places2[i], year1[i],avg[i]);
		} else {
			break;
		}
	}
	
	//If there is no tie in the values, the highest value will be printed out with the corresponding year and province
	if (avg[34] != avg[33]) {
		printf("\nThe province with the highest percent of diabetes is %s in the year %d with a percentage of %.2lf%%.\n", places2[34], year1[34],avg[34]);
	}
}

//Fucntion to get the average age values		
void ageAvg(char geo[size][32], char ageGroup[size][19], double valueNum[size]) {		
	printf("\nAGE AVERAGES: \n");
	
	//Variables to be used in the function
	double total[15];
	double count[15];
	double avg[15];
	
	//declaring arrays to compare
	char *places[] = {"Canada (excluding territories)", "Quebec", "Ontario", "Alberta", "British Columbia", "\0"};
	char *age[] = {"35 to 49 years", "50 to 64 years", "65 years and over", "\0"};
	
	//Initalizing the total and  coutner arrays to 0
	for (int i = 0; i < 15; i++) {
		total[i] = count[i] = 0;
	}
	
	//declaring variables
	int t = 0;
	int counter  = 0;
	
	//While loop that finds that changes the place comparision
	while (t < 5) { //For loop to change the age comparison
		for (int j = 0; j < 3; j++) { //For loop to compare all the values sread from file
			for (int i = 0; i < size; i++) {
				if (strcmp(ageGroup[i], age[j]) == 0 && strcmp(geo[i], places[t]) == 0) {
					total[counter] = total[counter] +  valueNum[i]; //Adding to the sum if the conditions are met
					if (total[counter] != (total[counter] - valueNum[i])) {
						count[counter]++; //Adding to the counter only if there is a value present
					}
				}
			}
			counter++; //Moving on to the next array element
		}
		t++; //Moving onto the next place
	}
	
	//For loop to  calulate all of the age averages
	for (int i = 0; i < counter; i++) {
		avg[i] = total[i] / count[i];
	}
	 
	 //Sending averages to the gnu plot data creation funciton
	gnuPlot(avg);
	
	//declating variables
	int a = 0;
	int b = 0;
	int c = 0;
	
	t = 0;
	
	//Pringing the first place in the array
	printf("%s: \n", places[0]);
	
	//Loop that will output each age group average with it corresponidng age group and place
	while (t < counter) {
		printf("%s: %.2lf%%\n", age[c], avg[t]);
		//Counters that will change the age group name every time but loops back to the first group every 3 averages
		a++;
		c++;
		if (c > 2) {
			c = 0;
		}
		if (a > 2) {
			if (b < 4) {
				//Condition that ensures that the places output will change every 3 averages
				b++;
				printf("\n%s: \n", places[b]);
				a = 0;
			}
		}
		t++; //Counter to change the average value
	} 	
}

//Main method
int main(int argc, char **argv) {

    //Open data to determine the lines of data
	FILE *data = fopen("statscan_diabetes.csv", "r");
    
	char buffer1[1024];

	while (fgets(buffer1, 1024, data) != NULL) {
		size++;
	}
	
	fclose(data);       //Close data file
  
    //Intialize arrays
    //These arrays consist of string arrays and will store the strings that are
    //read from the csv file
	char date[size][6];
	char geo[size][32];
	char nullArr[size][1];
	char ageGroup[size][19];
	char sex[size][7];
	char value[size][5];

    //These arrays will store the numerical values in the form of int and double values
    int dateNum[size];
    double valueNum[size];
	
    //Open data
	FILE *dataFile = fopen("statscan_diabetes.csv", "r");

	char buffer[1024];

	fgets(buffer, 1024, dataFile);

	int i = 0;      //Counter for the lines of data

    //Loop through the file and read until there are no lines left
	while (fgets(buffer, 1024, dataFile) != NULL) {
        
        //Account for the extra blank lines at the bottom of the csv file that 
        //aren't considered null. When it reaches to that point,
		if (strcmp(buffer, "") == 10) {
			break;      //Break loop
		}

        //Otherwise, split the buffer array into separate strings and store them into
        //the corresponding arrays
		else {
			sscanf(buffer,
				"%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,]"
				",%[^,],%[^,],%[^,]",
				date[i], geo[i], nullArr[i], ageGroup[i], sex[i], nullArr[i],
				nullArr[i], nullArr[i], nullArr[i], nullArr[i], nullArr[i],
				nullArr[i], nullArr[i], value[i]);

        //Convert string to number
		dateNum[i] = stringToNum(date[i]);
		valueNum[i] = stringToNum(value[i]);
    
        //Remove the quotation marks from the string
		removeQuotations(geo[i]);
		removeQuotations(ageGroup[i]);
		removeQuotations(sex[i]);

		//printf("%d, %s, %s, %s, %.2f\n", dateNum[i], geo[i], ageGroup[i], sex[i], valueNum[i]);

		i++;        //Increment the counter for next line of data
		}

	}

	fclose(dataFile);       //Close data file
  
    //Call functions to calculate for average
  	provincialAvg(geo, valueNum);
	yearAvg(dateNum, geo, valueNum);
	ageAvg(geo, ageGroup, valueNum);

	return 0; // Exit

}