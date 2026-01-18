#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <fstream>
using namespace std;

void merge(vector<int>& arr, int left, int mid, int right) {
	int n1 = mid - left + 1;
	int n2 = right - mid;

	vector<int> L(n1), R(n2);
	for (int i = 0; i < n1; i++){
		L[i] = arr[left + i];
	}
	for (int j = 0; j < n2; j++) {
		R[j] = arr[mid + 1 + j];
	}

	
	int i = 0, j = 0;
	int k = left;
	while (i < n1 && j < n2) {
		if (L[i] <= R[j]) {
			arr[k] = L[i];
			i++;
		}
		else {
			arr[k] = R[j];
			j++;
		}
		k++;
	}
	while (i < n1) {
		arr[k] = L[i];
		i++;
		k++;
	}
	while (j < n2) {
		arr[k] = R[j];
		j++;
		k++;
	}
}

void mergeSort(vector<int>& arr, int left, int right) {
	if (left >= right) {
		return;
	}
	int mid = left + (right - left) / 2;
	mergeSort(arr, left, mid);
	mergeSort(arr, mid + 1, right);
	merge(arr, left, mid, right);
}



int main() {
	srand(time(0));
	int nTest;
	
	cout << "How many trials?: ";
	cin >> nTest;
	vector<int> amt(nTest);
	vector<double> nTime(nTest);

	for (int i = 0; i < nTest; i++) {
		cout << "trial " << (i + 1) << " size: ";
		cin >> amt[i];
	}

	

	for (int j = 0; j < nTest; j++) {
		vector<int> numbers(amt[j]);
		for (int k = 0; k < amt[j]; k++) {
			numbers[k] = rand() % 10000;
		}
		clock_t start = clock();
		mergeSort(numbers, 0, numbers.size() - 1);

		clock_t end = clock();
		double time_record = double(end - start) / CLOCKS_PER_SEC;
		nTime[j] = time_record;

		cout << "Trial " << (j + 1) << " (" << amt[j] << " numbers): " << time_record << " seconds" << endl;
	}
		


	ofstream dataFile("mergesort_data.txt");
	for (int i = 0; i < nTest; i++) {
		dataFile << amt[i] << " " << nTime[i] << endl;
	}
	dataFile.close();


	ofstream gnuplotScript("plot_script.gnu");
	gnuplotScript << "set terminal png size 1200,780" << endl;
	gnuplotScript << "set output 'mergesort_performance.png" << endl;
	gnuplotScript << "set title 'Merge Sort Performance'" << endl;
	gnuplotScript << "set xlabel 'Array size'" << endl;
	gnuplotScript << "set ylabel 'Time (seconds)'" << endl;
	gnuplotScript << "set grid" << endl;
	gnuplotScript << "set style line 1 lc rgb 'blue' pt 7 ps 1.5 lt 1 lw 2" << endl;
	gnuplotScript << "plot 'mergesort_data.txt' with linespoints ls 1 title 'Merge Sort'" << endl;
	gnuplotScript.close();

	int result = system("gnuplot plot_script.gnu");
	if (result == 0) {
		cout << "Plot saved as 'mergesort_performance.png'" << endl;
		#ifdef _WIN32
			system("start mergesort_performance.png");
		#elif __APPLE__
			system("open mergesort_performance.png");
		#else
			system("xdg-open mergesort_performance.png");
		#endif
	} else {
		cout << "Error" << endl;
	}

	return 0;


}

	


