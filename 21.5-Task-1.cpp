#include <iostream>
#include <fstream>
#include <string>

const std::string fileName = "statements.txt";

struct data
{
    std::string name = "unknown";
    std::string family = "unknown";
    std::string date = "01.01.1900";
    int amount = 0;
};

//Функция проверки строки с датой
bool check_date(std::string &date)
{
    //Проверка длины строки,наличия разделительных точек, что день месяц и год - числа
    if (date[2] != '.' || date[5] != '.' || date.length() != 10 
    || !isdigit(date[0]) || !isdigit(date[1]) || !isdigit(date[3]) || !isdigit(date[4])
    || !isdigit(date[6]) || !isdigit(date[7]) || !isdigit(date[8]) || !isdigit(date[9]))
    {
        std::cerr << std::endl << "Date format not validated!" << std::endl;
        return false;
    }
    else
    {
        //Выделяем из строки день и месяц
        int DD = stoi (date.substr(0,2));
        int MM = stoi (date.substr(3,2));

        //Проверка корректность ввода дня и месяца
        int dayInMonth[12] = {31,28,31,30,31,30,31,31,30,31,30,31};
        if(MM < 1 || MM > 12 || DD < 1 || DD > dayInMonth[MM-1]) 
        {
            std::cerr << std::endl << "Date format not validated!" << std::endl;
            return false;
        }     
    }

    return true;
}

//Функция проверки строки с целым числом
bool check_int(std::string &num)
{
    //Проверка длины строки с числом
    if (num.length() > 9)
    {
        std::cerr << std::endl << "Error! The number is not a positive integer!" << std::endl;
        return false;
    }
    else 
    {
        //Проверка что все символы являются числами
        for (int i = 0; i < num.length(); i++)
        {
            if (!isdigit(num[i]))
                {
                    std::cerr << std::endl << "Error! The number is not a positive integer!" << std::endl;
                    return false;
                }
        }
    }
    return true;
}

//Функция чтения ведомости 
void list ()
{
    data field;

    std::string amount;

    //Открываем файл для чтения
    std::ifstream file (fileName);
    if (!file.is_open())
    {
        std::ofstream file(fileName);
        if (!file.is_open())
        {
            std::cerr << "Error create file" << std::endl;
            exit(1);
        }
        std::cout << "File not found! Create new file. " << std::endl;
        file.close();
        return;
    }

    std::cout << std::endl << "----- Begining of file '" << fileName << "' -----" << std::endl;

    int i=0;

    while (!file.eof())
    {
        i++;
        
        //Считывание и вывод имени
        file >> field.name;
        //Проверка пустой строки
        if(file.eof()) break;
        std::cout << i << ". " << field.name << " ";
        
        //Считывание и вывод фамилии
        file >> field.family;
        std::cout << field.family << " ";

        //Считывание и вывод даты
        file >> field.date; 
        //Проверка формата считанной даты
        if (check_date(field.date)) std::cout << field.date << " ";
        else
        {
            std::cerr << "Wrong date format! Reading from file aborted!\n" << std::endl;
            file.close();
            return;
        }

        //Считывание и вывод суммы выплаты
        file >> amount;
        //Проверка считанной суммы
        if (check_int(amount)) 
        {
            field.amount = stoi (amount);
            std::cout << field.amount << std::endl;
        }
        else
        {
            std::cerr << "Wrong amount format! Reading from file aborted!\n" << std::endl;
            file.close();
            return;
        }
    }
    std::cout << "----- End of file '" << fileName << "' -----" << std::endl << std::endl;
    std::cout << "Reading from file completed successfully!\n" << std::endl;
    file.close();
    
    return;
}

void add()
{
    data field;

    std::string amount;

    //Ввод имени и фамилии
    std::cout << "Enter your name: ";
    std::getline(std::cin, field.name);
    std::cout << "Enter last name: ";
    std::getline(std::cin, field.family);

    bool error = false;

    //Ввод даты
    do
    {
        error = false;

        std::cout << "Enter date (DD.MM.YYYY): ";
        std::getline(std::cin, field.date);

        if (!check_date(field.date)) error = true;
    } while (error);

    //Ввод суммы выплаты
    do
    {
        std::cout << "Enter payout amount: ";
        std::getline(std::cin, amount);
    } while (!check_int(amount));
    field.amount = stoi(amount);

    //Запись в файл    
    std::ofstream file;
    file.open (fileName, std::ios::app);
    if (!file.is_open())
    {
        std::cerr << "Error open file\n";
        return;
    }
    file << field.name << " " << field.family << " " << field.date << " " << field.amount << std::endl;
    file.close();
    std::cout << "---Data recorded!---\n" << std::endl;    
}

int main()
{
    std::cout <<"-----Accounting program-----\n" << std::endl;

    //Флаг конца программы
    bool endProg = false;
    
    do
    {
        std::string command;
        std::cout << "Enter:" << std::endl;
        std::cout << "- 'list' to read the list" << std::endl;
        std::cout << "- 'add' to write data to the list" << std::endl;
        std::cout << "- 'quit' to exit the program" << std::endl;
        std::getline(std::cin, command);

        if (command == "list")
        {
            //Вызов функции чтения из файла
            list();
        }

        else if (command == "add")
        {
            //Вызов функции добавления новой записи в ведомость
            add();
        }
        
        else if (command == "quit") endProg = true;

        else std::cerr << "Error! Repeat input." << std::endl;
    } while (!endProg);

    std::cout << std::endl << "Program completed. Press any key...";
    std::cin.get();
    return 1;
}