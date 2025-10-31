#include <cstdlib>
#include <iostream>
#include <ostream>
#include <thread>
#include <vector>

int MAX_THREADS = 2;

void eliminate(std::vector<std::vector<double>>& A, std::vector<double>& b, int pivotRow,
               int startRow, int endRow) {
    int size = A.size();

    for (int currentRow = startRow; currentRow < endRow; ++currentRow) {
        if (currentRow == pivotRow)
            continue;

        double factor = A[currentRow][pivotRow] / A[pivotRow][pivotRow];

        for (int col = pivotRow; col < size; ++col)
            A[currentRow][col] -= factor * A[pivotRow][col];

        b[currentRow] -= factor * b[pivotRow];
    }
}

std::vector<double> gaussSolve(std::vector<std::vector<double>> A, std::vector<double> b) {
    int size = A.size();

    for (int pivotRow = 0; pivotRow < size; ++pivotRow) {
        double pivot = A[pivotRow][pivotRow];
        for (int j = pivotRow; j < size; ++j)
            A[pivotRow][j] /= pivot;
        b[pivotRow] /= pivot;

        int rowsPerThread = (size + MAX_THREADS - 1) / MAX_THREADS;
        std::vector<std::thread> threads;

        for (int t = 0; t < MAX_THREADS; ++t) {
            int startRow = t * rowsPerThread;
            int endRow = std::min(startRow + rowsPerThread, size);

            if (startRow >= size)
                break;

            threads.emplace_back(eliminate, std::ref(A), std::ref(b), pivotRow, startRow, endRow);
        }

        for (auto& th : threads)
            th.join();
    }

    std::vector<double> x(size);
    for (int i = size - 1; i >= 0; --i) {
        x[i] = b[i];

        for (int j = i + 1; j < size; ++j)
            x[i] -= A[i][j] * x[j];
    }

    return x;
}

int main(int argc, char* argv[]) {
    if (argc > 1)
        MAX_THREADS = std::atoi(argv[1]);

    std::cout << "Maximum number of threads: " << MAX_THREADS << std::endl;

    int size;
    std::cout << "Enter system size: ";
    std::cin >> size;

    std::vector<std::vector<double>> A(size, std::vector<double>(size));
    std::vector<double> b(size);

    std::cout << "Enter the coefficients of matrix A (line by line):" << std::endl;
    for (int i = 0; i < size; ++i)
        for (int j = 0; j < size; ++j)
            std::cin >> A[i][j];

    std::cout << "Enter the vector of right-hand sides:" << std::endl;
    for (int i = 0; i < size; ++i)
        std::cin >> b[i];

    std::vector<double> result = gaussSolve(A, b);

    std::cout << std::endl << "System solution:" << std::endl;
    for (int i = 0; i < size; ++i)
        std::cout << "x[" << i << "] = " << result[i] << std::endl;

    return 0;
}
