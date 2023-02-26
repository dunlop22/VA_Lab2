#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <conio.h>
using namespace std;
int num_str = 0;    //количество строк в файле

double** Mas1;  //динамический массив под исходную матрицу
double* Mas2;   //динамический массив под b
double** Mas3;  //динамический массив под итоговую матрицу
double* Mas4;   //динамический массив под решение (X)
double* Mas5;   //динамический массив под невязки

double e;       //значение точности

//генерация случайных значений матрицы
void generate_matrx()
{
    for (int i = 0; i < num_str; i++)
    {
        for (int j = 0; j < num_str; j++)
        {
            Mas1[i][j] = rand() % (25 + 1);     //генерация чисел в заданном промежутке
            Mas3[i][j] = Mas1[i][j];
        }
    }

    //генерация вектора b
    for (int i = 0; i < num_str; i++)
    {
        Mas2[i] = rand() % (25 + 1);     //генерация чисел в заданном промежутке
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

    Mas3 = new double* [num_str + 1];       //создание динамического массива для итоговой матрицы
    for (int i = 0; i < num_str + 1; i++)
    {
        Mas3[i] = new double[num_str];
    }
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
    if (func == 50)     //чтение данных из файла
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
            scanf("lf", &e);
        } while (e > 1);
        
        system("cls");      //очистка консоли

        init_matrx();   //инициализация массивов под заданную размерность

        generate_matrx();           //заполнение матрицы случайными значениями
    }

    print_matrx(Mas3, 1);

    if (!check_dia())
    {
        cout << "На диагонали обнаружены нулевые элементы";
        return 0;
    }

}
