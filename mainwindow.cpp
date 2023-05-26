#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QString IdContent = "请输入ID";
    MainWindow::setLineEditPlaceholderText(ui->idEdit, IdContent);
    QString NameContent = "请输入名字";
    MainWindow::setLineEditPlaceholderText(ui->nameEdit, NameContent);
    QString NumContent = "请输入学号";
    MainWindow::setLineEditPlaceholderText(ui->numEdit, NumContent);
    QString ScoreContent = "请输入分数";
    MainWindow::setLineEditPlaceholderText(ui->scoreEdit, ScoreContent);
    MainWindow::createConnection();
    if(MainWindow::isDatabaseCreated()){
        qDebug() << "database已连接";
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_shake_clicked() //窗口抖动
{
    MainWindow::shakeWindow(this);
}


void MainWindow::on_exitButton_clicked()
{
    this->close();
}


void MainWindow::on_addButton_clicked() //增加操作
{
    int id = ui->idEdit->text().toInt();
    QString name = ui->nameEdit->text();
    int num =  ui->numEdit->text().toInt();
    int score = ui->scoreEdit->text().toInt();
    insertStudent(id, name, score, num);
    QMessageBox::information(this, "提示","success");

}


void MainWindow::on_deleteButton_clicked() //删除操作
{
    int id = ui->idEdit->text().toInt();
    MainWindow::deleteStudent(id);
}


void MainWindow::on_updateButton_clicked() //修改操作
{
    int id = ui->idEdit->text().toInt();
    QString name = ui->nameEdit->text();
    int score = ui->scoreEdit->text().toInt();
    int num = ui->numEdit->text().toInt();
    MainWindow::updateStudent(id, name, score, num);
    QMessageBox::information(this, "提示","success");
}


void MainWindow::on_selectAllButton_clicked() //查询操作
{
    MainWindow::queryStudents();
    QMessageBox::information(this, "提示","查询所有成功");
}


void MainWindow::on_selectByIdButton_clicked()
{
    int id = ui->idEdit->text().toInt();
    QString str = MainWindow::getStudentInfoById(id);
    ui->showList->append(str);
    QMessageBox::information(this, "提示","根据id查询成功！");
}

 //创建数据库
void MainWindow::createConnection() {
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("students.db");

    if (!db.open()) {
        qDebug() << "Error: Unable to open database" << db.lastError();
    }

    QSqlQuery query;
    if (!query.exec("CREATE TABLE IF NOT EXISTS students (id INTEGER PRIMARY KEY, name TEXT, score INTEGER, num INTEGER)")) {
        qDebug() << "Error: Unable to create table" << query.lastError();
    }
}
// 增加数据
bool MainWindow::insertStudent(int id, const QString &name, int score, int num) {
    QSqlQuery query;
    query.prepare("INSERT INTO students (id, name, score, num) VALUES (:id, :name, :age, :num)");
    query.bindValue(":id", id);
    query.bindValue(":name", name);
    query.bindValue(":score", score);
    query.bindValue(":num", num);

    if (!query.exec()) {
        qDebug() << "Error: Unable to insert data" << query.lastError();
        return false;
    }

    return true;
}
// 删除数据
bool MainWindow::deleteStudent(int id) {
    QSqlQuery query;
    query.prepare("DELETE FROM students WHERE id = :id");
    query.bindValue(":id", id);
    QMessageBox::information(this, "提示","根据id删除成功！");
    if (!query.exec()) {
        qDebug() << "Error: Unable to delete data" << query.lastError();
        return false;
    }

    return true;
}

// 修改数据
bool MainWindow::updateStudent(int id, const QString &name, int score, int num) {
    QSqlQuery query;
    query.prepare("UPDATE students SET name = :name, score = :score, num = :num WHERE id = :id");
    query.bindValue(":name", name);
    query.bindValue(":score", score);
    query.bindValue(":id", id);
    query.bindValue(":num", num);

    if (!query.exec()) {
        qDebug() << "Error: Unable to update data" << query.lastError();
        return false;
    }

    return true;
}
// 查询数据
void MainWindow::queryStudents() {
    QSqlQuery query("SELECT id, name, score, num FROM students");

    while (query.next()) {
        int id = query.value(0).toInt();
        QString name = query.value(1).toString();
        int score = query.value(2).toInt();
        int num = query.value(3).toInt();
        QString str = "ID: %1, 名字：%2, 成绩: %3, 学号：%4";
        str = str.arg(id).arg(name).arg(score).arg(num);
        ui->showList->append(str);
        qDebug() << "ID:" << id << ", Name:" << name << ", Score:" << score << ",Num:" << num;
    }
}
// 检查是否连接
bool MainWindow::isDatabaseCreated() {
    QSqlDatabase db = QSqlDatabase::database();
    if (db.isOpen()) {
        return true;
    } else {
        return false;
    }
}
// 根据id查询
QString MainWindow::getStudentInfoById(int id) {
    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen()) {
        qDebug() << "Error: Database is not open";
        return "";
    }

    QSqlQuery query;
    query.prepare("SELECT name, score, num FROM students WHERE id=:id");
    query.bindValue(":id", id);

    if (!query.exec()) {
        qDebug() << "Error: Unable to execute query" << query.lastError();
        return "";
    }

    if (query.next()) {
        QString name = query.value(0).toString();
        int score = query.value(1).toInt();
        int num = query.value(2).toInt();
        return QString("名字: %1 ，成绩: %2，分数: %3").arg(name).arg(score).arg(num);
    } else {
        return "error";
    }
}


