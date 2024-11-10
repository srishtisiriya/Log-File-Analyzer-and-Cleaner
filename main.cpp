#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <algorithm>

// Global mutex for synchronizing access to the shared result
std::mutex mtx;
int result = 0;  // Shared resource to store the final result

// Function to process a chunk of data (e.g., sum of numbers)
void processChunk(std::vector<int>& chunk) {
	int localResult = 0;

	// Process each data point in the chunk (e.g., summing up the numbers)
	for (int i : chunk) {
		if (i < 0) {
			std::cerr << "Error: Negative value encountered: " << i << std::endl;
		} else {
			localResult += i;
		}
	}

	// Lock the shared resource to safely update the result
	std::lock_guard<std::mutex> guard(mtx);
	result += localResult;
}

int main() {
	// Example dataset (large data can be added here)
	std::vector<int> data = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20};

	// Define the chunk size for each thread to process
	size_t chunkSize = 5;  // You can adjust the chunk size based on your system and dataset size

	// Vector to store the threads
	std::vector<std::thread> threads;

	// Split the data into chunks and assign each chunk to a thread for processing
	for (size_t i = 0; i < data.size(); i += chunkSize) {
		std::vector<int> chunk(data.begin() + i, data.begin() + std::min(i + chunkSize, data.size()));

		// Create a thread to process the chunk
		threads.push_back(std::thread(processChunk, std::ref(chunk)));
	}

	// Wait for all threads to finish processing
	for (auto& t : threads) {
		t.join();
	}

	// Print the final result after all threads have finished processing
	std::cout << "Data processing complete!" << std::endl;
	std::cout << "Total sum of all chunks: " << result << std::endl;

	return 0;
}
