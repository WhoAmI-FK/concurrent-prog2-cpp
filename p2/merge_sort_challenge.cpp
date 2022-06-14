#include <thread>
#include <cmath>
#include <cstdio>
void merge(int* array, unsigned int left, unsigned int mid, unsigned right);

void sequential_merge_sort(int* array, unsigned int left, unsigned right)
{
        if(left<right)
        {
                unsigned int mid=(left+right)/2;
                sequential_merge_sort(array,left,mid);
                sequential_merge_sort(array,mid+1,right);
                merge(array, left, mid, right);
        }

}

void parallel_merge_sort(int* array, unsigned int left, unsigned int right, unsigned int depth=0)
{
        if(depth>=std::log(std::thread::hardware_concurrency()))
        {
                sequential_merge_sort(array,left,right);
        }else{
                unsigned int mid = (left+right)/2;
                std::thread left_thread = std::thread(parallel_merge_sort,array,left,mid,depth+1);
                parallel_merge_sort(array,mid+1,right,depth+1);
                left_thread.join();
                merge(array,left,mid,right);
        }
}

void merge(int* array, unsigned int left, unsigned int mid, unsigned int right)
{
        unsigned int num_left = mid - left + 1;
                unsigned int num_right = right - mid; // number of elements in right subarray

        // copy data into temporary left and right subarrays to be merged
        int array_left[num_left], array_right[num_right];
        std::copy(&array[left], &array[mid + 1], array_left);
        std::copy(&array[mid + 1], &array[right + 1], array_right);

        // initialize indices for array_left, array_right, and input subarrays
        unsigned int index_left = 0;    // index to get elements from array_left
        unsigned int index_right = 0;    // index to get elements from array_right
        unsigned int index_insert = left; // index to insert elements into input array

        // merge temporary subarrays into original input array
        while ((index_left < num_left) || (index_right < num_right)) {
                if ((index_left < num_left) && (index_right < num_right)) {
                        if (array_left[index_left] <= array_right[index_right]) {
                                array[index_insert] = array_left[index_left];
                                index_left++;
                        }
                        else {
                                array[index_insert] = array_right[index_right];
                                index_right++;
                        }
                }
                // copy any remain elements of array_left into array
                else if (index_left < num_left){
                        array[index_insert] = array_left[index_left];
                        index_left += 1;
                }
                // copy any remain elements of array_right into array
                else if (index_right < num_right) {
                        array[index_insert] = array_right[index_right];
                        index_right += 1;
                }
                index_insert++;
        }
}


int main()
{
        const int NUM_EVAL_RUNS = 100;
        const int N = 100000;

                int original_array[N], sequential_result[N], parallel_result[N];
        for (int i=0; i<N; i++) {
                original_array[i] = rand();
        }

        printf("Evaluating Sequential Implementation...\n");
        std::chrono::duration<double> sequential_time(0);
        std::copy(&original_array[0], &original_array[N-1], sequential_result);
        sequential_merge_sort(sequential_result, 0, N-1);
        for (int i=0; i<NUM_EVAL_RUNS; i++) {
                std::copy(&original_array[0], &original_array[N-1], sequential_result);
                auto start_time = std::chrono::high_resolution_clock::now();
                sequential_merge_sort(sequential_result, 0, N-1);
                sequential_time += std::chrono::high_resolution_clock::now() - start_time;
        }
        sequential_time /= NUM_EVAL_RUNS;

        printf("Evaluating Parallel Implementation...\n");
        std::chrono::duration<double> parallel_time(0);
        std::copy(&original_array[0], &original_array[N-1], parallel_result);
        parallel_merge_sort(parallel_result, 0, N-1);
        for (int i=0; i<NUM_EVAL_RUNS; i++) {
                std::copy(&original_array[0], &original_array[N - 1], parallel_result);
                auto start_time = std::chrono::high_resolution_clock::now();
                parallel_merge_sort(parallel_result, 0, N-1);
                parallel_time += std::chrono::high_resolution_clock::now() - start_time;
        }
        parallel_time /= NUM_EVAL_RUNS;


        for (int i=0; i<N; i++) {
                if (sequential_result[i] != parallel_result[i]) {
                        printf("ERROR: Result mismatch at index %d!\n", i);
                }
        }
        printf("Average Sequential Time: %.2f ms\n", sequential_time.count()*1000);
        printf("  Average Parallel Time: %.2f ms\n", parallel_time.count()*1000);
        printf("Speedup: %.2f\n", sequential_time/parallel_time);
        printf("Efficiency %.2f%%\n", 100*(sequential_time/parallel_time)/std::thread::hardware_concurrency());

}