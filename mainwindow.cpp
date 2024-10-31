#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QPushButton>
#include<QImage>
#include<QtMultimedia/QtMultimedia>
#include<QtMultimediaWidgets/QVideoWidget>
#include<QThread>
#include<QApplication>
#include<QTimer>
#include"head.h"
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // 创建明文输入框

      ui->plaintextEdit->setPlaceholderText("请输入明文/密文");
      ui->plaintextEdit->setGeometry(20, 50, 360, 80);

      // 创建密钥输入框

      ui->keyEdit->setPlaceholderText("请输入密钥");
      ui->keyEdit->setGeometry(20, 140, 360, 80);

      // 创建加密按钮

      ui->encryptButton->setGeometry(20, 230, 100, 40);

     // 创建解密按钮

      ui->decryptButton->setGeometry(280, 230, 100, 40);

      // 创建密文显示框

      ui->ciphertextEdit->setReadOnly(true);
      ui->ciphertextEdit->setPlaceholderText("加密后的密文将显示在这里");
      ui->ciphertextEdit->setGeometry(20, 280, 360, 80);

      // 创建解密后的明文显示框

      ui->decryptedPlaintextEdit->setReadOnly(true);
      ui->decryptedPlaintextEdit->setPlaceholderText("解密后的明文将显示在这里");
      ui->decryptedPlaintextEdit->setGeometry(20, 370, 360, 80);

      connect(ui->encryptButton, &QPushButton::clicked, this, &MainWindow::encrypt);
      connect(ui->decryptButton, &QPushButton::clicked, this, &MainWindow::decrypt);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete ui->plaintextEdit;
    delete ui->keyEdit;
    delete ui->encryptButton;
    delete ui->decryptButton;
    delete ui->ciphertextEdit;
    delete ui->decryptedPlaintextEdit;
}
void MainWindow::encrypt()
{
    QString plaintext = ui->plaintextEdit->toPlainText();
    QString key = ui->keyEdit->toPlainText();

    // 验证输入是否为有效的二进制字符串
    if (!isBinaryString(plaintext) ||!isBinaryString(key)) {
        QMessageBox::warning(this, "错误", "明文和密钥必须是有效的二进制字符串。");
        return;
    }

    // 将 QString 转换为 int** 形式的明文和密钥
    int** mingwen = convertToIntArray(plaintext);
    int** keyArray = convertToIntArray(key);

    // 密钥扩展
    int** key1 = new int*[2];
    for (int i = 0; i < 2; i++)
        key1[i] = new int[8];
    int** key2 = new int*[2];
    for (int i = 0; i < 2; i++)
        key2[i] = new int[8];

    // 使用现有函数进行密钥扩展
    key1[0] = yihuo8(keyArray[0], g(keyArray[1], rcon1));
    key1[1] = yihuo8(key1[0], keyArray[1]);
    key2[0] = yihuo8(key1[0], g(key1[1], rcon2));
    key2[1] = yihuo8(key2[0], key1[1]);

    // 加密
    lunmiyaojia(mingwen, keyArray);
    s_he_tihuan(mingwen[0]);
    s_he_tihuan(mingwen[1]);
    zuoyi(mingwen);
    liehunxiao(mingwen);
    lunmiyaojia(mingwen, key1);
    s_he_tihuan(mingwen[0]);
    s_he_tihuan(mingwen[1]);
    zuoyi(mingwen);
    lunmiyaojia(mingwen, key2);

    // 将加密后的密文转换为 QString 并显示在密文显示框中
    QString ciphertextStr = convertToString(mingwen);
    ui->ciphertextEdit->setPlainText(ciphertextStr);

    // 释放内存
    for (int i = 0; i < 2; i++) {
        delete[] mingwen[i];
        delete[] key1[i];
        delete[] key2[i];
    }
    delete[] mingwen;
    delete[] key1;
    delete[] key2;
}

void MainWindow::decrypt()
{
    QString ciphertext = ui->ciphertextEdit->toPlainText();

    // 验证输入是否为有效的二进制字符串
    if (!isBinaryString(ciphertext)) {
        QMessageBox::warning(this, "错误", "密文必须是有效的二进制字符串。");
        return;
    }

    // 将 QString 转换为 int** 形式的密文
    int** ciphertextArray = convertToIntArray(ciphertext);

    // 获取原始密钥
    QString key = ui->keyEdit->toPlainText();
    int** keyArray = convertToIntArray(key);

    // 密钥扩展
    int** key1 = new int*[2];
    for (int i = 0; i < 2; i++)
        key1[i] = new int[8];
    int** key2 = new int*[2];
    for (int i = 0; i < 2; i++)
        key2[i] = new int[8];

    // 使用现有函数进行密钥扩展
    key1[0] = yihuo8(keyArray[0], g(keyArray[1], rcon1));
    key1[1] = yihuo8(key1[0], keyArray[1]);
    key2[0] = yihuo8(key1[0], g(key1[1], rcon2));
    key2[1] = yihuo8(key2[0], key1[1]);

    // 解密


    // 将解密后的明文转换为 QString 并显示在明文显示框中
    QString plaintextStr = convertToString(ciphertextArray);
    ui->decryptedPlaintextEdit->setPlainText(plaintextStr);

    // 释放内存
    for (int i = 0; i < 2; i++) {
        delete[] ciphertextArray[i];
        delete[] key1[i];
        delete[] key2[i];
        delete[] keyArray[i];
    }
    delete[] ciphertextArray;
    delete[] key1;
    delete[] key2;
    delete[] keyArray;
}
bool MainWindow::isBinaryString(const QString &str)
{
    for (QChar c : str) {
        if (c!= '0' && c!= '1') {
            return false;
        }
    }
    return true;
}

int** MainWindow::convertToIntArray(const QString &str)
{
    int rows = 2;
    int cols = 8;
    int** array = new int*[rows];
    for (int i = 0; i < rows; i++) {
        array[i] = new int[cols];
        for (int j = 0; j < cols; j++) {
            array[i][j] = str.at(i * cols + j).digitValue();
        }
    }
    return array;
}

QString MainWindow::convertToString(int** array)
{
    QString str;
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 8; j++) {
            str.append(QString::number(array[i][j]));
        }
    }
    return str;
}
