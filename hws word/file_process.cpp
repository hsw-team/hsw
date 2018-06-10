#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::open_file()//打开文件
{
    // 如果当前有没保存的先保存
    if ( Doc->isModified ){
        const QMessageBox::StandardButton ret
            = QMessageBox::warning( this, tr( "Miniword" ),
                        tr( "当前文档未保存，是否保存?" ),
                        QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel );
        switch ( ret )
        {
        case QMessageBox::Save:
            save_all();
            break;
        case QMessageBox::Discard:
            Doc->clear_all();
            break;
        case QMessageBox::Cancel:
            return;
        default:
            return;
        }
    }
    while(!Undo.isEmpty())
        Undo.pop();
    while(!Redo.isEmpty())
        Redo.pop();
    // 打开新的文档
    QString tmp_file_name = QFileDialog::getOpenFileName( this );
    //选择的文件不为空
    if ( !tmp_file_name.isEmpty() ){
        // 检查文件后缀是否为 txt
        QFileInfo file_info	= QFileInfo( tmp_file_name );
        QString file_suffix	= file_info.suffix();
        if ( file_suffix == "txt" ){
            //载入文件
            string str = tmp_file_name.toStdString();
            this->Doc->read_file( (char *) str.c_str() );
            Doc->file_name = tmp_file_name;

            refresh_screen();
        }
    }
    else{
        qDebug() << "未选中文件";
    }
}

/* 把内容保存到文件中 */
void MainWindow::save_all()
{
    //如果没有默认路径，选择保存路径
    if ( Doc->file_name.isEmpty() )
        Doc->file_name = QFileDialog::getSaveFileName( this, tr( "选择保存位置" ), "", tr( "*.txt" ) );
    //保存
    string str = Doc->file_name.toStdString();
    if (!(this->Doc->save_file( (char *) str.c_str()))){
        QMessageBox::information( this,
                      tr( "保存结果" ),
                      tr( "文件未保存!" ) );
        return;
    }
    while(!Undo.isEmpty())
        Undo.pop();
    while(!Redo.isEmpty())
        Redo.pop();
    refresh_screen();
}

void MainWindow::save_as()
{
    Doc->file_name = "";
    save_all();
    refresh_screen();
}

void MainWindow::new_file()
{
    //如果有未保存的修改，先提示是否保存
    if (this->Doc->isModified) {
        const QMessageBox::StandardButton ret
                = QMessageBox::warning(this, tr("Miniword"),
                                       tr("当前文档未保存，是否保存?"),
                                       QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        switch (ret) {
            case QMessageBox::Save:
                save_all();
                Doc->clear_all();
                break;
            case QMessageBox::Discard:
                Doc->clear_all();
                break;
            case QMessageBox::Cancel:
                break;
            default:
                break;
        }

    }
    else{
        Document *tmp = this->Doc;
        this->Doc = new Document;
        delete tmp;
    }
    while(!Undo.isEmpty())
        Undo.pop();
    while(!Redo.isEmpty())
        Redo.pop();
    refresh_screen();

}

void MainWindow::print_pdf()
{
    Row *temp = Doc->first_row;
    QString output_string = "";
    while ( temp )
    {
        output_string.append( temp->row_text );
        output_string.append( '\n' );
        temp = temp->next_row;
    }
    QTextEdit textEdit;
    textEdit.setText(output_string);

    QFileDialog fileDialog(this, tr("Export PDF"));
    fileDialog.setAcceptMode(QFileDialog::AcceptSave);
    fileDialog.setMimeTypeFilters(QStringList("application/pdf"));
    fileDialog.setDefaultSuffix("pdf");
    if (fileDialog.exec() != QDialog::Accepted)
        return;
    QString fileName = fileDialog.selectedFiles().first();
    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(fileName);
    textEdit.document()->print(&printer);
    QDesktopServices::openUrl(fileName);//自动打开输出的pdf
}


/* 显示 Help 函数 */
void MainWindow::show_help()//帮助
{
    QMessageBox::information( this, tr( "帮助" ), tr( "编辑文档。" ) );
}

/* 显示 About 函数 */
void MainWindow::show_about()//关于
{
    QMessageBox::about( this, tr( "关于 MiniWord" ), tr( "欢迎您的使用!\n"
                               "此程序 MiniWord 由 Daoxu Sheng, Weiran Huang\n 以及 Zengrui Wang 共同开发完成。" ) );
}

/* 主窗口 关闭 函数 */
void MainWindow::closeEvent( QCloseEvent *event )
{
    if (this->Doc->isModified){
        const QMessageBox::StandardButton ret
            = QMessageBox::warning( this, tr( "Miniword" ),
                        tr( "当前文档未保存，是否保存?" ),
                        QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel );
        switch ( ret )
        {
        case QMessageBox::Save:
            save_all();
            break;
        case QMessageBox::Discard:
            event->accept();
            break;
        case QMessageBox::Cancel:
            event->ignore();
            break;
        default:
            event->ignore();
            break;
        }
    }
}
