#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <regex>
#include <iostream>
#include "global_comparator.h"

using namespace std;
using namespace Qt;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_listModel = new QStringListModel(this);
    ui->listOfWords->setModel(m_listModel);

    connect(ui->btCompare, SIGNAL(released()), this, SLOT(compareTexts()));
    connect(ui->listOfWords, SIGNAL(clicked(const QModelIndex&)), this, SLOT(markTextsWords(const QModelIndex&)));
}

MainWindow::~MainWindow()
{
    if(m_listModel != nullptr)
        delete m_listModel;

    delete ui;
}

// This function is called as a slot of btCompare released() signal
void MainWindow::compareTexts()
{
    const QString text1 = ui->textEdit1->toPlainText().simplified();
    const QString text2 = ui->textEdit2->toPlainText().simplified();
    m_lastText1 = text1;
    m_lastText2 = text2;
    GlobalComparator gComparator(m_lastText1, m_lastText2);
    int coincidence = gComparator.compare();
    m_listModel->setStringList(gComparator.getUniqueWordsList());

    if(coincidence > 0)
        ui->coincidenceBar->setValue(coincidence);
    else
        ui->coincidenceBar->setValue(0);
}

// This function is called as a slot of clicked() signal on any of QListView children
void MainWindow::markTextsWords(const QModelIndex& index)
{
    QString modifiedText1, modifiedText2;
    QStringList wordsOfText1, wordsOfText2;
    QString clickedWord = index.data().toString();
    modifiedText1 = m_lastText1;
    modifiedText2 = m_lastText2;
    QRegExp rx('^' + clickedWord + '$');
    rx.setCaseSensitivity(CaseSensitivity::CaseInsensitive);
    wordsOfText1 = m_lastText1.split(QRegExp("[^A-Za-z0-9'\"]+"));
    wordsOfText2 = m_lastText2.split(QRegExp("[^A-Za-z0-9'\"]+"));

    for(const auto& word : wordsOfText1) {
        if(rx.exactMatch(word))
            modifiedText1 = modifiedText1.replace(word, "<font color=\"red\">" + word + "</font>");
    }

    for(const auto& word : wordsOfText2) {
        if(rx.exactMatch(word))
            modifiedText2 = modifiedText2.replace(word, "<font color=\"red\">" + word + "</font>");
    }

    ui->textEdit1->setHtml(modifiedText1);
    ui->textEdit2->setHtml(modifiedText2);
}

