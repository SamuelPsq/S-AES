#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QPushButton>
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QMessageBox>
#include <QFile>
#include <QtGui>
#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <iostream>
#include <vector>
#include<QTextEdit>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void encrypt();
    void decrypt();
    bool isBinaryString(const QString &str);
        int** convertToIntArray(const QString &str);
        QString convertToString(int** array);
private:
    Ui::MainWindow *ui;
    QTextEdit *plaintextEdit;
        QTextEdit *keyEdit;
        QPushButton *encryptButton;
        QPushButton *decryptButton;
        QTextEdit *ciphertextEdit;
        QTextEdit *decryptedPlaintextEdit;
};

#endif // MAINWINDOW_H
