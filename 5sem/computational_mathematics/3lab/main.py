from typing import Union

import numpy as np

import matplotlib.pyplot as plt


class Solver:
    @staticmethod
    def solve_equation_by_simple_iteration_method(f, x0, precision=1e-5):
        """
        F(x) = 0 -> x_{n+1} = f(x_{n})
        :param f: function
        :param x0: start value that represented by float number
        :param precision: precision that represented by float number
        :return: solution and residual that represented by float number and numpy array
        """
        residual = []

        current_x = x0
        next_x = f(current_x)

        residual.append(abs(next_x - current_x))

        while residual[-1] > precision:
            current_x = next_x
            next_x = f(current_x)

            residual.append(abs(next_x - current_x))

        return next_x, np.array(residual)

    @staticmethod
    def solve_equation_by_newton_method(F, x0, precision=1e-5):
        """
        F(x) = 0 -> x_{n+1} = x_{n} - [F^{`}_{x}(x_{n})]^{-1} * F(x_{n})
        :param F: equation
        :param x0: start value that represented by float number
        :param precision: precision that represented by float number
        :return: solution and residual that represented by float number and numpy array
        """
        def derivative_F(x, h=1e-2):
            return (F(x + h) - F(x)) / h

        residual = []

        current_x = x0
        next_x = current_x - derivative_F(current_x)**(-1) * F(current_x)

        residual.append(abs(next_x - current_x))

        while residual[-1] > precision:
            current_x = next_x
            next_x = current_x - derivative_F(current_x)**(-1) * F(current_x)

            residual.append(abs(next_x - current_x))

        return next_x, np.array(residual)

    @staticmethod
    def solve_nonlinear_system_equations_by_simple_iteration_method(f, u0, precision=1e-5):
        """
        F(u) = 0 -> u_{n+1} = f(u_{n})
        :param f: function that represented by numpy array
        :param u0: start value that represented by numpy array
        :param precision: precision that represented by float number
        :return: solution and residual that represented by 2 numpy arrays
        """
        residual = []

        current_u = u0
        next_u = np.array([f[i](*current_u) for i in range(len(f))])

        residual.append(np.linalg.norm(next_u - current_u))

        while residual[-1] > precision:
            current_u = next_u
            next_u = np.array([f[i](*current_u) for i in range(len(f))])

            residual.append(np.linalg.norm(next_u - current_u))

        return next_u, np.array(residual)

    @staticmethod
    def solve_nonlinear_system_equations_by_newton_method(F, u0, precision=1e-5):
        """
        F(u) = 0 -> u_{k+1} = u_{k} - J^{-1}(u_{k})*F(u_{k})
        :param F: equation that represented by numpy array
        :param u0: start value that represented by numpy array
        :param precision: precision that represented by float number
        :return: solution and residual that represented by 2 numpy arrays
        """
        n = u0.shape[0]

        def calculate_J(u):

            def derivative(i, j, h=1e-2):
                return (F[i](*(u + np.array([h if index == j else 0 for index in range(n)]))) - F[i](*u)) / h

            J = np.zeros(shape=(n, n))

            for i in range(n):
                for j in range(n):
                    J[i, j] = derivative(i, j)

            return J

        residual = []

        current_u = u0
        next_u = current_u - np.matmul(np.linalg.inv(calculate_J(current_u)), np.array([F[i](*current_u) for i in range(n)]))

        residual.append(np.linalg.norm(next_u - current_u))

        while residual[-1] > precision:
            current_u = next_u
            next_u = current_u - np.matmul(np.linalg.inv(calculate_J(current_u)), np.array([F[i](*current_u) for i in range(n)]))

            residual.append(np.linalg.norm(next_u - current_u))

        return next_u, np.array(residual)

    @staticmethod
    def visualize_solution(x: Union[float, np.array], residual: np.array, symbol):
        plt.title(str(symbol) + ' = ' + repr(x), fontsize=12)

        plt.scatter(
            x=1 + np.arange(len(residual)),
            y=residual
        )

        plt.xlabel(r'$ k $', fontsize=12)
        plt.ylabel(r'$ ||r|| $', fontsize=12)

        plt.grid(True)

        plt.show()


# Equation: 2x^2 + 5x - 3 = 0. We want to specify root x = 0.5 use simple iteration method
x, residual = Solver.solve_equation_by_simple_iteration_method(
    lambda x: (3 - 2 * x * x) / 5,
    0
)
Solver.visualize_solution(x, residual, 'x')

# Equation: 2x^2 + 5x - 3 = 0. We want to specify root x = -3 use newton method
x, residual = Solver.solve_equation_by_newton_method(
    lambda x: 2 * x*x + 5*x - 3,
    -1.3
)
Solver.visualize_solution(x, residual, 'x')

#  |--                     |--
#  | sin(x+1) - y = 12     | x = 1 - cos(y)/2
# --                   -> --
#  | 2x + cos(y) = 2       | y = sin(x + 1) - 12
#  |--                     |--
x, residual = Solver.solve_nonlinear_system_equations_by_simple_iteration_method(
    np.array([lambda x, y: 1 - np.cos(y) / 2, lambda x, y: np.sin(x + 1) - 12]),
    np.array([0, 0]),
    1e-3
)
Solver.visualize_solution(x, residual, '(x, y)')

#  |--                     |--
#  | sin(x+1) - y = 12     | sin(x+1) - y - 12 = 0
# --                   -> --
#  | 2x + cos(y) = 2       | cos(y) + 2x - 2 = 0
#  |--                     |--
x, residual = Solver.solve_nonlinear_system_equations_by_newton_method(
    np.array([lambda x, y: np.sin(x + 1) - y - 12, lambda x, y: np.cos(y) + 2 * x - 2]),
    np.array([0, 0]),
    1e-3
)
Solver.visualize_solution(x, residual, '(x, y)')
