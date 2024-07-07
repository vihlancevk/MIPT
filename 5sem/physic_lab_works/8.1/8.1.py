import numpy as np
import matplotlib.pyplot as plt

from scipy.optimize import curve_fit
from scipy.interpolate import make_interp_spline, BSpline


def func(x, a, b):
    return a * x + b


lg_T = np.log(np.array([1030, 1140, 1250, 1350, 1450]))
lg_W = np.log(np.array([2.44, 3.46, 5.57, 8.61, 9.46]))

popt, pcov = curve_fit(func, lg_T, lg_W, maxfev=10**6)

print(popt)

perr = np.sqrt(np.diag(pcov))

print(perr)

print(perr / popt)

plt.scatter(lg_T, lg_W, color='orange')

array_of_lg_T = np.linspace(lg_T.min(), lg_T.max(), 1000)
spl = make_interp_spline(lg_T, func(lg_T, *popt), k=3)
array_of_lg_W = spl(array_of_lg_T)

plt.plot(array_of_lg_T, array_of_lg_W)
plt.xlabel(r'$\lg{T}$', fontsize=12)
plt.ylabel(r'$\lg{W}$', fontsize=12)
plt.grid(True)

plt.show()
