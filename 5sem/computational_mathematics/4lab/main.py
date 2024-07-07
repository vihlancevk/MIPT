import numpy as np
import matplotlib.pyplot as plt
from scipy.optimize import curve_fit


def calculate_newton_polynom(divided_differences: np.array, x_data: np.array, x: float) -> float:
    """
    P(x) = f(x_0) + f(x_0, x_1)(x - x_0) + f(x_0, x_1, x_2)(x - x_0)(x - x_1) ....
    """

    # Value of Newton Polynom in a given point x
    P_x: float = 0

    for i in range(len(divided_differences)):
        brackets_mul: float = 1
        for j in range(i):
            brackets_mul *= (x - x_data[j])

        brackets_mul *= divided_differences[i]
        P_x += brackets_mul

    return P_x


def calculate_newton_interpolation(data: np.array, x: list) -> np.array:
    """
    :param data: values to be interpolated
    :param x: arguments where data(x[i]) = data[i]
    :return:
    """

    # coefficients in P(x) described above
    f: np.array = np.zeros(len(data))

    divided_differences: list[list] = []

    diff_0: np.array = data.copy()
    divided_differences.append(diff_0)

    for i in range(len(data) - 1):
        diff: list[float] = []
        for j in range(len(divided_differences[i]) - 1):
            dev_diff: float = (divided_differences[i][j + 1] - divided_differences[i][j]) / (x[j + i + 1] - x[j])

            diff.append(dev_diff)

        divided_differences.append(diff)

    for i in range(len(divided_differences)):
        f[i] = divided_differences[i][0]

    return f


def display_newton_interpolation(data: np.array, x: np.array) -> None:
    f: np.array = calculate_newton_interpolation(data, x.tolist())

    plt.scatter(x, data)

    x_interpol: np.array = np.arange(min(x), max(x), 0.1)
    y_interpol: np.array = np.zeros(len(x_interpol))

    for i in range(len(x_interpol)):
        y_interpol[i] = calculate_newton_polynom(f, x, x_interpol[i])

    plt.plot(x_interpol, y_interpol, 'r--')

    plt.xlabel('Year')
    plt.ylabel('Population quantity in USA')

    plt.title('Interpolated data with Newton Polynom')

    plt.show()

    plt.close()

    predicted_val: float = calculate_newton_polynom(f, x, 2010)
    print('\nNewton Polynom predicted:', predicted_val)
    print(
        'Method error',
        round((np.abs(predicted_val - true_val) / true_val) * 100, 2),
        '%\n'
    )


def f_fitted(x, a, b):
    return a * np.exp(x * b)


def display_mls_interpolation(data: np.array, x: np.array) -> None:
    data: np.array = data / 10e7
    x: np.array = x / 10e2

    params, pcov = curve_fit(f_fitted, x, data)

    x_interpol: np.array = np.arange(x.min(), x.max(), 0.01, dtype=float)
    y_interpol: np.array = np.zeros(len(x_interpol), dtype=float)

    for i in range(len(x_interpol)):
        y_interpol[i]: float = f_fitted(x_interpol[i], *params)

    plt.scatter(x, data)

    plt.plot(x_interpol, y_interpol, 'r--')

    plt.xlabel('Year')
    plt.ylabel('Population quantity in USA')

    plt.title('Interpolated data with MLS, y = a*exp(b*x)')

    plt.show()

    plt.close()

    predicted_val = f_fitted(2.01, *params)
    true_scaled = true_val / 10e7
    print('\nMLS spline predicted:', predicted_val)
    print(
        'Method error',
        round((np.abs(predicted_val - true_scaled) / true_scaled) * 100, 2),
        '%\n'
    )


def evaluate_a_k(data: np.array, x: np.array) -> np.array:
    return data[: len(x) - 1]


