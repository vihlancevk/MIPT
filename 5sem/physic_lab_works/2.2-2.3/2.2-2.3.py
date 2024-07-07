import numpy as np
import matplotlib.pyplot as plt

from scipy.optimize import curve_fit
from scipy.interpolate import make_interp_spline, BSpline


def func(x, a, b, c):
    return a * x**3 + b * x**2 + c


array_of_data_to_angle = np.array([2994, 2916, 2902  , 2850, 2836,
                                   2812, 2788, 2778  , 2740, 2734,
                                   2714, 2704, 2686  , 2674, 2666,
                                   2646, 2644, 2616  , 2606, 2586,
                                   2560, 2546, 2517.5, 2500, 2470,
                                   2456, 2278, 2240  , 2202, 2192,
                                   1854, 1192,  634])
array_of_data_to_wavelength = np.array([7032, 6929, 6907, 6717, 6678,
                                        6599, 6533, 6507, 6402, 6383,
                                        6334, 6305, 6267, 6234, 6217,
                                        6143, 6164, 6096, 6074, 6030,
                                        5976, 5945, 5882, 5852, 5791,
                                        5770, 5460, 5401, 5341, 5331,
                                        4916, 4358, 4047])

popt, pcov = curve_fit(func, array_of_data_to_angle, array_of_data_to_wavelength, maxfev=10**6)

# print('popt -', popt)

perr = np.sqrt(np.diag(pcov))

# print('perr -', perr)

# print('perr/popt -', perr/popt)


def foo(teta, n):
    lambda_n = func(teta, *popt)
    print('lambda_' + n + ' -', lambda_n)
    nu_n = (3*1e8) / (lambda_n*1e-10)
    print('nu_' + n + ' -', nu_n)
    print('hnu_' + n + ' -', 6.6260695729 * 1e-34 * nu_n * 6.242 * 1e18)


# foo(2620, '1,0')
# foo(2510, '1,5')
foo(1990, 'гр')

# plt.scatter(array_of_data_to_angle, array_of_data_to_wavelength, s=20, color='orange')
#
# array_of_angle = np.linspace(array_of_data_to_angle.min(), array_of_data_to_angle.max(), 1000)
# spl = make_interp_spline(np.flip(array_of_data_to_angle), np.flip(func(array_of_data_to_angle, *popt)), k=5)
# array_of_wavelength = spl(array_of_angle)
#
# plt.plot(array_of_angle, array_of_wavelength)
# plt.xlabel(r'$\theta,^\circ$', fontsize=12)
# plt.ylabel(r'$\lambda,\;\mathring{A}$', fontsize=12)
# plt.grid(True)
#
# plt.show()

array_of_H_lines_to_angle = np.array([1198, 1798, 2792])
array_of_H_lines_to_wavelength = func(array_of_H_lines_to_angle, *popt)

# print('array_of_H_lines_to_angle -', array_of_H_lines_to_angle)
# print('array_of_H_lines_to_wavelength -', array_of_H_lines_to_wavelength)
# print('1/array_of_H_lines_to_wavelength -', 1 / array_of_H_lines_to_wavelength)


def func(x, a, b):
    return a * x + b


x = np.array([0, 0.139, 0.188, 0.21])
y = np.array([0, 1.53, 2.09, 2.32])

popt, pcov = curve_fit(func, x, y)

# print('popt -', popt)

perr = np.sqrt(np.diag(pcov))

# print('perr -', perr)

# print('perr/popt -', perr/popt)

# plt.scatter(x[1:], y[1:], s=20, color='orange')
#
# new_x = np.linspace(x.min(), x.max(), 100)
# spl = make_interp_spline(x, func(x, *popt), k=3)
# new_y = spl(new_x)
#
# plt.plot(new_x, new_y)
# plt.xlabel(r'$ \frac{1}{n^2} - \frac{1}{m^2} $', fontsize=12)
# plt.ylabel(r'$ \frac{1}{\lambda_{mn}}, \; \mathring{A}^{-1}10^{-4} $', fontsize=12)
# plt.grid(True)
#
# plt.show()
