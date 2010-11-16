#include "preferenceswindow.h"
#include "ui_preferenceswindow.h"
#include "global.h"
#include "mainwindow.h"
#include <version.h>

#include <QDebug>
#include <QString>
#include <QFileDialog>
#include <QDir>
#include <QFileInfo>
#include <QFile>
#include <QListWidgetItem>
#include <QPalette>
#include <QColorDialog>

using namespace Global;

PreferencesWindow::PreferencesWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PreferencesWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Errorise v." + amplayerVersion() + " - Preferences");
    this->setGeometry(0, 0, pref->res_pref_width, pref->res_pref_height);


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
    ui->rowHeight->setText(QString().number(pref->pl_row_height));

    ui->recursive_scan->setChecked(pref->recursive_dirs);

    ui->useHTML->setChecked(pref->pl_use_html);
    ui->tabPlColStyleSheets->setEnabled(pref->pl_use_html);
    ui->plGroupStyle->setEnabled(pref->pl_use_html);
    ui->lblPlGroupStyle->setEnabled(pref->pl_use_html);
    ui->groupPlItemsColors->setEnabled(!pref->pl_use_html);
    ui->groupPlGroupColors->setEnabled(!pref->pl_use_html);

    ui->groupTracks->setChecked(pref->pl_use_groups);
    ui->tabPlGroups->setEnabled(pref->pl_use_groups);



    ui->plGroupStyle->setPlainText(pref->pl_groups_stylesheet);
    ui->plGroupColorText->setText(pref->pl_groups_text_color);
    ui->plGroupColorBack->setText(pref->pl_groups_back_color);
    ui->plGroupTextFormat->setPlainText(pref->pl_groups_format);

    ui->mainStylesheet->setPlainText(pref->main_stylesheet);
    ui->plStylesheet->setPlainText(pref->pl_stylesheet);



    ui->plAlternateColors->setChecked(pref->pl_alternate_colors);
//    ui->plColorText->setEnabled(!pref->pl_alternate_colors);
    ui->plColorBack->setEnabled(!pref->pl_alternate_colors);
//    ui->lblPlColorText->setEnabled(!pref->pl_alternate_colors);
    ui->lblPlColorBack->setEnabled(!pref->pl_alternate_colors);

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
        colback = pref->pl_columns_back.split("[;]");

        colplayformat = pref->pl_columns_playing_format.split("[;]");
        colplayback = pref->pl_columns_playng_back.split("[;]");

        colsize = pref->pl_columns_sizes.split(";");
    }


    ui->colAligment->clear();
    ui->colAligment->addItem("Left", "1");
    ui->colAligment->addItem("Right", "2");
    ui->colAligment->addItem("Center", "4");
    ui->colAligment->addItem("Justify", "8");


    ui->colList->clear();
    ui->colList->insertItems(0, colnames);

    ui->colList->setCurrentRow(0);
    this->on_colList_pressed(ui->colList->currentIndex());

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

    ui->colFormat->setPlainText( colformat.at(curColumnIndex) );
    ui->colBack->setPlainText( colback.at(curColumnIndex) );

    ui->colPlayFormat->setPlainText( colplayformat.at(curColumnIndex) );
    ui->colPlayBack->setPlainText( colplayback.at(curColumnIndex) );

    ui->colSize->setText( colsize.at(curColumnIndex) );

    switch (QString(pref->pl_columns_aligment.at(curColumnIndex)).toInt()){
    case 1:
        ui->colAligment->setCurrentIndex(0); break;
    case 2:
        ui->colAligment->setCurrentIndex(1); break;
    case 4:
        ui->colAligment->setCurrentIndex(2); break;
    case 8:
        ui->colAligment->setCurrentIndex(3); break;
    }

    ui->plColorText->setText(pref->pl_color_text.at(curColumnIndex));
    ui->plColorBack->setText(pref->pl_color_back.at(curColumnIndex));
    ui->plColorPlayText->setText(pref->pl_color_play_text.at(curColumnIndex));
    ui->plColorPlayBack->setText(pref->pl_color_play_back.at(curColumnIndex));
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
    emit playlist_changed(colnames, colformat, colback, colplayformat, colplayback, colsize);
}

void PreferencesWindow::on_colSave_clicked()
{
    pref->pl_columns_names = colnames.join("[;]");

    pref->pl_columns_format = colformat.join("[;]");
    pref->pl_columns_back = colback.join("[;]");

    pref->pl_columns_playing_format = colplayformat.join("[;]");
    pref->pl_columns_playng_back = colplayback.join("[;]");

    pref->pl_columns_sizes = colsize.join(";");
}

