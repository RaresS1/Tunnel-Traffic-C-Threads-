#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define MAX_CARS 20
#define MAX_GAS_LEAKS 5

pthread_mutex_t carCountMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t gasLeakMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t smokeMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t panicButtonM = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t panicButton = PTHREAD_COND_INITIALIZER;
pthread_cond_t tunelCapacity = PTHREAD_COND_INITIALIZER;
pthread_mutex_t externalBlckMutex = PTHREAD_MUTEX_INITIALIZER;

int carCount = 0;
int gasLeak = 0;
int smokeDetected = 0;
int panicButtonActivated = 0;
int external_barrier = 0;


void* car_Monitoring_Entrance(void* arg){
    while (1){

		//nested mutexes for the panic mode. It checks the variable of the pannic button to be 0 in order to let the cars enter the tunnel
		if(((pthread_mutex_lock(&panicButtonM) == 0) && (panicButtonActivated == 0))  &&  ((pthread_mutex_lock(&gasLeakMutex) == 0)  && (gasLeak == 0)) && ((pthread_mutex_lock(&smokeMutex) == 0)  && (smokeDetected == 0)) && ((pthread_mutex_lock(&externalBlckMutex)== 0) && (external_barrier == 0)) ) {

			//mutex for the entering and leaving the tunnel + the case in which the max number is exceeded.
			pthread_mutex_lock(&carCountMutex);

			//If the tunnel is full it waits for the exit of 50 percent of the cars

			if (carCount >= MAX_CARS){
				printf("\nMaximum number of cars reached. Stopping tunnel access.\n");
				while(carCount > (MAX_CARS * 0.5)){
					printf("\nThere are still to many cars to reestablish the tunnel access!\n");
					pthread_mutex_unlock(&externalBlckMutex);
					pthread_cond_wait(&tunelCapacity, &carCountMutex);

				}
			}

			carCount += rand() % 5 + 1;
			printf("\nCars enter!\n");
			printf("\nThe number of cars in the tunnel is: %d\n", carCount);

			//It signals that the traffic is dense, we chose a point of 80 percent

			if(carCount > (MAX_CARS * 0.8)){
				printf("\nThe tunnel traffic is dense!\n");
			}

			pthread_mutex_unlock(&carCountMutex);
			pthread_mutex_unlock(&panicButtonM);
			pthread_mutex_unlock(&gasLeakMutex);
			pthread_mutex_unlock(&smokeMutex);
			pthread_mutex_unlock(&externalBlckMutex);
		}else
			{
				sleep(2);
				continue;
			}


		sleep(3);


	}
    return NULL;
}

void *car_Monitoring_Exit(void *arg){

	while (1){
		if((pthread_mutex_lock(&externalBlckMutex) == 0) && (external_barrier == 0)){
			pthread_mutex_lock(&carCountMutex);

			if(carCount >=1){
				carCount -= rand() % 2 + 1;
				printf("\nCar exits\n");
				printf("\nThe number of cars in the tunnel is: %d\n", carCount);

			}else{
				printf("\nThe Tunnel is empty!\n");
			}

			pthread_mutex_unlock(&carCountMutex);
			pthread_mutex_unlock(&externalBlckMutex);
			pthread_cond_signal(&tunelCapacity);


		}else{
			sleep(2);
			continue;
		}
		sleep(4);
	}
	return NULL;
}


void* gasLeakMonitoring(void* arg) {
    while (1){
        // Simulate some delay between gas leak checks
		sleep(rand() % 6 + 5);
		int number = rand() % 3 + 1;
        if (number == 3) {
            pthread_mutex_lock(&gasLeakMutex);
			gasLeak = 1;
			printf("\n-------Too much gas. Stopping tunnel access.-------\n");

			sleep (6);

			gasLeak = 0;
			printf("\n------The gas level in the Tunnel is 0!--------\n");

			pthread_mutex_unlock(&gasLeakMutex);

        }


    }
    return NULL;
}

void* smokeDetection(void* arg) {
   while (1){
		sleep(rand() % 6 + 10);
        // Simulate smoke detection
		int number = rand() % 3 + 1;
        if (number == 1) {
            pthread_mutex_lock(&smokeMutex);
            smokeDetected = 1;
			printf("\n!!!!!Smoke has been detected!!!!!\n");

			sleep(4);

			smokeDetected = 0;
			printf("\n!!!!!!The somke is gone, reestablish the access!!!!!!!\n");

			pthread_mutex_unlock(&smokeMutex);

        }



    }
    return NULL;
}

void* panicButtonMonitoring(void* arg) {
    // Simulate panic button activation
    while (1){
		//Simulating a time for the button to pe pressed and the we choose a probability for the panic button to be pressed
		sleep(rand() % 6 + 15);
		int number = rand() % 3 + 1;
		if(number == 2){
			//sleep((rand() % 11) + 20);
			pthread_mutex_lock(&panicButtonM);
			panicButtonActivated = 1;
			printf("\n#######The panic button has been pressed!######\n");


			sleep(5);

			panicButtonActivated = 0;
			printf("\n######The danger is gone, access is clear!######\n");

			pthread_mutex_unlock(&panicButtonM);
		}
	}
	return NULL;
}




void* externalControl(void* arg) {
    // Simulate external control (e.g., emergency services)
    while (1) {
		sleep(rand() % 6 + 25);
		int number1 = rand() % 3 + 1;
        if (number1 < 3){
            pthread_mutex_lock(&externalBlckMutex);
			external_barrier = 1;
            printf("External control: Tunnel access blocked.\n");

			sleep (5);

			external_barrier = 0;
			pthread_mutex_unlock(&externalBlckMutex);
        }


	}
    return NULL;
}

int main() {
    pthread_t carThread1, carThread2, panicButtonThread, gasLeakThread, smokeThread, externalControlThread;

    // Create threads for monitoring different aspects
    pthread_create(&carThread1, NULL, car_Monitoring_Entrance, NULL);
	pthread_create(&carThread2, NULL, car_Monitoring_Exit, NULL);
    pthread_create(&panicButtonThread, NULL, panicButtonMonitoring, NULL);
	pthread_create(&gasLeakThread, NULL, gasLeakMonitoring, NULL);
    pthread_create(&smokeThread, NULL, smokeDetection, NULL);
	pthread_create(&externalControlThread, NULL, externalControl, NULL);

    // Join threads

    pthread_join(carThread1, NULL);
	pthread_join(carThread2,NULL);
    pthread_join(panicButtonThread, NULL);
	pthread_join(gasLeakThread, NULL);
    pthread_join(smokeThread, NULL);
    pthread_join(externalControlThread, NULL);

    return 0;
}
