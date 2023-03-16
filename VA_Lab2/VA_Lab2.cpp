#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <conio.h>
using namespace std;
int num_str = 0;    //количество строк в файле

double** Mas1;  //динамический массив под исходную матрицу (n*n)
double* Mas2;   //динамический массив под b (1*n)
double** Mas3;  //динамический массив для размещения 1 и 2 исходных матриц
double* Mas4_1;   //динамический массив под решение (X)
double* Mas4_2;   //динамический массив под решение (X)
double* Mas5_1;   //динамический массив под невязки Якоби
double* Mas5_2;   //динамический массив под невязки Зейделя

double e = 0 ;       //значение точности

//генерация случайных значений матрицы
void generate_matrx()
{
    for (int i = 0; i < num_str; i++)
    {
        for (int j = 0; j < num_str; j++)
        {
            //Mas1[i][j] = rand() % (25 + 1);     //генерация чисел в заданном промежутке (0, 25)
            Mas1[i][j] = -25 + rand() % (50 + 1);     //генерация чисел в заданном промежутке (-25 25)
            Mas3[i][j] = Mas1[i][j];
        }
    }

    //генерация вектора b
    for (int i = 0; i < num_str; i++)
    {
        Mas2[i] = -25 + rand() % (50 + 1);     //генерация чисел в заданном промежутке (-25 25)
        //Mas2[i] = rand() % (25 + 1);     //генерация чисел в заданном промежутке
        Mas3[i][num_str] = Mas2[i];
    }
}

//Проверка форматирования файла
int check_file()
{
    ifstream file("file.txt");
    if (!file)      //проверка открытия файла
    {
        cout << "Файл не открыт\n\n";       //ошибки при открытии файла
        return -1;
    }
    else          //файл открыт без ошибок
    {
        char* str = new char[1024];
        int i = 0;
        while (!file.eof())
        {
            file.getline(str, 1024, '\n');
            i++;        //подсчет количества строк
        }
        num_str = i;
        if ((num_str - 1) % 2 == 0)
        {
            file.clear();
            file.seekg(0, ios_base::beg);   //переход в начало файла
            float ch;
            int count = 0;
            while (!file.eof()) //считать общее количество чисел в файле
            {
                file >> ch;
                count++;        //подсчет чисел (счетчик)
            }
            file.close();       //закрытие файла
            count = count - (num_str - 1) / 2 - 1;
            if (pow((num_str - 1) / 2, 2) == count) {
                num_str = (num_str - 1) / 2;      //размерность матрицы
                return 1;
            }
            return -1;
        }
        return -1;
    }
    return 0;
}

//Заполнение динамических массивов данными из файла
void read_matrx()
{
    ifstream file("file.txt");
    double temp;
    for (int i = 0; i < num_str; i++)       //заполнение первой матрицы
    {
        for (int j = 0; j < num_str; j++)
        {
            file >> temp;
            Mas1[i][j] = temp;;
            Mas3[i][j] = temp;;
        }
    }
    for (int i = 0; i < num_str; i++)       //заполнение второй матрицы
    {
        file >> temp;
        Mas2[i] = temp;
        Mas3[i][num_str] = temp;      //дополнение к первой матрице
    }
    file >> e;      //чтение точности
    file.close();       //закрытие файла
}


//вывод матрицы
void print_matrx(double** Mas, int func)
{
    int temp;
    for (int i = 0; i < num_str; i++)
    {
        if (func == 1)
        {
            temp = (num_str);
        }
        else
        {
            temp = num_str + 1;
        }
        for (int j = 0; j < temp; j++)
        {
            if (Mas[i][j] > 99)
            {
                printf(" %.0f   ", Mas[i][j]);
            }
            else if (Mas[i][j] > 9)
            {
                printf(" %.1f   ", Mas[i][j]);
            }
            else if ((Mas[i][j] >= 0) && (Mas[i][j] < 10))
            {
                printf(" %.2f   ", Mas[i][j]);
            }
            else if (Mas[i][j] > -10)
            {
                printf("%.2f   ", Mas[i][j]);
            }
            else if (Mas[i][j] > -100)
            {
                printf("%.1f   ", Mas[i][j]);
            }
        }
        printf("\n");
    }
}

void print_residuals(double* Mas5)
{
    for (int i=0;i<num_str;i++)
    {
        cout << Mas5[i] << "\n";
    }
}

bool check_dia()
{
    for (int i = 0; i < num_str; i++)
    {
        if (Mas3[i][i] == 0)
        {
            return false;
        }
    }
    return true;
}

