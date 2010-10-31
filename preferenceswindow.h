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

private slots:
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

};

#endif // PREFERENCESWINDOW_H
