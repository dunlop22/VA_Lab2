#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <conio.h>
using namespace std;
int num_str = 0;    //количество строк в файле

double** Mas1;     //динамический массив под исходную матрицу
double* Mas2;     //динамический массив под b
double** Mas3;     //динамический массив под итоговую матрицу
double* Mas4;     //динамический массив под решение (X)
double* Mas5;     //динамический массив под невязки


double e;   //значение точности

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
        if (num_str % 2 == 0)
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
            count = count - (num_str - 1) / 2;
            if (pow((num_str - 1) / 2, 2) == count) {
                return 1;
            }
            return -1;
        }
        return -1;
    }
    return 0;
}

//Заполнение динамических массивов данными из файла
void read_matrx(double* mas2)
{
    ifstream file("file.txt");
    double temp;
    for (int i = 0; i < num_str / 2; i++)       //заполнение первой матрицы
    {
        for (int j = 0; j < num_str / 2; j++)
        {
            file >> temp;
            Mas1[i][j] = temp;;
            Mas3[i][j] = temp;;
        }
    }
    for (int i = 0; i < num_str / 2; i++)       //заполнение второй матрицы
    {
        file >> temp;
        mas2[i] = temp;
        Mas3[i][num_str / 2] = temp;      //дополнение к первой матрице
    }
    file >> e;      //чтение точности
    file.close();       //закрытие файла
}


int main()
{
    std::cout << "Hello World!\n";
}
