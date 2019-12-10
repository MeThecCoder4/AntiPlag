#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <map>
#include <QStringListModel>

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

    std::map<QString, int> mapIdentifiers(const QString& text);

    int calculateCoincidence(const std::map<QString, int>& map1, const std::map<QString, int>& map2);

    QStringListModel* m_listModel;

private slots:
    void compareTexts();

};
#endif // MAINWINDOW_H
