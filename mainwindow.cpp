#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->line_condition, SIGNAL(editingFinished()), SLOT(first_last_condition_slot()));
    connect(ui->pushButton_gen_table, SIGNAL(clicked()), SLOT(gen_table_slot()));
    connect(ui->comboBox_first, SIGNAL(currentTextChanged(QString)), this, SLOT(first_symbol_slot(QString)));
    connect(ui->comboBox_exit, SIGNAL(currentTextChanged(QString)), this, SLOT(exit_symbol_slot(QString)));
    connect(ui->pushButton_start, SIGNAL(clicked()),SLOT(start_slot()));
    //test
    QVector<QString> iinputt;
    iinputt << "p" << "q" << "q";
    deter_state_mach << iinputt;
    iinputt.clear();
    iinputt << "r" << "q" << "p";
    deter_state_mach << iinputt;
    iinputt.clear();
    iinputt << "r" << "r" << "r";
    deter_state_mach << iinputt;
    input_symbols << "0" << "1" << "2";
    condition_symbols << "q" << "p" << "r";
    first_rule_symb = "q";
    for(int i = 0; i < deter_state_mach.size(); i++){
        for(int j = 0; j < deter_state_mach[i].size(); j++){
            rules_map.insert(condition_symbols[i], input_symbols[j] + deter_state_mach[i][j]);
        }
    }
}
void MainWindow::first_last_condition_slot()
{
    ui->comboBox_first->clear();
    ui->comboBox_exit->clear();
    condition_symbols = ui->line_condition->text().split(",");
    for(int i = 0; i < condition_symbols.size(); i++){
        ui->comboBox_first->addItem(condition_symbols[i]);
        ui->comboBox_exit->addItem(condition_symbols[i]);
    }
}
void MainWindow::first_symbol_slot(QString s)
{
   first_rule_symb = s;
}
void MainWindow::exit_symbol_slot(QString s)
{
   exit_rule_symb = s;
}
void MainWindow::gen_table_slot()
{
    //?????
}
void MainWindow::start_slot()
{
    QStringList chains_need_check;
    input_symbols = ui->line_input->text().split(",");
    condition_symbols = ui->line_condition->text().split(",");
    if(ui->textEdit_input_chains->toPlainText() != ""){
        chains_need_check = ui->textEdit_input_chains->toPlainText().split("\n");
    }
    rules_map.insert(exit_rule_symb, "~");

    ui->textEdit_outpu_chains->clear();

    for(int i = 0; i < chains_need_check.size(); i++){
        chains_need_check[i] += "~";
    }

    for(int i = 0; i < chains_need_check.size(); i++){
        check_chain(chains_need_check[i]);
    }
}
void MainWindow::check_chain(QString chain_cur)
{
    bool word_check_chain[chain_cur.size()-1];
    for(int i = 0; i < chain_cur.size()-1; i++){
        word_check_chain[i] = false;
        for(int j = 0; j < input_symbols.size(); j++){
            if(chain_cur[i] == input_symbols[j]){
                word_check_chain[i] = true;
                break;
            }
        }
        if(!word_check_chain[i]){
            msgBox.setText("Цепочка " + chain_cur.leftRef(chain_cur.size()-1) + " состоит из неизвестных символов.");
            msgBox.exec();
            return;
        }
    }
    QStringList chains = rules_map.values(first_rule_symb);
    QStringList chains_next_step;
    QStringList answer;
    QString chain;
    QStringList rule_variants;
    bool flag_no_term = false, equal_chain_check = true;
    short int size_chain_term = 0;
    qDebug() << chains;
    for(; chains.size() > 0;){
        flag_no_term = false;
        size_chain_term = 0;
        for(int num_word_chain = 0; num_word_chain < chains[0].size(); num_word_chain++){
            if(!rules_map.uniqueKeys().contains((QString)chains[0][num_word_chain])){
                size_chain_term++;
            }else{
                flag_no_term = true;
            }
        }
        qDebug()<<flag_no_term;
        if(size_chain_term < chain_cur.size() && flag_no_term){
            for(int num_word_chain = chains[0].size()-1; num_word_chain >= 0; num_word_chain--){
                if(rules_map.uniqueKeys().contains((QString)chains[0][num_word_chain])){

                    rule_variants = rules_map.values((QString)chains[0][num_word_chain]);
                    qDebug()<< "VAriant" <<rule_variants<< " SYMB "<< chains[0][num_word_chain];
                    qDebug() << chains[0];
                    for (int i = 0; i < rule_variants.size(); i++){
                        equal_chain_check = true;
                        chain = chains[0].left(num_word_chain);
                        chain +=  rule_variants[i];
                        chain += chains[0].rightRef(chains[0].size()-num_word_chain-1);

                        for(int k = 0; k < chain.size()-1; k++){
                            if(chain[k] != chain_cur[k]){

                                equal_chain_check = false;
                                break;
                            }
                        }
                        //qDebug() << "CHAIN " <<chain << " CHAIN_CUr " << chain_cur << equal_chain_check;
                            if(equal_chain_check)
                                chains_next_step.push_back(chain);

                    }
                    qDebug() << chains_next_step;
                    break;
                }
            }
        }else if (!flag_no_term && chains[0] == chain_cur){
            answer.push_back(chains[0]);
        }
        chains.removeFirst();
        if(chains.size() == 0){
            chains = chains_next_step;
            chains_next_step.clear();
        }
    }
        for(int i = 0; i < answer.size(); i++){
            for(int j = 0; j < answer[i].size(); j++){
                if(answer[i][j] == '~'){
                    answer[i].remove(j,1);
                    j--;
                }
            }
        }
    qDebug() <<"\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\"<< answer;
    if(answer.isEmpty()){
        msgBox.setText("Цепочка " + chain_cur.leftRef(chain_cur.size()-1) + " не подходит.");
        msgBox.exec();
        return;
    }
    answer.removeDuplicates();
    for(int i = 0; i < answer.size(); i++){
        ui->textEdit_outpu_chains->append(answer[i]);
    }
}
MainWindow::~MainWindow()
{
    delete ui;
}

