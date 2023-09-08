#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>


typedef struct Point {
	float lat, lon;
}Point;


void readBenchmarkDataset(std::vector<Point> &dataset, char* filename, int length) {
	FILE *f_data = fopen(filename, "rb");
	if( f_data == NULL ) {
		printf( "File not found: %s\n", filename );
		exit(1);
	}

	for ( int i = 0; i < length; i ++ ) {
		int numPoints = dataset.size();
		dataset.resize(numPoints+1);
		fread(&dataset[i].lat, sizeof(float), 1, f_data);
		fread(&dataset[i].lon, sizeof(float), 1, f_data);
	}

	fclose(f_data);
}

void writeBenchmarkDataset(std::vector<Point> &dataset, char* filename, int length) {
	FILE* f_data = fopen(filename, "wb");

	for ( int i = 0; i < length; i ++ ) {
		fwrite(&dataset[i].lat, sizeof(float), 1, f_data);
		fwrite(&dataset[i].lon, sizeof(float), 1, f_data);
	}

	fclose(f_data);
}


int main() {
	int initNumCities = 44691;

	std::vector<Point> dataset_ori;
	std::vector<Point> dataset_aug;
	std::vector<Point> dataset_chk;
	std::vector<float> dataset_aug_lat;
	std::vector<float> dataset_aug_lon;

	// Read original dataset first
	char benchmark_ori_filename[] = "worldcities.bin";
	readBenchmarkDataset(dataset_ori, benchmark_ori_filename, initNumCities);

	// Make latitude data first
	for ( float i = -80.00; i < 80.00; i = i + 1 ) {
		dataset_aug_lat.push_back(i);
	}
	printf( "%f, %d\n", dataset_aug_lat[0], (int)dataset_aug_lat.size() );

	// Make longitude data then
	for ( float i = -176.00; i < 176.00; i = i + 0.0005 ) {
		dataset_aug_lon.push_back(i);
	}
	printf( "%f, %d\n", dataset_aug_lon[0], (int)dataset_aug_lon.size() );

	// Merge latitude and longitude
	for ( int i = 0; i < (int)dataset_aug_lat.size(); i ++ ) {
		float lat = dataset_aug_lat[i];
		for ( int j = 0; j < (int)dataset_aug_lon.size(); j ++ ) {
			Point temp;
			temp.lat = lat;
			temp.lon = dataset_aug_lon[j];
			dataset_aug.push_back(temp);
		}
	}
	printf( "%f %f, %d\n", dataset_aug[0].lat, dataset_aug[0].lon, (int)dataset_aug.size() );

	// Write the augmented dataset to bin file
	char benchmark_aug_filename[] = "worldcities_augmented.bin";
	writeBenchmarkDataset(dataset_aug, benchmark_aug_filename, (int)dataset_aug.size());

	// Check writing to bin file in a right way
	readBenchmarkDataset(dataset_chk, benchmark_aug_filename, (int)dataset_aug.size());
	printf( "%f %f, %d\n", dataset_chk[0].lat, dataset_chk[0].lon, (int)dataset_chk.size() );
}
