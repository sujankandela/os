#include <stdio.h>

//mergeUser will merge the arrays according to the increasing order of arrival time of repective processes
int mergeUser(int arrivalTime[], int serviceTime[], int remainingServiceTime[], int beg, int mid, int end) {
	int i = beg, j = mid + 1, k = 0;
	int tempArrival[end - beg + 1], tempService[end - beg + 1], tempRemaining[end - beg + 1];	

	while(i <= mid && j <= end) {
		if(arrivalTime[i] < arrivalTime[j]) {
			tempArrival[k] = arrivalTime[i];
			tempService[k] = serviceTime[i];
			tempRemaining[k] = remainingServiceTime[i];
			i++;
		}
		else {
			tempArrival[k] = arrivalTime[j];
			tempService[k] = serviceTime[j];
			tempRemaining[k] = remainingServiceTime[j];
			j++;
		}
		k++;
	}

	if(i > mid) {
		while(j <= end){
			tempArrival[k] = arrivalTime[j];
			tempService[k] = serviceTime[j];
			tempRemaining[k] = remainingServiceTime[j];
			j++; k++;
		}
	}
	else {
			tempArrival[k] = arrivalTime[i];
			tempService[k] = serviceTime[i];
			tempRemaining[k] = remainingServiceTime[i];
			i++; k++;
	}

	for(i = beg, k = 0; i <= end; i++, k++) {
		arrivalTime[i] = tempArrival[k];
		serviceTime[i] = tempService[k];
		remainingServiceTime[i] = tempRemaining[k];
	}

	return 0;
}

//mergesortUser will sort the arrays recursively
int mergesortUser(int arrivalTime[], int serviceTime[], int remainingServiceTime[], int beg, int end) {
	if(beg == end)
		return 0;
	int mid = (beg + end) / 2;
	mergesortUser(arrivalTime, serviceTime, remainingServiceTime, beg, mid);
	mergesortUser(arrivalTime, serviceTime, remainingServiceTime, mid + 1, end);
	mergeUser(arrivalTime, serviceTime, remainingServiceTime, beg, mid, end);

	return 0;
}

//getHighestPriority give the index of highest value in priority array
int getHighestPriority(int priority[], int serviceTime[], int remainingServiceTime[], int limit) {
	int highestPriority = 0, iter;
	for(iter = 1; iter < limit; iter++) {

		if(priority[highestPriority] == priority[iter]) {
			if((serviceTime[iter] - remainingServiceTime[iter]) < (serviceTime[highestPriority] -remainingServiceTime[highestPriority])) {
				highestPriority = iter;
			}
		}
		else if(priority[highestPriority] < priority[iter]) {
				 highestPriority = iter;
		    }
	} 
	return highestPriority;
}

int main() {
	int numberOfProcess, sumArrival = 0, sumRemaining = 0, sum = 0, timeCounter = 0;
	printf("Enter number of processes : ");
	scanf("%d", &numberOfProcess);

	int arrivalTime[numberOfProcess], priority[numberOfProcess];
	int serviceTime[numberOfProcess], remainingServiceTime[numberOfProcess], waitingTime = 0;

	int i = 0;

	printf("\n");
	for( ; i < numberOfProcess; i++) {
		printf("\nEnter arrival time of process P%d : ", i + 1);
		scanf("%d", &arrivalTime[i]);

		printf("\nEnter service time of process P%d : ", i + 1);
		scanf("%d", &serviceTime[i]);

		priority[i] = 0;
		remainingServiceTime[i] = serviceTime[i];

		sumArrival += arrivalTime[i];
		sumRemaining += remainingServiceTime[i];

		sum = (sumArrival > sumRemaining) ? sumArrival : sumRemaining;
	}

	mergesortUser(arrivalTime, serviceTime, remainingServiceTime, 0, numberOfProcess - 1);
 
 	int timeLimit = 1; 
 	timeCounter = arrivalTime[0];
	while(timeCounter <= sum) {
		int highestPriority;
			while(timeCounter != arrivalTime[timeLimit]) {
				highestPriority = getHighestPriority(priority, serviceTime, remainingServiceTime, timeLimit);
				remainingServiceTime[highestPriority] -= 1;
				if(remainingServiceTime[highestPriority] == 0) {
					priority[highestPriority] = 0;
					waitingTime += (timeCounter + 1) - arrivalTime[highestPriority] - serviceTime[highestPriority];
				}
				else
					priority[highestPriority] += 1;

				int i = 0;
				while(i < timeLimit) {
					if(i == highestPriority || remainingServiceTime[i] == 0) {
						i++;
						continue;
					}
					priority[i] += 2;
					i++;
				}
	
				timeCounter++;
				if(timeCounter > sum)
					break;

				if(remainingServiceTime[numberOfProcess - 1] == 0)
					goto average;
			}
	
		if(timeCounter > sum)
			break;

		if(timeLimit == numberOfProcess) {
			highestPriority = getHighestPriority(priority, serviceTime, remainingServiceTime, timeLimit);
			remainingServiceTime[highestPriority] -= 1;
			if(remainingServiceTime[highestPriority] == 0) {
				priority[highestPriority] = 0;
				waitingTime += (timeCounter + 1) - arrivalTime[highestPriority] - serviceTime[highestPriority];
			}
			else
				priority[highestPriority] += 1;

			int i = 0;
			while(i < timeLimit) {
				if(i == highestPriority || remainingServiceTime[i] == 0) {
					i++;
					continue;
				}
				priority[i] += 2;
				i++;
			}
	
			timeCounter++;
			if(timeCounter > sum)
				break;
			continue;
		}

		timeLimit++;
	}

	average : printf("\nThe average waiting time for each process is : %.2f\n", (float)waitingTime/numberOfProcess);

	return 0;
}
