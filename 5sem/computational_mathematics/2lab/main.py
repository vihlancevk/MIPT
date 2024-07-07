import pprint
import numpy as np
import matplotlib.pyplot as plt

from typing import Tuple


class MatrixSolver:
    """
    Solver for next equations:
    Ax = f
    """

    def _calculate_A_and_f_from_matrix_of_coefficients(self, matrix: np.matrix) -> Tuple[np.matrix, np.matrix]:
        """
        :param matrix: numpy matrix of coefficient [n x m]
        :return: tuple from 2 values: numpy matrix A [n x n] and numpy matrix f [n x 1]
        """
        n: int = matrix.shape[0]
        m: int = matrix.shape[1] - 1

        A: np.matrix = np.matrix(
            np.zeros([n, m])
        )
        f: np.matrix = np.matrix(
            np.zeros([n, 1])
        )

        i: int
        for i in range(n):

            j: int
            for j in range(m):
                A[i, j] = matrix[i, j]

            f[i, 0] = matrix[i, -1]

        return A, f

    def __init__(self, matrix: np.matrix) -> None:
        """
        :param matrix: numpy matrix (A|f) [n x m]
        """
        self._matrix: np.matrix = matrix
        self._A, self._f = self._calculate_A_and_f_from_matrix_of_coefficients(matrix)

    def _calculate_difference(self, x: np.matrix) -> np.matrix:
        """
        r = Ax - f
        :param x: numpy matrix of solution [n x 1]
        :return: numpy matrix of residual [n x 1]
        """
        return self._A * x - self._f

    def solve_by_gaussian_method_with_selection_main_element(self) -> Tuple[np.matrix, float]:
        """
        Gaussian method with selection of the main element by column
        :return: tuple from 2 values: numpy matrix x [n x 1] and residual
        """
        matrix: np.matrix = self._matrix.copy()

        n: int = matrix.shape[0]

        i: int
        for i in range(n):
            max_element: float = abs(matrix[i, i])
            max_row: int = i

            k: int
            for k in range(i + 1, n):
                if abs(matrix[k, i]) > max_element:
                    max_element: float = abs(matrix[k, i])
                    max_row: int = k

            row: np.array = np.copy(matrix[i])
            matrix[i] = matrix[max_row]
            matrix[max_row] = row

            K: int
            for k in range(i + 1, n):
                factor: float = matrix[k, i] / matrix[i, i]

                j: int
                for j in range(i, n + 1):
                    if i == j:
                        matrix[k, j] = 0
                    else:
                        matrix[k, j] -= factor * matrix[i, j]

        x: np.matrix = np.matrix(
            np.zeros([n, 1])
        )

        i: int
        for i in range(n - 1, -1, -1):
            x[i, 0] = matrix[i, n] / matrix[i, i]

            k: int
            for k in range(i - 1, -1, -1):
                matrix[k, n] -= matrix[k, i] * x[i, 0]

        return x, np.linalg.norm(self._calculate_difference(x))

    def _decompose_to_LU(self) -> np.matrix:
        """
        Decompose matrix A to L and U matrices.
        L and U triangular matrices will be represented in a single [n x n] matrix.
        :return: numpy LU matrix [n x n]
        """
        A: np.matrix = self._A.copy()

        n: int = A.shape[0]

        # create emtpy LU-matrix
        lu_matrix: np.matrix = np.matrix(
            np.zeros([n, n])
        )

        k: int
        for k in range(n):

            # calculate all residual k-row elements
            j: int
            for j in range(k, n):
                lu_matrix[k, j] = A[k, j] - lu_matrix[k, : k] * lu_matrix[: k, j]

            # calculate all residual k-column elemetns
            i: int
            for i in range(k + 1, n):
                lu_matrix[i, k] = (A[i, k] - lu_matrix[i, :k] * lu_matrix[:k, k]) / lu_matrix[k, k]

        return lu_matrix

    def solve_by_LU_decomposition_method(self) -> Tuple[np.matrix, float]:
        """
        Solve system of equations from given LU-matrix and vector b of absolute terms.
        :return: tuple from 2 values: numpy matrix x [n x 1] and residual
        """
        lu_matrix: np.matrix = self._decompose_to_LU()
        f: np.matrix = self._f

        # get supporting vector y
        y: np.matrix = np.matrix(np.zeros([lu_matrix.shape[0], 1]))

        i: int
        for i in range(y.shape[0]):
            y[i, 0] = f[i] - lu_matrix[i, :i] * y[:i]

        # get vector of answers x
        x: np.matrix = np.matrix(np.zeros([lu_matrix.shape[0], 1]))

        i: int
        for i in range(1, x.shape[0] + 1):
            x[-i, 0] = (y[-i] - lu_matrix[-i, -i:] * x[-i:, 0]) / lu_matrix[-i, -i]

        return x, np.linalg.norm(self._calculate_difference(x))

    def solve_by_seidel_method(self, eps=1e-5) -> Tuple[np.matrix, np.array]:
        """
        Solves the equation Ax=f via the Seidel iterative method.
        """
        A: np.matrix = self._A
        f: np.matrix = self._f

        n: int = A.shape[0]

        x: np.matrix = np.zeros_like(f)
        residuals: list[float] = []

        while True:
            old_x: np.matrix = x.copy()

            i: int
            for i in range(n):
                temp1: np.matrix = A[i, :i] * x[:i]
                temp2: np.matrix = A[i, i + 1:] * x[i + 1:]
                x[i] = (f[i] - temp1 - temp2) / A[i, i]

            residuals.append(
                np.linalg.norm(self._calculate_difference(x))
            )

            if np.linalg.norm(x - old_x) < eps:
                return x, np.array(residuals)

    def solve_by_jacobi_method(self, eps=1e-5) -> Tuple[np.matrix, np.array]:
        """
        Solves the equation Ax=f via the Jacobi iterative method.
        """
        A: np.matrix = self._A
        f: np.matrix = self._f

        n: int = A.shape[0]

        # Create an initial guess
        x: np.matrix = np.zeros_like(f)
        residuals: list[float] = []

        # Create a vector of the diagonal elements of A and subtract them from A
        D: np.matrix = np.matrix(
            np.zeros(A.shape)
        )

        i: int
        for i in range(n):
            D[i, i] = A[i, i]

        R: np.matrix = A - D

        while True:
            old_x = np.copy(x)

            x = np.linalg.inv(D) * (f - R * x)

            residuals.append(
                np.linalg.norm(self._calculate_difference(x))
            )

            if np.linalg.norm(x - old_x) < eps:
                return x, np.array(residuals)

    def solve_by_upper_relaxation_method(self, omega=1.1, eps=1e-6) -> Tuple[np.matrix, np.array]:
        """
        Solves the equation Ax=f via the upper relaxation method iterative method.
        """
        A: np.matrix = self._A
        f: np.matrix = self._f

        n: int = A.shape[0]

        x: np.matrix = np.zeros_like(f)
        residuals: list[float] = []

        while True:
            x_old: np.matrix = x.copy()

            i: int
            for i in range(n):
                row_sum: np.matrix = A[i, :i] * x[:i] + A[i, i + 1:] * x[i + 1:]
                x[i] = (1 - omega) * x_old[i] + omega * (f[i] - row_sum) / A[i, i]

            residuals.append(
                np.linalg.norm(self._calculate_difference(x))
            )

            if np.linalg.norm(x - x_old) < eps:
                return x, np.array(residuals)