void init_matrx()
{
    Mas1 = new double* [num_str];       //создание динамического массива для начальной матрицы
    for (int i = 0; i < num_str; i++)
    {
        Mas1[i] = new double[num_str];
    }

    Mas2 = new double[num_str];        //создание динамического массива под вектор b

    Mas3 = new double* [num_str + 1];       //создание динамического массива для размещения 1 и 2 исходных матриц
    for (int i = 0; i < num_str + 1; i++)
    {
        Mas3[i] = new double[num_str];
    }

    Mas4_1 = new double[num_str];        //создание динамического массива
    Mas4_2 = new double[num_str];        //создание динамического массива
    Mas5_1 = new double[num_str];        //создание динамического массива под невязки Якоби
    Mas5_2 = new double[num_str];        //создание динамического массива под невязки Зейделя
}

bool check_e(double* Mas4_copy, double* Mas4)
{
    double num = 0;
    double temp;
    for (int i = 0; i < num_str && num < e; i++)
    {
        temp = abs(Mas4_copy[i] - Mas4[i]);
        if (temp > num)     //подсчет максимального значения разности
        {
            num = temp;
        }
    }

    if (num < e)
    {
        return true;
    }
    return false;
}

int jacob(int max_num_iter)
{
    int num_iter = 0;
    double* Mas4_copy = new double[num_str];      //динамический массив для исходной матрицы b
    for (int i = 0; i < num_str; i++)
    {
        Mas4_1[i] = Mas2[i] / Mas3[i][i];
        Mas4_2[i] = Mas2[i] / Mas3[i][i];
        //cout << Mas4[i] << "  ";
    }

    while (num_iter < max_num_iter && num_iter >= 0)
    {
        for (int i = 0; i < num_str; i++)
        {
            Mas4_copy[i] = Mas4_1[i];
        }

        for (int i = 0; i < num_str; i++)
        {
            double summa = 0;
            for (int j = 0; j < num_str; j++)
            {
                if (i != j)
                {
                    summa = summa + Mas3[i][j] * Mas4_copy[j];
                    
                }
            }
            Mas4_1[i] = (Mas2[i] - summa) / Mas3[i][i];
            //cout << Mas4[i];
            if (Mas4_1[i] == INFINITY)
            {
                num_iter = max_num_iter - 1;
            }
        }
        num_iter = num_iter + 1;
        if (check_e(Mas4_copy, Mas4_1))
        {
            break;
        }
        if (num_iter == max_num_iter)
        {
            num_iter = -1;
        }
    }
    return num_iter;
}

int Zeidel(int max_num_iter)
{
    int num_iter = 0;
    double* Mas4_copy = new double[num_str];      //динамический массив для исходной матрицы b
    for (int i = 0; i < num_str; i++)
    {
        //Mas4_2[i] = Mas2[i] / Mas3[i][i];
    }

    while (num_iter < max_num_iter && num_iter >= 0)
    {
        for (int i = 0; i < num_str; i++)
        {
            Mas4_copy[i] = Mas4_2[i];
        }

        for (int i = 0; i < num_str; i++)
        {
            double sum = 0;

            for (int j = 0; j < num_str; j++)
            {
                if (i != j)
                {
                    sum += Mas3[i][j] * Mas4_2[j];
                }
            }

            Mas4_2[i] = (Mas3[i][num_str] - sum) / Mas3[i][i];

            if (Mas4_2[i] == INFINITY)
            {
                num_iter = max_num_iter - 1;
                break;
            }
        }

        num_iter = num_iter + 1;
        if (check_e(Mas4_copy, Mas4_2))
        {
            break;
        }
        if (num_iter == max_num_iter)
        {
            num_iter = -1;
        }


    }
    return num_iter;
}

//определение невязок
void residuals(double* Mas4, double* Mas5)
{
    for (int i = 0; i < num_str; i++)
    {

        Mas5[i] = Mas3[i][num_str];

        for (int j = 0; j < num_str; j++)
        {
            Mas5[i] = Mas5[i] - Mas3[i][j] * Mas4[j];
        }
    }
}

//определение норм невязок
void normal_residuals(double* Mas5, double* normal)
{
    *normal = fabs(Mas5[0]);
    for (int i = 0; i < num_str - 1; i++)
    {
        if (fabs(Mas5[i + 1]) > *normal)
        {
            *normal = fabs(Mas5[i + 1]);
        }
    }
}

