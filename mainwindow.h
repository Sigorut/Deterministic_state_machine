#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QDebug>
#include <QStringList>
#include <QMultiMap>
#include <QMessageBox>
#include <QGridLayout>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QVector<QVector<QLineEdit*>> matrix; //Матрица переходов
    QVector<QLabel*> condition_input_name;
    QVector<QVector<QString>> deter_state_mach; //Считанная матрица переходов
    QMultiMap<QString, QString> rules_map; // ДКА в регулярном выражении
    QString first_rule_symb; // Начальное состояние ДКА
    QString exit_rule_symb; // Конечное состояние ДКА
    QStringList input_symbols; // Входы
    QStringList condition_symbols; // Состояния
    QGridLayout* matrix_layout;
    QMessageBox msgBox;

private:
    void check_chain(QString chain); //Проверка цепочки на вхождение в ДКА
    void get_data_from_matrix();
private slots:
    void first_last_condition_slot(); //Добавление в QComboBox
    void first_symbol_slot(QString s); //Выбор начального состояния
    void exit_symbol_slot(QString); // Выбор конечного состояния
    void gen_table_slot();  // Создание таблицы переходов
    void start_slot();
};
#endif // MAINWINDOW_H
