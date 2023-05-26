#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QMainWindow>
#include <QMessageBox>
#include <QPropertyAnimation>
#include <QLineEdit>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void queryStudents();
    bool updateStudent(int id, const QString &name, int score, int num);
    bool insertStudent(int id, const QString &name, int score, int num);
    void createConnection();
    bool isDatabaseCreated();
    bool deleteStudent(int id);
    QString getStudentInfoById(int id);
    void shakeWindow(QWidget* window);
    void setLineEditPlaceholderText(QLineEdit* lineEdit, QString& placeholderText);
private slots:
    void on_exitButton_clicked();

    void on_addButton_clicked();

    void on_deleteButton_clicked();

    void on_updateButton_clicked();

    void on_selectAllButton_clicked();

    void on_selectByIdButton_clicked();

    void on_shake_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
