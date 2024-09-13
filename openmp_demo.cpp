// Demo of openmp code

// to build: g++ openmp_demo.cc -o openmp_demo

#include <iostream>
#include <omp.h>
#include <chrono>

// Normally we'd just check that each value in sum[] equals 3.
// This method is just to show the reduce function in openmp.
bool verify(int* sum_array, long array_size)
{
    long actual_sum = 0;
    long expected_sum = 3 * array_size;

    #pragma omp parallel for reduction(+ : actual_sum);
    {
        for (int i = 0; i < array_size; ++i)
        {
            actual_sum += sum_array[i];
        }
    }
    
    // return 0 if equal, 1 if not.
    return !(actual_sum == expected_sum);
}

// Basic streaming benchmark that sums 2 arrays. Can be used to measure memory overhead.
bool omp_stream ( unsigned int scale) {

    long array_size = 1 << scale;

    int array1[array_size];
    int array2[array_size];
    int sum[array_size];

    #pragma omp parallel for
    {
        // Initialize the arrays
        for (int i = 0; i < array_size; ++i) {
            array1[i] = 1;
            array2[i] = 2;
            sum[i] = 0;
        }
    }


    // sum each element in the 2 arrays in parallel, store in sum
    auto start = std::chrono::high_resolution_clock::now();

    #pragma omp parallel for
    {
        for (int i=0; i < array_size; ++i)
        {
            sum[i] = array1[i] + array2[i];
        }
    }
    
    // Get execution time in milliseconds
    auto end = std::chrono::high_resolution_clock::now();
    double time_ms = std::chrono::duration<double, std::ratio<1, 1000>>(end - start).count();

    // Print results
    std::cout << "Added 2 arrays with " << array_size << " elements each in " 
        << time_ms << " ms." << std::endl;

    bool result = verify(sum, array_size);
    if (result == 0)
        std::cout << "Verification passed." << std::endl;
    else 
        std::cout << "Verification failed." << std::endl;

    return result;
}


int main(int argc, char* argv[])
{
    omp_stream(16);
}