import numpy as np

import matplotlib.pyplot as plt


class AbsoluteError:
    def __init__(self, f, f1, x, title):
        self.f = f
        self.f1 = f1
        self.x = x
        self.title = title
        self.h_values = np.array([2 / (2 ** n) for n in range(1, 22)])

    def _get_absolute_errors(self, formula):
        absolute_errors = []
        for h in self.h_values:
            absolute_error = abs(self.f1(self.x) - formula(h))
            if absolute_error != 0.0:
                absolute_errors.append(absolute_error)
            else:
                absolute_errors.append(min(absolute_errors))
        return np.array(absolute_errors)

    def build_graph(self):
        plt.title(self.title, fontsize=12)

        plt.plot(
            np.log2(self.h_values),
            np.log2(self._get_absolute_errors(lambda h: (self.f(self.x + h) - self.f(self.x)) / h)),
            color='red'
        )

        plt.plot(
            np.log2(self.h_values),
            np.log2(self._get_absolute_errors(lambda h: (self.f(self.x) - self.f(self.x - h)) / h)),
            color='orange'
        )

        plt.plot(
            np.log2(self.h_values),
            np.log2(self._get_absolute_errors(lambda h: (self.f(self.x + h) - self.f(self.x - h)) / (2 * h))),
            color='yellow'
        )

        plt.plot(
            np.log2(self.h_values),
            np.log2(
                self._get_absolute_errors(
                    lambda h: (4 / 3) * (self.f(self.x + h) - self.f(self.x - h)) / (2 * h) - \
                              (1 / 3) * (self.f(self.x + 2 * h) - self.f(self.x - 2 * h)) / (4 * h)
                )
            ),
            color='green'
        )

        plt.plot(
            np.log2(self.h_values),
            np.log2(
                self._get_absolute_errors(
                    lambda h: (3 / 2) * (self.f(self.x + h) - self.f(self.x - h)) / (2 * h) - \
                              (3 / 5) * (self.f(self.x + 2 * h) - self.f(self.x - 2 * h)) / (4 * h) + \
                              (1 / 10) * (self.f(self.x + 3 * h) - self.f(self.x - 3 * h)) / (6 * h)
                )
            ),
            color='blue'
        )

        plt.xlabel(r'$ \log_2(h), \; h = \frac{2}{2^n} \; (n = \overline{1, 21}) $', fontsize=12)
        plt.ylabel(r'$ \log_2(\Delta), \; \Delta = |f^{(1)}(x) - f_i(x, h)| $', fontsize=12)

        plt.legend(
            [
                r'$ f_1(x, h) = \frac{f(x + h) - f(x)}{h} $',
                r'$ f_1(x, h) = \frac{f(x) - f(x - h)}{h} $',
                r'$ f_1(x, h) = \frac{f(x + h) - f(x - h)}{2h} $',
                r'$ f_1(x, h) = \frac{4}{3} \cdot \frac{f(x + h) - f(x - h)}{2h} - \frac{1}{3} \cdot \frac{f(x + 2h) - f(x - 2h)}{4h} $',
                r'$ f_1(x, h) = \frac{3}{2} \cdot \frac{f(x + h) - f(x - h)}{2h} - \frac{3}{5} \cdot \frac{f(x + 2h) - f(x - 2h)}{4h} + \frac{1}{10} \cdot \frac{f(x + 3h) - f(x - 3h)}{6h} $',
            ]
        )

        plt.grid(True)

        plt.show()


if __name__ == '__main__':
    AbsoluteError(
        lambda x: np.sin(x ** 2),
        lambda x: 2 * x * np.cos(x ** 2),
        1,
        r'$ f = \sin(x^2), \; f^{(1)} = 2x \cdot \cos(x^2), \; x = 1 $'
    ).build_graph()

    AbsoluteError(
        lambda x: np.cos(np.sin(x)),
        lambda x: -1 * np.sin(np.sin(x)) * np.cos(x),
        1,
        r'$ f = \cos(\sin(x)), \; f^{(1)} = -\sin(\sin(x))\cos(x), \; x = 1 $'
    ).build_graph()

    AbsoluteError(
        lambda x: np.exp(np.sin(np.cos(x))),
        lambda x: -1 * np.exp(np.sin(np.cos(x))) * np.cos(np.cos(x)) * np.sin(x),
        1,
        r'$ f = e^{sin(cos(x))}, \; f^{(1)} = -e^{sin(cos(x))}\cos(\cos(x))\sin(x), \; x = 1 $'
    ).build_graph()

    AbsoluteError(
        lambda x: np.log(x + 3),
        lambda x: 1 / (x + 3),
        1,
        r'$ f = \ln(x + 3), \; f^{(1)} = \frac{1}{x + 3}, \; x = 1 $'
    ).build_graph()

    AbsoluteError(
        lambda x: (x + 3) ** 0.5,
        lambda x: 1 / (2 * np.sqrt(x + 3)),
        1,
        r'$ f = (x + 3)^{0.5}, \; f^{(1)} = \frac{1}{2\sqrt{x + 3}}, \; x = 1 $'
    ).build_graph()
