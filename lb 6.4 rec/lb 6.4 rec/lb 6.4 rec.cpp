// Лабораторна робота № 6, завдання № 6.4, варіант 13 (рекурсивний спосіб)
//
// Одновимірний динамічний масив з n дійсних елементів.
// 1.1. Кількість елементів, що лежать в діапазоні від A до B.
// 1.2. Сума елементів, розташованих після максимального елемента.
// 2.   Впорядкувати парні елементи за спаданням модулів, зберігши
//      попереднє розташування інших елементів.
// Параметри n, A, B вводяться з клавіатури.
//
// Примітка: "парні елементи" - це елементи з парними індексами (0, 2, 4, ...),
// так само, як у прикладах з посібника (крок 2, починаючи з i = 0).
// Якщо максимум зустрічається кілька разів - береться перший.
//
// Усі дії над масивом виконують рекурсивні функції.
// Параметр i - індекс поточного елемента (на початку виклику - початковий індекс).

#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <string>

#ifdef _WIN32
#include <windows.h>
#endif

using namespace std;

// ---------------------------------------------------------------------
//  Прототипи
// ---------------------------------------------------------------------

// Створення та знищення динамічного масиву (не потребують рекурсії)
double* CreateArray(const int size);
void    DestroyArray(double*& a);

// Заповнення масиву випадковими числами з [low, high], елементи від i до кінця
void FillRandom(double* a, const int size, const double low, const double high, int i);

// Виведення масиву у вигляді рядка, елементи від i до кінця
void PrintArray(const double* a, const int size, int i);

// 1.1. Кількість елементів з діапазону [A, B], починаючи з індексу i
int CountInRange(const double* a, const int size, const double A, const double B, int i);

// Індекс першого максимального елемента серед a[i..size-1];
// imax - індекс найбільшого елемента серед уже переглянутих.
// Початковий виклик: IndexOfMax(a, size, 1, 0)
int IndexOfMax(const double* a, const int size, int i, int imax);

// 1.2. Сума елементів a[i..size-1].
// Для суми після максимуму: SumFrom(a, size, IndexOfMax(a, size, 1, 0) + 1)
double SumFrom(const double* a, const int size, int i);

// Індекс найбільшого за модулем елемента серед a[i], a[i+2], a[i+4], ...;
// imax - індекс найбільшого серед уже переглянутих
int IndexOfMaxModulusEven(const double* a, const int size, int i, int imax);

// 2. Впорядкування елементів з парними індексами за спаданням модулів,
//    починаючи з індексу i. Початковий виклик: SortEvenByModulusDesc(a, size, 0)
void SortEvenByModulusDesc(double* a, const int size, int i);

// ---------------------------------------------------------------------
//  main
// ---------------------------------------------------------------------
int main()
{
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif

    srand(static_cast<unsigned int>(time(nullptr)));

    int n;
    double A, B;

    cout << "Введіть n: ";
    cin >> n;
    cout << "Введіть A: ";
    cin >> A;
    cout << "Введіть B: ";
    cin >> B;

    if (n <= 0)
    {
        cout << "Помилка: n має бути додатним." << endl;
        return 1;
    }

    double* a = CreateArray(n);
    FillRandom(a, n, -10.0, 10.0, 0);

    cout << "\nПочатковий масив:" << endl;
    PrintArray(a, n, 0);

    // 1.1
    int count = CountInRange(a, n, A, B, 0);
    cout << "\n1.1. Кількість елементів в діапазоні [" << A << "; " << B
        << "] = " << count << endl;

    // 1.2
    int imax = IndexOfMax(a, n, 1, 0);
    double sum = SumFrom(a, n, imax + 1);
    cout << "1.2. Максимальний елемент a[" << imax << "] = " << a[imax]
        << ", сума елементів після нього = " << fixed << setprecision(2) << sum << endl;

    // 2
    SortEvenByModulusDesc(a, n, 0);
    cout << "\n2. Масив після впорядкування парних елементів за спаданням модулів:" << endl;
    PrintArray(a, n, 0);

    DestroyArray(a);
    return 0;
}

// ---------------------------------------------------------------------
//  Реалізація
// ---------------------------------------------------------------------

double* CreateArray(const int size)
{
    return new double[size];
}

void DestroyArray(double*& a)
{
    delete[] a;
    a = nullptr;
}

void FillRandom(double* a, const int size, const double low, const double high, int i)
{
    if (i < size)
    {
        a[i] = low + (high - low) * rand() / RAND_MAX;
        FillRandom(a, size, low, high, i + 1);
    }
}

void PrintArray(const double* a, const int size, int i)
{
    if (i < size)
    {
        cout << setw(8) << fixed << setprecision(2) << a[i];
        PrintArray(a, size, i + 1);
    }
    else
        cout << endl;
}

int CountInRange(const double* a, const int size, const double A, const double B, int i)
{
    if (i < size)
    {
        int here = (a[i] >= A && a[i] <= B) ? 1 : 0;
        return here + CountInRange(a, size, A, B, i + 1);
    }
    else
        return 0;
}

int IndexOfMax(const double* a, const int size, int i, int imax)
{
    if (i < size)
    {
        if (a[i] > a[imax])        // строге порівняння - лишається перший максимум
            imax = i;
        return IndexOfMax(a, size, i + 1, imax);
    }
    else
        return imax;
}

double SumFrom(const double* a, const int size, int i)
{
    if (i < size)
        return a[i] + SumFrom(a, size, i + 1);
    else
        return 0;
}

int IndexOfMaxModulusEven(const double* a, const int size, int i, int imax)
{
    if (i < size)
    {
        if (fabs(a[i]) > fabs(a[imax]))
            imax = i;
        return IndexOfMaxModulusEven(a, size, i + 2, imax);
    }
    else
        return imax;
}

void SortEvenByModulusDesc(double* a, const int size, int i)
{
    if (i < size)
    {
        int imax = IndexOfMaxModulusEven(a, size, i, i);   // найбільший за модулем серед a[i], a[i+2], ...

        double tmp = a[i];
        a[i] = a[imax];
        a[imax] = tmp;

        SortEvenByModulusDesc(a, size, i + 2);            // решта парних позицій
    }
}