def evaluate_c_k(data: np.array, x: np.array) -> np.array:
    # boundary conditions c_0, c_n = 0
    c: np.array = np.zeros(len(x))

    # we have n+1 points -> n intervals -> n-1 unknown c values
    A_size: int = len(x)
    A: np.array = np.zeros((A_size, A_size), dtype=float)

    f: np.array = np.zeros(A_size, dtype=float)

    h: np.array = np.zeros(len(x) - 1)
    for i in range(len(x) - 1):
        h[i] = x[i + 1] - x[i]

    for i in range(len(A[0]) - 2):
        A[i][i] = h[i]
        A[i][i + 1] = 2 * (h[i] + h[i + 1])
        A[i][i + 2] = h[i + 1]

        f[i] = 3 * (((data[i + 2] - data[i + 1]) / h[i + 1]) - ((data[i + 1] - data[i]) / h[i]))

    # Now we need to drop first and last columns + last two lines
    A = np.delete(A, A_size - 1, 0)
    A = np.delete(A, A_size - 2, 0)

    A = np.delete(A, 0, 1)
    A = np.delete(A, A_size - 2, 1)

    f = np.delete(f, A_size - 1, 0)
    f = np.delete(f, A_size - 2, 0)

    c = np.linalg.solve(A, f)
    c = np.insert(c, 0, 0.)
    c = np.append(c, 0.)

    return c


def evaluate_b_k(data: np.array, x: np.array, c: np.array) -> np.array:
    b: np.array = np.zeros(len(x) - 1)

    for i in range(len(b)):
        b[i] = ((data[i + 1] - data[i]) / (x[i + 1] - x[i])) - (x[i + 1] - x[i]) * (2 * c[i] + c[i + 1]) / 3

    return b


def evaluate_d_k(x: np.array, c: np.array) -> np.array:
    d: np.array = np.zeros(len(x) - 1)
    for i in range(len(d)):
        d[i] = (c[i + 1] - c[i]) / (3 * (x[i + 1] - x[i]))

    return d


def calculate_cubic_polynom(x: float, x_k: float, a: float, b: float, c: float, d: float) -> float:
    return a + b * (x - x_k) + c * (x - x_k) ** 2 + d * (x - x_k) ** 3


def display_cubic_spline_interpolation(data: np.array, x: np.array) -> None:
    a: np.array = evaluate_a_k(data, x)
    c: np.array = evaluate_c_k(data, x)
    b: np.array = evaluate_b_k(data, x, c)
    d: np.array = evaluate_d_k(x, c)

    plt.scatter(x, data)

    for i in range(len(a)):
        x_interpol: np.array = np.arange(x[i], x[i + 1], 0.1)
        y_interpol: np.array = np.zeros(len(x_interpol))

        for j in range(len(x_interpol)):
            y_interpol[j] = calculate_cubic_polynom(x_interpol[j], x[i], a[i], b[i], c[i], d[i])

        plt.plot(x_interpol, y_interpol, 'r--')

    plt.xlabel('Year')
    plt.ylabel('Population quantity in USA')

    plt.title('Interpolate data with classic Cubic Spline')

    plt.show()

    plt.close()

    predicted_val = calculate_cubic_polynom(
        2010, x[len(x) - 2], a[len(a) - 1], b[len(b) - 1], c[len(c) - 2], d[len(d) - 1]
    )
    print('\nCubic spline predicted:', predicted_val)
    print(
        'Method error',
        round((np.abs(predicted_val - true_val) / true_val) * 100, 2),
        '%\n'
    )


year_data: list[int] = [92_228_496,
                        106_021_537,
                        123_202_624,
                        132_164_569,
                        151_325_798,
                        179_323_175,
                        203_211_926,
                        226_545_805,
                        248_709_873,
                        281_421_906]

true_val: int = 308_745_538

ref_data: np.array = np.array(year_data, dtype=float)


if __name__ == '__main__':
    x_n: np.array = np.arange(1910, 2010, 10, dtype=float)

    display_newton_interpolation(ref_data.copy(), x_n.copy())

    display_mls_interpolation(ref_data.copy(), x_n.copy())

    display_cubic_spline_interpolation(ref_data.copy(), x_n.copy())
