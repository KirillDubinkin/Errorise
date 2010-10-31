#include "preferenceswindow.h"
#include "ui_preferenceswindow.h"
#include "global.h"
#include "mainwindow.h"

#include <QDebug>
#include <QString>
#include <QFileDialog>
#include <QDir>
#include <QFileInfo>
#include <QFile>

using namespace Global;

PreferencesWindow::PreferencesWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PreferencesWindow)
{
    ui->setupUi(this);


    // connect(ui->treeWidget, SIGNAL(activated(QModelIndex)), this, SLOT(changeWindow(QModelIndex)));
    connect(ui->listWidget, SIGNAL(pressed(QModelIndex)), this, SLOT(changeWindow(QModelIndex)));

    ui->musFolder->setText(pref->music_library_path);
    ui->fileFilter->setText((pref->files_filter));
    ui->ShowStatusBar->setChecked(pref->status_bar);

    ui->showVolume->setEnabled(ui->ShowStatusBar->isChecked());

    ui->statusFormat->setText(pref->status_bar_format);
    ui->statusFormat->setEnabled(ui->ShowStatusBar->isChecked());

    ui->windowFormat->setText(pref->window_title_format);

}

PreferencesWindow::~PreferencesWindow()
{
    delete ui;
}


void PreferencesWindow::changeWindow(QModelIndex idx)
{
    ui->stackedWidget->setCurrentIndex(idx.row());
}


void PreferencesWindow::on_musFolderChange_clicked()
{
    QString filename = QFileDialog::getExistingDirectory(this,
                              tr("Choose Your Music Path"), pref->music_library_path);
    if (!filename.isEmpty()){
        pref->music_library_path = filename;
        ui->musFolder->setText(pref->music_library_path);
        emit music_folder_changed();
    }
}


void PreferencesWindow::on_musFolder_textEdited(QString s)
{
    if (QDir(s).isReadable()){
        pref->music_library_path = s;
        emit music_folder_changed();
    }
}

void PreferencesWindow::on_fileFilter_editingFinished()
{
    pref->files_filter = ui->fileFilter->text();
    emit file_filter_changed();
}

void PreferencesWindow::on_ShowStatusBar_toggled(bool checked)
{
    pref->status_bar = checked;
    ui->showVolume->setEnabled(checked);
    ui->statusFormat->setEnabled(checked);
    emit hide_status_bar(!checked);
}

void PreferencesWindow::on_statusFormat_textChanged(QString tex)
{
    pref->status_bar_format = tex;
}

void PreferencesWindow::on_windowFormat_textChanged(QString tex)
{
    pref->window_title_format = tex;
}
