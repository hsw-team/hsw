#include "document.h"
#include "mainwindow.h"
Row::Row()
{
    qDebug() << " Row 构造函数\n";
    this->row_text = (char*)calloc(1, sizeof(char) * 100);
    this->Next_Row = NULL;
    this->Prev_Row = NULL;
    this->cur_len = 0;
    this->max_len = 100;

}
void Row::add_block()
{
    qDebug() << "调用 add_block()\n";

    this->row_text = (char*)realloc(row_text, this->max_len + 100 * sizeof(char));

    this->max_len += 100;
}
/*void Row::edit(char *s)
{
    qDebug() << "调用 edit()\n";
    int s_len = strlen(s);
    while (s_len >= (this->max_len - this->cur_len)) {
        this->add_block();
        qDebug() << "edit循环\n";
    }
    if (this->cur_len == 0) {
        strcpy(this->row_text, s);
        this->cur_len += s_len;
    }
    else {
        strcat(this->row_text, s);
        this->cur_len += s_len;
    }
}*/


//=========================================================================================

Document::Document()
{
    qDebug() << " Document 构造函数\n";
    //创建第一行
    this->first_row = new Row;
    this->cursor.hang = this->first_row;
    this->first_row->Prev_Row = NULL;
    //光标初始化
    cursor.hang = first_row;
    cursor.col = 0;
}
void Document::edit(char *s)
{
    qDebug() << "***DOCUMNET*** 调用 edit()\n";
    int s_len = strlen(s);
    while (s_len >= (this->cursor.hang->max_len - this->cursor.hang->cur_len)) {
        this->cursor.hang->add_block();
        qDebug() << "edit循环";
    }
    this->cursor.hang->cur_len += s_len;

    for(int i = cursor.hang->cur_len - 1; i >= cursor.col; i--)
    {
        cursor.hang->row_text[i + s_len] = cursor.hang->row_text[i];
    }

    for(int j = 0; j < s_len; j++){
        cursor.hang->row_text[j + cursor.col] = s[j];
    }
    this->cursor.col += s_len;
}

void Document::add_row(Row *ptr)
{
    Row *tmp = new Row;
    tmp->Prev_Row = ptr;
    tmp->Next_Row = ptr->Next_Row;
    if(ptr->Next_Row)
        ptr->Next_Row->Prev_Row = tmp;
    ptr->Next_Row = tmp;
    ptr = tmp;
}
void Document::show_doc()
{
    qDebug() << "调用 show_doc()\n";
    Row *ptr = this->first_row;
    while (ptr) {
        qDebug() << "show_doc 循环\n";
        qDebug() << ptr->row_text << endl;
        ptr = ptr->Next_Row;
    }
}

//读取文件到document对象，文件路径为char *s;
void Document::read_file(char *s)
{
    qDebug() << "调用 read_file 函数";
    QFile inputFile(s);
    QString line_text;
    string str;
    if(inputFile.open(QIODevice::ReadOnly)){
        QTextStream in(&inputFile);
        while(in.readLineInto(&line_text)){
            qDebug()<<line_text;
            str = line_text.toStdString() + "\n";
            this->edit((char*)str.c_str());
            //qDebug() << "@@ " << this->cur_row->row_text << "## " << this->cur_row;
            this->add_row(this->cursor.hang);
        }
        inputFile.close();
    }
}

void Document::save_file(char *file)
{

}

//===========================================================================================


void Document::cursor_left()
{
    qDebug() << "@Cursor Left";
    if(cursor.col == 0){
        if(cursor.hang->Prev_Row){
            cursor.hang = cursor.hang->Prev_Row;
            cursor.col = cursor.hang->cur_len - 1;
        }
    }
    else{
        cursor.col--;
    }
}
void Document::cursor_right()
{
    qDebug() << "@Cursor Left";
    if(cursor.col == 0){
        if(cursor.hang->Prev_Row){
            cursor.hang = cursor.hang->Prev_Row;
            cursor.col = cursor.hang->cur_len;
            //MODIFIED
        }
    }
    else{
        //TODO:
        //判断中文字符
        cursor.col--;
    }
}
void Document::cursor_up()
{
    qDebug() << "@Cursor up";
    if(cursor.hang->Prev_Row){
        cursor.hang = cursor.hang->Prev_Row;
        if(cursor.col > cursor.hang->cur_len){
            cursor.col = cursor.hang->cur_len;
        }
    }
}
void Document::cursor_down()
{
    qDebug() << "@Cursor down";
    if(cursor.hang->Next_Row){
        cursor.hang = cursor.hang->Next_Row;
        if(cursor.col > cursor.hang->cur_len){
            cursor.col = cursor.hang->cur_len;
        }
    }
}
void Document::cursor_home()
{
    qDebug() << "@Cursor home";
    cursor.hang = first_row;
    cursor.col = 0;
}
void Document::cursor_end()
{
    qDebug() << "@Cursor end";
    while(cursor.hang->Next_Row){
        cursor.hang = cursor.hang->Next_Row;
    }
    cursor.col = cursor.hang->cur_len;
}










