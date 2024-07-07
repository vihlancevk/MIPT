import numpy as np


def calculate_value_of_definite_integral_by_trapezoidal_method(x: np.array, f_x: np.array, step: int = 1) -> float:
    n: int = len(x)

    I: float = 0

    k: int = 0
    while k < n - step:
        h_k = (x[k + step] - x[k])
        I += (h_k / 2) * (f_x[k + step] + f_x[k])

        k += step

    return I


def calculate_value_of_definite_integral_by_trapezoidal_method_with_richardson_extrapolation(x: np.array,
                                                                                             f_x: np.array) -> float:
    I_h: float = calculate_value_of_definite_integral_by_trapezoidal_method(x, f_x, 1)
    I_2h: float = calculate_value_of_definite_integral_by_trapezoidal_method(x, f_x, 2)

    I: float = I_h + (I_h - I_2h) / (2 ** 2 - 1)

    return I


def calculate_value_of_definite_integral_by_simpson_method(x: np.array, f_x: np.array) -> float:
    n: int = len(x) // 2

    I: float = 0

    for k in range(n):
        h_2k = (x[2 * k + 1] - x[2 * k])
        I += (h_2k / 3) * (f_x[2 * k] + 4 * f_x[2 * k + 1] + f_x[2 * k + 2])

    return I


if __name__ == '__main__':
    x: np.array = np.array([0, 0.25, 0.5, 0.75, 1.0, 1.25, 1.5, 1.75, 2])
    f_x: np.array = np.array([1.000000, 0.989616, 0.958851, 0.908852, 0.841471, 0.759188, 0.664997, 0.562278, 0.454649])

    I: float = calculate_value_of_definite_integral_by_trapezoidal_method(x, f_x)
    print('Метод трапеций:', I, '\n')

    I: float = calculate_value_of_definite_integral_by_trapezoidal_method_with_richardson_extrapolation(x, f_x)
    print('Метод трапеций с экстраполяцией Ричардсона:', I, '\n')

    I: float = calculate_value_of_definite_integral_by_simpson_method(x, f_x)
    print('Метод Симпсона:', I, '\n')
