#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <regex>
#include <iostream>
#include <set>

using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_listModel = new QStringListModel(this);
    ui->listOfWords->setModel(m_listModel);
    connect(ui->btCompare, SIGNAL(released()), this, SLOT(compareTexts()));
}

MainWindow::~MainWindow()
{
    if(m_listModel != nullptr)
        delete m_listModel;

    delete ui;
}

// Find all identifiers in text and count their occurances
std::map<QString, int> MainWindow::mapIdentifiers(const QString &text)
{
    if(text == nullptr || text.length() == 0)
        return map<QString, int>();

    QStringList identifiers = text.split(QRegExp("[^A-Za-z0-9]+"));
    map<QString, int> mapOfIdentifiers;

    for(const auto& id : identifiers)
        mapOfIdentifiers[id]++;

    return mapOfIdentifiers;
}

int MainWindow::calculateCoincidence(const std::map<QString, int> &map1, const std::map<QString, int> &map2)
{
    set<QString> uniqueElements;
    QStringList uniqueWordsList;
    uniqueElements.clear();

    for(const auto& entry : map1)
        if(!entry.first.isEmpty())
            uniqueElements.insert(entry.first);

    for(const auto& key : uniqueElements) {
        auto entry1 = map1.find(key);
        auto entry2 = map2.find(key);

        if(entry1 != map1.end() && entry2 != map2.end()) {
            if(entry1->second == entry2->second) {
                uniqueWordsList << entry1->first;
            }
        }
    }

    m_listModel->setStringList(uniqueWordsList);
    cout << uniqueWordsList.size() << ", " << uniqueElements.size() << endl;
    return static_cast<int>((static_cast<float>(uniqueWordsList.size()) / uniqueElements.size()) * 100);
}

void MainWindow::compareTexts()
{
    const QString text1 = ui->textEdit1->toPlainText().simplified();
    const QString text2 = ui->textEdit2->toPlainText().simplified();

    int coincidence = 0;
    map<QString, int> map1, map2;

    if((!(map1 = mapIdentifiers(ui->textEdit1->toPlainText())).empty()) &&
       (!(map2 = mapIdentifiers(ui->textEdit2->toPlainText())).empty()))
        coincidence = calculateCoincidence(map1, map2);

    if(coincidence > 0)
        ui->coincidenceBar->setValue(coincidence);
    else
        ui->coincidenceBar->setValue(0);
}

