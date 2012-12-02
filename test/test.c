
#include <sys/syscall.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#define SET_GPS 376
#define GET_GPS 377

/* Mounted Test GPS File */
#define TEST_GPS_FILE "/data/misc/hmwk6/gps_test.txt"

struct gps_location {
	double latitude;
	double longitude;
	float  accuracy;  /* in meters */
};


static void print_gps(struct gps_location gps_location)
{
	unsigned long long int lat = 0, lng = 0;
	unsigned int acc = 0;
	lat = *((unsigned long long int*) &gps_location.latitude);
	lng = *((unsigned long long int*) &gps_location.longitude);
	acc = *((unsigned int*) &gps_location.accuracy);

	/* still doesn't appear to work */
	printf("Latitude: %#llx\n Longitude: %#llx\n Accuracy: %#x",
			lat, lng, acc);
}

static void test()
{
	int ret;
	struct gps_location loc;
	FILE *fp = NULL;

	printf("Test GPS Program\n");
	memset(&loc, 0, sizeof(loc));

	printf("Starting GPS Info:\n");
	print_gps(loc);

	/* Create a New File and Close it */
	fp = fopen(TEST_GPS_FILE, "w");
	printf("Opening file ...\n");
	if (fp == NULL) {
		printf("Failed to open Test GPS File: %s\n", TEST_GPS_FILE);
		return;
	}
	printf("File Open succeeded\n");
	fprintf(fp, "Hello World at Time T = %ld\n", time(NULL));
	fflush(NULL);
	fclose(fp);

	/* Retrieve File GPS Info From Kernel */
	printf("About to Make System Call to Kernel to retrieve GPS info\n");
	ret = syscall(GET_GPS, "/data/misc/hmwk6/gps_test.txt", &loc);
	printf("done\n");
	return;
	if (ret < 0 ) {
		perror("System call failed:");
		return;
	} else {
		printf("System call worked\n");
		printf("Retrieved GPS Information:\n");
		print_gps(loc);
	}
}

int main(int argc, char **argv)
{
	test();
	return 0;
}
