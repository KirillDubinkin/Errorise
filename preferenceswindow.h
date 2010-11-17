#ifndef PREFERENCESWINDOW_H
#define PREFERENCESWINDOW_H

#include <QDialog>
#include <QModelIndex>

namespace Ui {
    class PreferencesWindow;
}

class PreferencesWindow : public QDialog
{
    Q_OBJECT

public:
    explicit PreferencesWindow(QWidget *parent = 0);
    ~PreferencesWindow();

public slots:
    void changeWindow(QModelIndex idx);

private:
    Ui::PreferencesWindow *ui;

    QStringList colformat, colback, colplayformat, colplayback, colsize;
    int curColumnIndex;

    void fillPlaylistPref();
    void changePlPref();
    QString getRGBText(const QString &initColor);

private slots:
    void on_plGroupStyle_textChanged();
    void on_plGroupTextFormat_textChanged();
    void on_plGroupsLabels_toggled(bool checked);
    void on_btnPlGroupBackColor_clicked();
    void on_plGroupColorBack_editingFinished();
    void on_plGroupColorText_editingFinished();
    void on_btnPlGroupTextColor_clicked();
    void on_btnPlColorPlayBack_clicked();
    void on_btnPlColorPlayText_clicked();
    void on_btnPlColorBack_clicked();
    void on_btnPlColorText_clicked();
    void on_plColorPlayBack_editingFinished();
    void on_plColorPlayText_editingFinished();
    void on_plColorBack_editingFinished();
    void on_plColorText_editingFinished();
    void on_plAlternateColors_toggled(bool checked);
    void on_plStylesheet_textChanged();
    void on_mainStylesheet_textChanged();
    void on_plRemoveCol_clicked();
    void on_colAligment_currentIndexChanged(int index);
    void on_groupTracks_toggled(bool checked);
    void on_useHTML_toggled(bool checked);
    void on_recursive_scan_toggled(bool checked);
    void on_rowHeight_textEdited(QString );
    void on_colBack_textChanged();
    void on_colPlayBack_textChanged();
    void on_colPlayFormat_textChanged();
    void on_showPlplayingTime_toggled(bool checked);

    void on_colApply_clicked();
    void on_colFormat_textChanged();
    void on_colSize_editingFinished();
    void on_colTitle_editingFinished();
    void on_colList_pressed(QModelIndex index);
    void on_windowFormat_textChanged(QString );
    void on_statusFormat_textChanged(QString );
    void on_ShowStatusBar_toggled(bool checked);
    void on_fileFilter_editingFinished();
    void on_musFolderChange_clicked();
    void on_musFolder_textEdited(QString );


signals:
    void music_folder_changed();
    void file_filter_changed();
    void hide_status_bar(bool checked);
    void playlist_changed();

    void dontShowCurrentTimeInPl();

    void mainStyleChanged(QString stylesheet);
    void plStyleChanged(QString stylesheet);

};

#endif // PREFERENCESWINDOW_H
