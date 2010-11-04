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
#include <QListWidgetItem>

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
    ui->showPlplayingTime->setChecked(pref->pl_show_playing_time);

    ui->statusFormat->setText(pref->status_bar_format);
    ui->statusFormat->setEnabled(ui->ShowStatusBar->isChecked());

    ui->windowFormat->setText(pref->window_title_format);

    fillPlaylistPref();

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


void PreferencesWindow::fillPlaylistPref()
{
    if (colnames.isEmpty()){
        colnames = pref->pl_columns_names.split("[;]");
        colformat = pref->pl_columns_format.split("[;]");
        colplayformat = pref->pl_columns_playing_format.split("[;]");
        colsize = pref->pl_columns_sizes.split(";");
    }

    ui->colList->clear();
    ui->colList->insertItems(0, colnames);

}


void PreferencesWindow::changePlPref()
{
    QListWidgetItem *item;
    item = ui->colList->item(curColumnIndex);

    item->setText(colnames.at(curColumnIndex));
}

void PreferencesWindow::on_colList_pressed(QModelIndex index)
{
    curColumnIndex = index.row();
    ui->colTitle->setText( index.data().toString() );
    ui->colFormat->setPlainText( colformat.at(curColumnIndex));
    ui->colPlayFormat->setPlainText( colplayformat.at(curColumnIndex));
    ui->colSize->setText( colsize.at(curColumnIndex));
}

void PreferencesWindow::on_colTitle_editingFinished()
{
    colnames.replace(curColumnIndex, ui->colTitle->text());
    fillPlaylistPref();
}

void PreferencesWindow::on_colSize_editingFinished()
{
    colsize.replace(curColumnIndex, ui->colSize->text());
}

void PreferencesWindow::on_colFormat_textChanged()
{
    colformat.replace(curColumnIndex, ui->colFormat->toPlainText());
}

void PreferencesWindow::on_colApply_clicked()
{
    emit playlist_changed(colnames, colformat, colplayformat, colsize);
}

void PreferencesWindow::on_colSave_clicked()
{
    pref->pl_columns_names = colnames.join("[;]");
    pref->pl_columns_format = colformat.join("[;]");
    pref->pl_columns_playing_format = colplayformat.join("[;]");
    pref->pl_columns_sizes = colsize.join(";");
}

void PreferencesWindow::on_colReset_clicked()
{
    colnames.clear();
    colformat.clear();
    colplayformat.clear();
    colsize.clear();

    fillPlaylistPref();
    ui->colList->setCurrentRow(curColumnIndex);

    ui->colTitle->setText( colnames.at(curColumnIndex) );
    ui->colFormat->setPlainText( colformat.at(curColumnIndex));
    ui->colPlayFormat->setPlainText( colplayformat.at(curColumnIndex));
    ui->colSize->setText( colsize.at(curColumnIndex));

    emit playlist_reset();
}

void PreferencesWindow::on_showPlplayingTime_toggled(bool checked)
{
    pref->pl_show_playing_time = checked;
    if (!checked){
        emit dontShowCurrentTimeInPl();
    }
}

void PreferencesWindow::on_colPlayFormat_textChanged()
{
    colplayformat.replace(curColumnIndex, ui->colPlayFormat->toPlainText());
}
