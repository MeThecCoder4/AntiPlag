#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStringListModel>
#include <QModelIndex>

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

    QStringListModel* m_listModel;

    QString m_lastText1, m_lastText2;

private slots:
    void compareTexts();

    void markTextsWords(const QModelIndex& index);

};
#endif // MAINWINDOW_H
