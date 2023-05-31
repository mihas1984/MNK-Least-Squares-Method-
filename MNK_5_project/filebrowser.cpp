#include "filebrowser.h"
#include "ui_filebrowser.h"

FileBrowser::FileBrowser(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FileBrowser)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    QString startPath = "C:/";
    //setting directory and file models:
    dirmodel = new QFileSystemModel(this);
    dirmodel->setFilter(QDir::NoDotAndDotDot | QDir::AllDirs);
    dirmodel->setRootPath(startPath);
    ui->treeView->setModel(dirmodel);
    //connecting:
    connect(this, SIGNAL(sendPath(QString)), parent, SLOT(recievePath(QString)));
    QObject::connect(ui->treeView, &QTreeView::clicked, this, &FileBrowser::treeViewClicked);
    QObject::connect(ui->listView, &QListView::clicked, this, &FileBrowser::listViewClicked);
    connect(ui->cancel_button, &QPushButton::clicked, this, &FileBrowser::cancelButtonClicked);
    connect(ui->choose_button, &QPushButton::clicked, this, &FileBrowser::chooseButtonClicked);
}

FileBrowser::~FileBrowser()
{
    delete ui;
}
void FileBrowser::treeViewClicked(const QModelIndex &index)
{
    QString startPath = dirmodel->fileInfo(index).absoluteFilePath();
    filemodel = new QFileSystemModel(this);
    filemodel->setFilter(QDir::NoDotAndDotDot | QDir::Files);
    ui->listView->setModel(filemodel);
    ui->listView->setRootIndex(filemodel->setRootPath(startPath));
}
void FileBrowser::listViewClicked(const QModelIndex &index)
{
    chosen_file_path = filemodel->fileInfo(index).absoluteFilePath();
}
void FileBrowser::chooseButtonClicked()
{
    if (chosen_file_path != ""){
        emit sendPath(chosen_file_path);
        this->accept();//Dialog closes
    }
}
void FileBrowser::cancelButtonClicked()
{
    this->reject();//Dialog closes
}