void PreferencesWindow::on_colReset_clicked()
{
    colnames.clear();
    colformat.clear();
    colback.clear();
    colplayformat.clear();
    colplayback.clear();
    colsize.clear();


    fillPlaylistPref();

    ui->colList->setCurrentRow(curColumnIndex);
    ui->colTitle->setText( colnames.at(curColumnIndex) );

    ui->colFormat->setPlainText( colformat.at(curColumnIndex) );
    ui->colBack->setPlainText( colback.at(curColumnIndex) );

    ui->colPlayFormat->setPlainText( colplayformat.at(curColumnIndex) );
    ui->colPlayBack->setPlainText( colplayback.at(curColumnIndex) );

    ui->colSize->setText( colsize.at(curColumnIndex) );

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

void PreferencesWindow::on_colPlayBack_textChanged()
{
    colplayback.replace(curColumnIndex, ui->colPlayBack->toPlainText());
}

void PreferencesWindow::on_colBack_textChanged()
{
    colback.replace(curColumnIndex, ui->colBack->toPlainText());
}

void PreferencesWindow::on_rowHeight_textEdited(QString tex)
{
    pref->pl_row_height = tex.toInt();
}

void PreferencesWindow::on_recursive_scan_toggled(bool checked)
{
    pref->recursive_dirs = checked;
}

void PreferencesWindow::on_useHTML_toggled(bool checked)
{
    pref->pl_use_html = checked;

    ui->tabPlColStyleSheets->setEnabled(checked);
    ui->lblPlGroupStyle->setEnabled(checked);
    ui->plGroupStyle->setEnabled(checked);

    ui->groupPlGroupColors->setEnabled(!checked);
    ui->groupPlItemsColors->setEnabled(!checked);
}

void PreferencesWindow::on_groupTracks_toggled(bool checked)
{
    pref->pl_use_groups = checked;
    ui->tabPlGroups->setEnabled(checked);
}

void PreferencesWindow::on_colAligment_currentIndexChanged(int index)
{
    if (this->isVisible()){
        switch (index){
        case 0: pref->pl_columns_aligment.replace(curColumnIndex, "1"); break;
        case 1: pref->pl_columns_aligment.replace(curColumnIndex, "2"); break;
        case 2: pref->pl_columns_aligment.replace(curColumnIndex, "4"); break;
        case 3: pref->pl_columns_aligment.replace(curColumnIndex, "8"); break;
        }
    }
}

void PreferencesWindow::on_plRemoveCol_clicked()
{
    int idx = ui->colList->currentRow();

    if (idx > -1){
       // this->on_colSave_clicked();

        // this's simple, but i think we need qstringlists in preferences, not qstrings
        // so.. it's will be next, after

    }
}

void PreferencesWindow::on_mainStylesheet_textChanged()
{
    pref->main_stylesheet = ui->mainStylesheet->toPlainText();
    emit mainStyleChanged(pref->main_stylesheet);
}

void PreferencesWindow::on_plStylesheet_textChanged()
{
    pref->pl_stylesheet = ui->plStylesheet->toPlainText();
    emit plStyleChanged(pref->pl_stylesheet);
}

void PreferencesWindow::on_plAlternateColors_toggled(bool checked)
{
    pref->pl_alternate_colors = checked;

//    ui->plColorText->setEnabled(!checked);
    ui->plColorBack->setEnabled(!checked);
//    ui->lblPlColorText->setEnabled(!checked);
    ui->lblPlColorBack->setEnabled(!checked);
}

void PreferencesWindow::on_plColorText_editingFinished()
{
    pref->pl_color_text.replace(curColumnIndex, ui->plColorText->text());
}

void PreferencesWindow::on_plColorBack_editingFinished()
{
    pref->pl_color_back.replace(curColumnIndex, ui->plColorBack->text());
}

void PreferencesWindow::on_plColorPlayText_editingFinished()
{
    pref->pl_color_play_text.replace(curColumnIndex, ui->plColorPlayText->text());
}

void PreferencesWindow::on_plColorPlayBack_editingFinished()
{
    pref->pl_color_play_back.replace(curColumnIndex, ui->plColorPlayBack->text());
}

void PreferencesWindow::on_btnPlColorText_clicked()
{
    ui->plColorText->setText(this->getRGBText(ui->plColorText->text()));
    this->on_plColorText_editingFinished();
}

void PreferencesWindow::on_btnPlColorBack_clicked()
{
    ui->plColorBack->setText(this->getRGBText(ui->plColorBack->text()));
    this->on_plColorBack_editingFinished();
}

void PreferencesWindow::on_btnPlColorPlayText_clicked()
{
    ui->plColorPlayText->setText(this->getRGBText(ui->plColorPlayText->text()));
    this->on_plColorPlayText_editingFinished();
}

void PreferencesWindow::on_btnPlColorPlayBack_clicked()
{
    ui->plColorPlayBack->setText(getRGBText(ui->plColorPlayBack->text()));
    this->on_plColorPlayBack_editingFinished();
}


QString PreferencesWindow::getRGBText(const QString &initColor)
{
    bool ok;
    int r=0, g=0, b=0;

    QColor(QColorDialog::getColor(QColor(QRgb(initColor.toInt(&ok, 16))))).getRgb(&r, &g, &b);
    return pref->getHex(r, g, b);
}