void MainWindow::shakeWindow(QWidget* window) {
    QPropertyAnimation* animation = new QPropertyAnimation(window, "pos");
    animation->setDuration(100);
    animation->setLoopCount(5);

    int originalX = window->pos().x();
    int originalY = window->pos().y();

    QList<QVariant> xValues;
    xValues << originalX - 5 << originalX + 5 << originalX - 5 << originalX + 5 << originalX;

    QList<QVariant> yValues;
    yValues << originalY << originalY << originalY - 5 << originalY - 5 << originalY;

    animation->setKeyValueAt(0, QPoint(originalX, originalY));
    animation->setKeyValueAt(0.2, QPoint(xValues.at(0).toInt(), yValues.at(0).toInt()));
    animation->setKeyValueAt(0.4, QPoint(xValues.at(1).toInt(), yValues.at(1).toInt()));
    animation->setKeyValueAt(0.6, QPoint(xValues.at(2).toInt(), yValues.at(2).toInt()));
    animation->setKeyValueAt(0.8, QPoint(xValues.at(3).toInt(), yValues.at(3).toInt()));
    animation->setKeyValueAt(1, QPoint(xValues.at(4).toInt(), yValues.at(4).toInt()));

    animation->start(QAbstractAnimation::DeleteWhenStopped);
}

void MainWindow::setLineEditPlaceholderText(QLineEdit* lineEdit, QString& placeholderText) {
    lineEdit->setPlaceholderText(placeholderText);
    lineEdit->setStyleSheet("QLineEdit { color: gray; }");

    QObject::connect(lineEdit, &QLineEdit::textChanged, [=]() {
        if (lineEdit->text().isEmpty()) {
            lineEdit->setStyleSheet("QLineEdit { color: gray; }");
        } else {
            lineEdit->setStyleSheet("");
        }
    });

    QObject::connect(lineEdit, &QLineEdit::editingFinished, [=]() {
        if (lineEdit->text().isEmpty()) {
            lineEdit->setStyleSheet("QLineEdit { color: gray; }");
        } else {
            lineEdit->setStyleSheet("");
        }
    });

    QObject::connect(lineEdit, &QLineEdit::selectionChanged, [=]() {
        if (lineEdit->text().isEmpty()) {
            lineEdit->setStyleSheet("QLineEdit { color: gray; }");
        } else {
            lineEdit->setStyleSheet("");
        }
    });
}