def build_graph(residuals: np.array) -> None:
    plt.title('', fontsize=12)

    plt.scatter(
        x=np.array([i for i in range(len(residuals))]),
        y=residuals
    )

    plt.xlabel(r'$ k $', fontsize=12)
    plt.ylabel(r'$ ||r|| $', fontsize=12)

    plt.grid(True)

    plt.show()


def create_matrix() -> np.matrix:
    n: int = 100
    a, b, c, p = 1, 10, 1, 1

    matrix: np.matrix = np.matrix(
        np.zeros((n, n + 1))
    )

    i: int
    j: int = -1
    for i in range(n - 1):
        if j >= 0:
            matrix[i, j] = a

        matrix[i, j + 1] = b
        matrix[i, j + 2] = c

        j += 1

    for j in range(n):
        matrix[-1, j] = 1

    for i in range(n):
        matrix[i, -1] = i + 1

    return matrix


def solve_by_direct_method(msg: str, method) -> None:
    x, r = method()

    print(msg)
    pprint.pprint(x)
    print('residual: ', r, '\n')


def solve_by_iterative_method(msg: str, method) -> None:
    x, r = method()

    print(msg)
    pprint.pprint(x)
    build_graph(r)
    print()


if __name__ == '__main__':
    matrix: np.matrix = create_matrix()

    matrix_solver: MatrixSolver = MatrixSolver(matrix)

    solve_by_direct_method(
        'Gaussian method with selection of the main element by column:',
        matrix_solver.solve_by_gaussian_method_with_selection_main_element
    )

    solve_by_direct_method(
        'LU decomposition method:',
        matrix_solver.solve_by_LU_decomposition_method
    )

    solve_by_iterative_method(
        'Seidel method:',
        matrix_solver.solve_by_seidel_method
    )

    solve_by_iterative_method(
        'Jacobi method:',
        matrix_solver.solve_by_jacobi_method
    )

    solve_by_iterative_method(
        'Upper relaxation method:',
        matrix_solver.solve_by_upper_relaxation_method
    )
