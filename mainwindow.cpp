#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <regex>
#include <iostream>

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

// Find all identifiers in text and count their occurances
map<QString, int> MainWindow::mapIdentifiers(const QString &text)
{
    if(text == nullptr || text.length() == 0)
        return map<QString, int>();

    QStringList identifiers = text.split(QRegExp("[^A-Z0-9]+"));
    map<QString, int> mapOfIdentifiers;

    for(const auto& id : identifiers)
        mapOfIdentifiers[id]++;

    return mapOfIdentifiers;
}

// This function checks how many words are the same in both texts (match is case insensitive)
// and returns the percentage of matched words
int MainWindow::calculateCoincidence(const std::map<QString, int> &map1, const std::map<QString, int> &map2)
{
    QStringList uniqueWordsList;
    m_uniqueElements.clear();

    for(const auto& entry : map1)
        if(!entry.first.isEmpty())
            m_uniqueElements.insert(entry.first);

    for(const auto& key : m_uniqueElements) {
        auto entry1 = map1.find(key);
        auto entry2 = map2.find(key);

        if(entry1 != map1.end() && entry2 != map2.end()) {
            if(entry1->second == entry2->second) {
                uniqueWordsList << entry1->first;
            }
        }
    }

    m_listModel->setStringList(uniqueWordsList);
    return static_cast<int>((static_cast<float>(uniqueWordsList.size()) / m_uniqueElements.size()) * 100);
}

// This function is called as a slot of btCompare released() signal
void MainWindow::compareTexts()
{
    const QString text1 = ui->textEdit1->toPlainText().simplified();
    const QString text2 = ui->textEdit2->toPlainText().simplified();
    m_text1 = text1;
    m_text2 = text2;

    int coincidence = 0;
    map<QString, int> map1, map2;

    if((!(map1 = mapIdentifiers(m_text1.toUpper())).empty()) &&
       (!(map2 = mapIdentifiers(m_text2.toUpper())).empty()))
        coincidence = calculateCoincidence(map1, map2);

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
    modifiedText1 = m_text1;
    modifiedText2 = m_text2;
    QRegExp rx('^' + clickedWord + '$');
    rx.setCaseSensitivity(CaseSensitivity::CaseInsensitive);
    wordsOfText1 = m_text1.split(QRegExp("[^A-Za-z0-9'\"]+"));
    wordsOfText2 = m_text2.split(QRegExp("[^A-Za-z0-9'\"]+"));

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

