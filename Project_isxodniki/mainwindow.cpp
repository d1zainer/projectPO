
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <bitset>
#include <string>
using namespace std;
#include <vector>
#include <fstream>
#include <cstdlib> // Для работы с функцией system()
#include <bits/stdc++.h>
#include <QMessageBox>

#include<QToolButton>
#include<QPixmap>
#include<QStyle>
#include<QApplication>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}



void tokenize(std::string const& str, const char delim, std::vector<std::string>& out)
{
    size_t start;
    size_t end = 0;

    while ((start = str.find_first_not_of(delim, end)) != std::string::npos)
    {
        end = str.find(delim, start);
        out.push_back(str.substr(start, end - start));
    }
}

void MainWindow::on_pushButton_clicked()
{


    string tok = ui ->vvodsig->text().toStdString();

    QString qs1 = QString::fromLocal8Bit(tok.c_str());
    if (qs1.isEmpty())
    {
        QMessageBox::warning(this, "Ошибка!", "Вы не ввели сигнатуру!");
    }
    else
    {
        const char delim = ' ';
        vector<std::string> out;
        tokenize(tok, delim, out);


        string signatura[8]{};
        int i = 0;
        for (auto &tok : out)
        {
            unsigned int g = std::stoul(tok, nullptr, 16);
            bitset <8> bitset = g;
            signatura[i] = bitset.to_string<char, char_traits<char>, allocator<char> >();
            i++;
        }


        string patch = ui ->vvodsig->text().toStdString() + ".vhd";
        ofstream fout;
        fout.open(patch);
        if (!fout.is_open())
        {
            QMessageBox::warning(this, "Ошибка!", "Ошибка при создании файла");
            //cout << "Ошибка открытия!" << endl;

        }
        else
        {
            fout << "library IEEE;" << endl;
            fout << endl;
            fout << "use IEEE.STD_LOGIC_1164.ALL;" << endl;
            fout << "entity fin_avt is" << endl;
            fout << "Port(clk   : in std_logic;" << endl;
            fout << "cnt: in std_logic_vector(7 downto 0);" << endl;
            fout << "rst: in std_logic;" << endl;
            fout << "fca: OUT std_logic_vector(4 downto 0));" << endl;
            fout << "end fin_avt;" << endl;
            fout << endl;
            fout << "architecture Behavioral of fin_avt is" << endl;
            fout << "TYPE state_type IS (Start, f1, f2, f3, f4, f5, PA, A); " << endl;
            fout << "SIGNAL  next_st    : state_type;" << endl;
            fout << "SIGNAL  present_st : state_type;" << endl;

            fout << "BEGIN" << endl;
            fout << "state_proc: PROCESS (present_st, cnt)" << endl;
            fout << "BEGIN" << endl;
            fout << "CASE present_st IS" << endl;

            fout << "WHEN Start =>" << endl;
            fout << "IF cnt = \"" + signatura[0] + '"'<< endl;
            fout << "THEN next_st <= f1" << endl;
            fout << "next_st <= Start;;" << endl;
            fout << "END IF;" << endl;

            fout << "WHEN f1 =>" << endl;
            fout << "IF cnt = \"" + signatura[1] + '"' << endl;
            fout << "THEN next_st <= f2" << endl;
            fout << "next_st <= Start;;" << endl;
            fout << "END IF;" << endl;

            fout << "WHEN f2 =>" << endl;
            fout << "IF cnt = \"" + signatura[2] + '"' << endl;
            fout << "THEN next_st <= f3" << endl;
            fout << "next_st <= Start;;" << endl;
            fout << "END IF;" << endl;

            fout << "WHEN f3 =>" << endl;
            fout << "IF cnt = \"" + signatura[3] + '"' << endl;
            fout << "THEN next_st <= f4" << endl;
            fout << "next_st <= Start;;" << endl;
            fout << "END IF;" << endl;

            fout << "WHEN f4 =>" << endl;
            fout << "IF cnt = \"" + signatura[4] + '"' << endl;
            fout << "THEN next_st <= f5" << endl;
            fout << "next_st <= Start;;" << endl;
            fout << "END IF;" << endl;

            fout << "WHEN f5 =>" << endl;
            fout << "IF cnt = \"" + signatura[5] + '"' << endl;
            fout << "THEN next_st <= f6" << endl;
            fout << "next_st <= Start;;" << endl;
            fout << "END IF;" << endl;

            fout << "WHEN f6 =>" << endl;
            fout << "IF cnt = \"" + signatura[6] + '"' << endl;
            fout << "THEN next_st <= PA" << endl;
            fout << "next_st <= Start;;" << endl;
            fout << "END IF;" << endl;

            fout << "WHEN PA =>" << endl;
            fout << "IF cnt = \"" + signatura[7] + '"' << endl;
            fout << "THEN next_st <= A" << endl;
            fout << "next_st <= Start;;" << endl;
            fout << "END IF;" << endl;

            fout << "WHEN OTHERS = " << endl;
            fout << "next_st = Start;" << endl;
            fout << "END CASE;" << endl;
            fout << "END PROCESS;" << endl;
            fout << "next_st_proc" << endl;
            fout << "PROCESS (clk, rst)" << endl;
            fout << "BEGIN" << endl;
            fout << "IF rst = '1'" << endl;
            fout << "THEN present_st = Start;" << endl;
            fout << "ELSIF (rising_edge(clk))" << endl;
            fout << "THEN present_st = next_st;" << endl;
            fout << "END IF;" << endl;

            fout << "END PROCESS;" << endl;
            fout << "out_proc" << endl;
            fout << "PROCESS (present_st)" << endl;
            fout << "BEGIN" << endl;
            fout << "CASE present_st IS" << endl;
            fout << "WHEN Start =" << endl;
            fout << "fca = 00000;" << endl;
            fout << "WHEN f1 =" << endl;
            fout << "fca = 00001;" << endl;
            fout << "WHEN f2 =" << endl;
            fout << "fca = 00010;" << endl;
            fout << "WHEN f3 =" << endl;

            fout << "fca = 00011;" << endl;
            fout << "WHEN f4 =" << endl;
            fout << "fca = 00100;" << endl;
            fout << "WHEN f5 =" << endl;
            fout << "fca = 00101;" << endl;
            fout << "WHEN PA =" << endl;
            fout << "fca = 01110;;" << endl;
            fout << "WHEN A =" << endl;
            fout << "fca = 11111;" << endl;
            fout << endl;
            fout << "END CASE;" << endl;
            fout << endl;
            fout << "END PROCESS;" << endl;
            fout << endl;
            fout << "END PROCESS;" << endl;
            fout << endl;
            QString qs = QString::fromLocal8Bit(patch.c_str());
            ui ->result->setText(qs);
            QMessageBox::about(this, "Результат", "Запись прошла успешно!");

            cout << "Запись прошла успешно. Результат был записан в файл - " + patch << endl;
        }
        fout.close();
        ui ->vvodsig->clear();
        ui ->result->clear();

    }

}


void MainWindow::on_pushButton_Clear_clicked()
{
    ui ->vvodsig->clear();
}


void MainWindow::on_pushButton_Clear_2_clicked()
{
    QMessageBox::about(this, "Инструкция", "Данная программа предназначена для генерации VHDL кода, описывающего поведение FGPA-аппаратуры. На вход мы получаем 8-ми байтовую сигнатуру, а на выходе получваем файл с расширением .vhd, описывающий поведение устройства.");
}