int write_good(int num_iter, double* Mas4, double* Mas5)
{
    //вывод количества итераций
    cout << "Количество итераций: " << num_iter;

    //вывод массива 
    cout << "\n\nМассив X:\n";
    print_residuals(Mas4);

    //вывод невязок
    cout << "\n\nНевязки:\n";
    residuals(Mas4, Mas5);
    print_residuals(Mas5);

    //вывод нормы невязок
    double normal_residuals_d;
    normal_residuals(Mas5_1, &normal_residuals_d);
    cout << "\n\nНорма невязок: " << normal_residuals_d;
    
    //запись в файл
    std::ofstream out;
    out.open("file_rez.txt");
    if (out.is_open())  //файл открыт
    {
        out << "Исходная матрица:" << endl;
        for (int i = 0; i < num_str; i++)
        {
            for (int j = 0; j < num_str; j++)
            {
                out << Mas3[i][j] << " ";
            }
            out << endl;
        }

        out << endl << "Исходный вектор b:" << endl;
        for (int i = 0; i < num_str; i++)
        {
            out << Mas2[i] << endl;
        }
        
        out << endl << "Вектор X:" << endl;
        for (int i = 0; i < num_str; i++)
        {
            out << Mas4[i] << endl;
        }

        out << endl << "Невязки" << endl;
        for (int i = 0; i < num_str; i++) 
        {
            out << Mas5[i] << endl;
        }
        
        out << endl << "Норма невязок: " << normal_residuals_d << endl;
        out.close();        //закрытие файла
    }
    else           //при открытии файла произошла ошибка
    {
        cout << "\n\nПри записи в файл произошла ошибка!";
        return 0;
    }
    return 1;
}

int main()
{
    setlocale(LC_ALL, "Rus");
    srand(time(NULL));

    int func;
    do
    {
        system("cls");      //очистка консоли
        cout << "1) Чтение данных из файла\n\n2) Генерация матрицы случайным образом";
        func = _getch();
    } while (func != 49 && func != 50);

    system("cls");      //очистка консоли

    cout << "Введите максимальное количество итераций: ";
    int max_num_iter;
    do
    {
        scanf("%d", &max_num_iter);
    } while (max_num_iter < 0 || max_num_iter > 100000);

    if (func == 49)     //чтение данных из файла
    {
        if (check_file() == 1)
        {
            //cout << "Файл имеет правильное форматирование!";

            init_matrx();   //инициализация массивов под считанную размерность из файла
            read_matrx();   //заполнение массивов данными
        }
        else
        {
            cout << "Файл имеет неверное форматирование!";
            return 0;
        }
    }
    if (func == 50)     //случайная генерация значений
    {
        do
        {
            system("cls");      //очистка консоли
            cout << "Введите размерность матрицы: ";
            scanf("%d", &num_str);
        } while (num_str <= 0);
        
        
        do
        {
            system("cls");      //очистка консоли
            cout << "Введите значение точности: ";
            scanf("%lf", &e);
        } while (e > 1);
        
        system("cls");      //очистка консоли

        init_matrx();   //инициализация массивов под заданную размерность

        generate_matrx();   //заполнение матрицы случайными значениями
    }

    system("cls");      //очистка консоли
    cout << "Исходная матрица:\n";

    print_matrx(Mas3, 1);

    cout << "\nВектор b:\n";

    print_residuals(Mas2);


    cout << "\n\nТочность: " << e << "\n" << "\nМаксимальное количество итераций: " << max_num_iter;


    if (!check_dia())
    {
        cout << "\n\n\n!!!На диагонали обнаружены нулевые элементы!!!";
        return 0;
    }
    int num_iter = 0;


    //Вычисление по методу Якоби
    num_iter = jacob(max_num_iter);

    cout << "\n\n\nВычисление по методу Якоби:\n\n";

    
    if (num_iter < 0)
    {
        cout << "Решение расходится, т.к. количество итераций < 0";
    }
    else
    {
        write_good(num_iter, Mas4_1, Mas5_1);
    }

    //Вычисление по методу Зейделя
    cout << "\n\n\nВычисление по методу Зейделя:\n\n";

    num_iter = Zeidel(max_num_iter);
    if (num_iter < 0)
    {
        cout << "Решение расходится, т.к. количество итераций < 0";
    }
    else
    {
        write_good(num_iter, Mas4_2, Mas5_2);
    }
    
    cout << "\n\nДля выхода нажмите любую клавишу\n\n";
    _getch();
}