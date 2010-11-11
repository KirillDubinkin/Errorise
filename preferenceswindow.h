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

    QStringList colnames, colformat, colback, colplayformat, colplayback, colsize;
    int curColumnIndex;

    void fillPlaylistPref();
    void changePlPref();

private slots:
    void on_groupTracks_toggled(bool checked);
    void on_useHTML_toggled(bool checked);
    void on_recursive_scan_toggled(bool checked);
    void on_rowHeight_textEdited(QString );
    void on_colBack_textChanged();
    void on_colPlayBack_textChanged();
    void on_colPlayFormat_textChanged();
    void on_showPlplayingTime_toggled(bool checked);
    void on_colReset_clicked();
    void on_colSave_clicked();
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
    void playlist_changed(const QStringList &names, const QStringList &format, const QStringList &back,
                          const QStringList &playformat, const QStringList &playback, const QStringList &sizes);
    void playlist_reset();
    void dontShowCurrentTimeInPl();

};

#endif // PREFERENCESWINDOW_H
